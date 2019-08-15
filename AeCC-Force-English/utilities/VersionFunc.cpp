#include "stdafx.h"
#include "VersionFunc.h"
#include "Tools.h"
#include "URLs.h"
#include <afxinet.h>


namespace VersionFunc {
    bool GetVersionInfo(FourIntegerVersionNumber &fourIntegerVersionNumber)
    {
        CString webPageContent;
        if (QueryWebpage(urls::Release_API, webPageContent, fourIntegerVersionNumber.errorInfo) &&
            GetLatestVersion(fourIntegerVersionNumber, webPageContent, fourIntegerVersionNumber.errorInfo)) {
            return true;
        }
        return false;
    }

    bool QueryWebpage(LPCTSTR url, CString &result, CString &errorInfo)
    {
        CInternetSession session;
        CHttpFile *pHttpFile = nullptr;

        try {
            pHttpFile = (CHttpFile*)session.OpenURL(
                url,
                1,
                INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
        }
        catch (CInternetException *ex) {
            tools::DWORDErrorCodeToCString(CString(_T("CInternetSession::OpenURL(")) + url, ex->m_dwError, errorInfo);
            ex->Delete();
            delete pHttpFile;
            return false;
        }

        DWORD dwStatusCode;
        if (!pHttpFile->QueryInfoStatusCode(dwStatusCode)) {
            tools::DWORDErrorCodeToCString(_T("CHttpFile::QueryInfoStatusCode(DWORD&)"), GetLastError(), errorInfo);
            pHttpFile->Close();
            delete pHttpFile;
            return false;
        }
        if (HTTP_STATUS_OK != dwStatusCode) {
            errorInfo.Format(_T("%s\r\nHTTP %u"), url, dwStatusCode);
            pHttpFile->Close();
            delete pHttpFile;
            return false;
        }

        CString strContentType;
        if (!pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_TYPE, strContentType)) {
            tools::DWORDErrorCodeToCString(_T("CHttpFile::QueryInfo(DWORD, CString&)"), GetLastError(), errorInfo);
            pHttpFile->Close();
            delete pHttpFile;
            return false;
        }
        if (-1 == strContentType.Find(JSON_ContentType)) {
            errorInfo.Format(_T("ContentType: %s != %s"), strContentType.GetString(), JSON_ContentType);
            pHttpFile->Close();
            delete pHttpFile;
            return false;
        }

        CString buffer;
        try {
            while (pHttpFile->ReadString(buffer) != NULL) {
                result += buffer;
            }
        }
        catch (CInternetException *ex) {
            tools::DWORDErrorCodeToCString(_T("CInternetFile::Read(CString&)"), ex->m_dwError, errorInfo);
            ex->Delete();
            pHttpFile->Close();
            delete pHttpFile;
            return false;
        }
        // https://social.msdn.microsoft.com/Forums/en-US/508cce4d-c1d6-4398-b991-ac67c7093153/cinternetsession-open-files-via-internet#e3a024a7-b2b7-4583-a8b4-cc5c95314568
        // Throw away MultiByteToWideChar()
        result = CString((LPSTR)result.GetString());
        pHttpFile->Close();
        delete pHttpFile;
        return true;
    }

    bool GetLatestVersion(FourIntegerVersionNumber &fourIntegerVersionNumber, CString &webPageContent, CString &errorInfo)
    {        
        typedef rapidjson::GenericDocument<rapidjson::UTF16<>> WDocument;
        WDocument document;
        rapidjson::ParseResult parseResult = document.Parse(webPageContent.GetString());
        if (!parseResult) {
            errorInfo.Format(_T("WDocument.Parse(wchar_t*) %s\r\n%u"), rapidjson::GetParseError_En(parseResult.Code()), parseResult.Offset());
            return false;
        }
        if (!document.IsObject()) {
            errorInfo = _T("!WDocument.IsObject()");
            return false;
        }
        if (!document.HasMember(Tag_Name_JSON_Key)) {
            errorInfo.Format(_T("!WDocument.HasMember(%s)"), Tag_Name_JSON_Key);
            return false;
        }
        if (!document[Tag_Name_JSON_Key].IsString()) {
            errorInfo.Format(_T("!WDocument[%s].IsString()"), Tag_Name_JSON_Key);
            return false;
        }
        auto tagname = document[Tag_Name_JSON_Key].GetString();

        std::wcmatch matched_groups;
        std::regex_match(tagname, matched_groups, tagname_versionNumber_pattern);
        if (matched_groups.empty()) {
            errorInfo.Format(_T("!std::regex_match(%s, matched_groups, tagname_versionNumber_pattern)"), tagname);
            return false;
        }
        else {
            fourIntegerVersionNumber.Major = std::stoi(matched_groups[1]);
            fourIntegerVersionNumber.Minor = std::stoi(matched_groups[2]);
            fourIntegerVersionNumber.Patch = std::stoi(matched_groups[3]);
            fourIntegerVersionNumber.Build = std::stoi(matched_groups[4]);
            return true;
        }
    }

    bool GetMyVersion(FourIntegerVersionNumber &fivm)
    {
        auto hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
        if (hRsrc == NULL) {
            fivm.errorInfo = CString(_T("FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION) == NULL"));
            return false;
        }
        auto hGlobal = LoadResource(NULL, hRsrc);
        if (hGlobal == NULL) {
            fivm.errorInfo = CString(_T("LoadResource(NULL, hRsrc) == NULL"));
            return false;
        }
        auto *pByte = (BYTE *)LockResource(hGlobal);
        auto *pFixedFileInfo = (VS_FIXEDFILEINFO*)(pByte + 40);

        fivm.Major = (int)((pFixedFileInfo->dwFileVersionMS >> 16) & 0xFF);
        fivm.Minor = (int)((pFixedFileInfo->dwFileVersionMS) & 0xFF);
        fivm.Patch = (int)((pFixedFileInfo->dwFileVersionLS >> 16) & 0xFF);
        fivm.Build = (int)((pFixedFileInfo->dwFileVersionLS) & 0xFF);

        return true;
    }
}
