#pragma once

namespace tools {
    void HResultToCString(CString source, HRESULT hResult, CString &result);
    void DWORDErrorCodeToCString(CString source, DWORD errorcode, CString &result);//Can be better?
    void ErrnotToCString(CString source, errno_t error, CString &result);//Unused
    int MultiByteToCStringUTF8(LPCCH lpMultiByteStr, CString &result);//Unused

    //void HResultToCString(std::wstring source, HRESULT hResult, std::wstring &result);
    //void DWORDErrorCodeToCString(std::wstring source, DWORD errorcode, std::wstring &result);//Can be better?
    //void ErrnotToCString(std::wstring source, errno_t error, std::wstring &result);//Unused
    //int MultiByteToCStringUTF8(const char *lpMultiByteStr, std::wstring &result);//Unused
}
