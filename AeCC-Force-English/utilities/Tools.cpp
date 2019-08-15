#include "stdafx.h"
#include "Tools.h"


namespace tools {

    void HResultToCString(CString source, HRESULT hResult, CString &result)
    {
        DWORD errorcode;
        if ((hResult & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0)) {
            errorcode = HRESULT_CODE(hResult);
        }
        else if (hResult == S_OK) {
            errorcode = ERROR_SUCCESS;
        }
        else {
            errorcode = ERROR_CAN_NOT_COMPLETE;// Not a Win32 HRESULT so return a generic error code.
        }
        DWORDErrorCodeToCString(source, errorcode, result);
    }
    //void HResultToCString(std::wstring source, HRESULT hResult, std::wstring &result)
    //{
    //    DWORD errorcode;
    //    if ((hResult & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0)) {
    //        errorcode = HRESULT_CODE(hResult);
    //    }
    //    else if (hResult == S_OK) {
    //        errorcode = ERROR_SUCCESS;
    //    }
    //    else {
    //        errorcode = ERROR_CAN_NOT_COMPLETE;// Not a Win32 HRESULT so return a generic error code.
    //    }
    //    DWORDErrorCodeToCString(source, errorcode, result);
    //}

    //Can be better?
    void DWORDErrorCodeToCString(CString source, DWORD errorcode, CString &result)
    {
        LPTSTR buffer = nullptr;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE,
            GetModuleHandle(_T("wininet.dll")),
            errorcode,
            0,
            (LPTSTR)&buffer,
            0,
            NULL);
        if (nullptr == buffer) {
            result.Format(_T("%s\r\n%u"), source.GetString(), errorcode);
        }
        else {
            result.Format(_T("%s\r\n%s"), source.GetString(), buffer);
        }
    }
    //void DWORDErrorCodeToCString(std::wstring source, DWORD errorcode, std::wstring &result)// #include <minwindef.h>
    //{
    //    wchar_t *buffer = nullptr;
    //    FormatMessage(// #include <winbase.h>
    //        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE,
    //        GetModuleHandle(_T("wininet.dll")),
    //        errorcode,
    //        0,
    //        (wchar_t *)&buffer,
    //        0,
    //        NULL);
    //    if (nullptr == buffer) {
    //        result = (boost::wformat(L"%1%\r\n%2%") %
    //            source %
    //            errorcode
    //            ).str();
    //    }
    //    else {
    //        result = (boost::wformat(L"%1%\r\n%2%") %
    //            source %
    //            buffer
    //            ).str();
    //    }
    //}

    //Unused
    void ErrnotToCString(CString source, errno_t error, CString &result)
    {
        char *buffer = new char[95];//... Your string message can be, at most, 94 characters long...
        strerror_s(buffer, 95, error);//... These functions truncate the error message if its length exceeds numberOfElements -1. The resulting string in buffer is always null-terminated. My bad CPP :(
        result.Format(_T("%s\r\n%s"), source.GetString(), CString(buffer).GetString());
        delete[] buffer;
    }
    //void ErrnotToCString(std::wstring source, errno_t error, std::wstring &result)
    //{
    //    char *buffer = new char[95];//... Your string message can be, at most, 94 characters long...
    //    strerror_s(buffer, 95, error);//... These functions truncate the error message if its length exceeds numberOfElements -1. The resulting string in buffer is always null-terminated. My bad CPP :(
    //    result = (boost::wformat(L"%1%\r\n%2%") %
    //        source %
    //        std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(buffer)
    //        ).str();
    //    delete[] buffer;
    //}

    //Unused
    int MultiByteToCStringUTF8(LPCCH lpMultiByteStr, CString &result)
    {
        int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, NULL, 0);
        LPTSTR lpWideCharStr = new wchar_t[cchWideChar];
        int returns = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
        result = CString(lpWideCharStr);
        delete[] lpWideCharStr;
        return returns;
    }
    //int MultiByteToCStringUTF8(const char *lpMultiByteStr, std::wstring &result)
    //{
    //    int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, NULL, 0);
    //    wchar_t *lpWideCharStr = new wchar_t[cchWideChar];
    //    int returns = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
    //    result = std::wstring(lpWideCharStr);
    //    delete[] lpWideCharStr;
    //    return returns;
    //}
}
