#include "stdafx.h"
#include "StringsLoader.h"


CString CSLoader::m_buffer;

CSLoader::CSLoader(HWND hWnd) { this->m_hWnd = hWnd; }

CString CSLoader::GetString(int stringID) {
    if (!m_buffer.LoadString(stringID)) {
        m_buffer.Empty();
    }
    return CString(m_buffer);
}

void CSLoader::SetAllText(int dialogID) {
    m_buffer = GetString(30000 + dialogID);
    SetWindowText(m_hWnd, m_buffer);

    for (int i = 0; i < 100; i++) {
        if (!m_buffer.LoadString(30000 + 100 * dialogID + i)) { continue; }
        SetDlgItemText(m_hWnd, 100 * dialogID + i, m_buffer);
    }
}

void CSLoader::SetControlText(int controlID, int stringID) {
    m_buffer = GetString(stringID);
    SetDlgItemText(m_hWnd, controlID, m_buffer);
}
void CSLoader::SetControlText(int controlID, CString prefix, int stringID) {
    m_buffer = prefix + GetString(stringID);
    SetDlgItemText(m_hWnd, controlID, m_buffer);
}
void CSLoader::SetControlText(int controlID, int stringID, CString suffix) {
    m_buffer = GetString(stringID) + suffix;
    SetDlgItemText(m_hWnd, controlID, m_buffer);
}
void CSLoader::SetControlText(int controlID, CString prefix, int stringID, CString suffix) {
    m_buffer = prefix + GetString(stringID) + suffix;
    SetDlgItemText(m_hWnd, controlID, m_buffer);
}
