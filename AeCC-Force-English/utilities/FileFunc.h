#pragma once


namespace FileFunc {
    enum TARGET_FILE_PROCESS_RESULT {
        DELETE_SUCCESS,
        CREATE_SUCCESS,
        OPERATE_FAILURE
    };

    bool GetTheFilePath(CString &result, CString &errorInfo);
    bool TargetExisted(LPCTSTR path);
    TARGET_FILE_PROCESS_RESULT processTarget(LPCTSTR path, CString &errorInfo);
    bool GetUserSpecialPath(REFKNOWNFOLDERID rfid, CString &result, CString &errorInfo);
    bool GetEnvString(LPCTSTR envName, CString &result, CString &errorInfo);
    bool ExploreObject(LPCTSTR path, CString &result, CString &errorInfo);
}
