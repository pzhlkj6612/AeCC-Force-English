#include "stdafx.h"
#include "FileFunc.h"
#include "Tools.h"


namespace FileFunc {
    bool GetTheFilePath(CString &result, CString &errorInfo)
    {
        if (GetUserSpecialPath(FOLDERID_Documents, result, errorInfo)) {
            result += CString(_T("\\ae_force_english.txt"));
            return true;
        }
        return false;
    }

    bool TargetExisted(LPCTSTR path) {
        DWORD dwAttrib = GetFileAttributes(path);
        if (INVALID_FILE_ATTRIBUTES != dwAttrib) {
            return true;
        }
        else {
            return false;
        }
    }

    TARGET_FILE_PROCESS_RESULT processTarget(LPCTSTR path, CString &errorInfo) {
        DWORD dwAttrib = GetFileAttributes(path);
        if (TargetExisted(path)) {
            //Existed
            if (FILE_ATTRIBUTE_DIRECTORY != dwAttrib) {
                //Is file
                if (DeleteFile(path)) {
                    return DELETE_SUCCESS;//Delete file successful
                }
                else {
                    tools::DWORDErrorCodeToCString(CString(_T("DeleteFile(")) + CString(path), GetLastError(), errorInfo);
                    return OPERATE_FAILURE;//Delete file failed
                }
            }
            else {
                //Is directory
                if (RemoveDirectory(path)) {
                    return DELETE_SUCCESS;//Delete dictionary successful
                }
                else {
                    tools::DWORDErrorCodeToCString(CString(_T("RemoveDirectory(")) + CString(path), GetLastError(), errorInfo);
                    return OPERATE_FAILURE;//Delete dictionary failed
                }
            }
        }
        else {
            //Not existed
            HANDLE hFile = CreateFile(path, GENERIC_ALL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            if (INVALID_HANDLE_VALUE == hFile) {
                tools::DWORDErrorCodeToCString(CString(_T("Create(")) + CString(path), GetLastError(), errorInfo);
                CloseHandle(hFile);
                return OPERATE_FAILURE;//Create file failed
            }
            else {
                CloseHandle(hFile);
                return CREATE_SUCCESS;//Create file successful
            }
        }
    }

    bool GetUserSpecialPath(REFKNOWNFOLDERID rfid, CString &result, CString &errorInfo)
    {
        PWSTR ppszPath;
        HRESULT hResult_SHGetKnownFolderPath;
        if (ERROR_SEVERITY_SUCCESS != (hResult_SHGetKnownFolderPath = SHGetKnownFolderPath(rfid, 0, NULL, &ppszPath))) {
            LPTSTR strCLSID;
            auto hResult_StringFromCLSID = StringFromCLSID(rfid, &strCLSID);
            if (S_OK != hResult_StringFromCLSID) {
                tools::HResultToCString(CString(_T("StringFromCLSID")), hResult_StringFromCLSID, errorInfo);
                return false;
            }
            tools::HResultToCString(CString(_T("SHGetKnownFolderPath(")) + CString(strCLSID), hResult_SHGetKnownFolderPath, errorInfo);
            return false;
        }
        result = CString(ppszPath);
        return true;
    }

    //Unused
    bool GetEnvString(LPCTSTR envName, CString &result, CString &errorInfo)
    {
        int bufferSize = 32767;
        LPTSTR buffer = new TCHAR[bufferSize];
        if (!GetEnvironmentVariable(envName, buffer, bufferSize)) {
            tools::DWORDErrorCodeToCString(CString(_T("GetEnvironmentVariable(")) + envName, GetLastError(), errorInfo);
            delete[] buffer;
            return false;
        }
        result = CString(buffer);
        delete[] buffer;
        return true;
    }

    //Unused
    bool ExploreObject(LPCTSTR path, CString &result, CString &errorInfo)
    {
        if (GetEnvString(_T("windir"), result, errorInfo)) {
            CString exe = result + CString(_T("\\explorer.exe"));//Absolute path of explorer.exe
            CString arg;
            arg.Format((_T("/select,\"%s\"")), path);
            ShellExecute(NULL, _T("open"), exe, arg, NULL, SW_SHOWNORMAL);
            return true;
        }
        return false;
    }
}
