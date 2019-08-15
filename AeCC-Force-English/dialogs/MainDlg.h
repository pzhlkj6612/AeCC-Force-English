
// MainDlg.h : header file
//

#pragma once
#include "../utilities/FileFunc.h"
#include "../utilities/VersionFunc.h"
#include "../utilities/StringsLoader.h"


#define WM_CHECKEDVERSION (WM_USER + 1000)


UINT GetVersionInfo(LPVOID lpParam);


// CMainDlg dialog
class CMainDlg : public CDialogEx
{
    // Construction
public:
    CMainDlg(CWnd* pParent = nullptr);	// standard constructor
    virtual ~CMainDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_10 };
#endif

    // Implementation
protected:
    HICON m_hIcon;

    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    // Generated message map functions
    virtual BOOL OnInitDialog();

    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnCheckedversion(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedBtn04();
    afx_msg void OnNMClickLnk05(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnReturnLnk05(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickLnk06(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnReturnLnk06(NMHDR *pNMHDR, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()
private:
    CString m_versionErrorInfo;
    VersionFunc::VERSION m_versionResultEnum;
    VersionFunc::FourIntegerVersionNumber *myVersionNumber;
public:
    VersionFunc::FourIntegerVersionNumber *onlineVersionNumber;
private:
    //Tool
    CSLoader *sLoader;

    //State
    bool m_gotFilePath;
    bool m_gotVersionInfo;

    //DDX
    CString m_DDX_txtFilePath;
    CString m_DDX_ForceEnglishState;
    CString m_DDX_ButtonState;
    CString m_DDX_versionCheckState;

    //Thread
    CWinThread* getVersionInfoThread;
private:
    void FireupLnk05();
    void FireupLnk06();
};
