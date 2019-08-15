#pragma once
#include "../resource.h"


class CSLoader {
public:
    CSLoader(HWND hWnd);

public:
    static CString GetString(int stringID);

private:
    static CString m_buffer;

public:
    void SetAllText(int dialogID);
    void SetControlText(int controlID, int stringID);
    void SetControlText(int controlID, CString prefix, int stringID);
    void SetControlText(int controlID, int stringID, CString suffix);
    void SetControlText(int controlID, CString prefix, int stringID, CString suffix);

private:
    HWND m_hWnd;
};
