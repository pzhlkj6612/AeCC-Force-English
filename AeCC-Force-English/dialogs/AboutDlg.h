#pragma once
#include "../utilities/StringsLoader.h"


// CAboutDlg dialog
class CAboutDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAboutDlg)

public:
    CAboutDlg(CWnd* pParent = nullptr);   // standard constructor
    virtual ~CAboutDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = DLG_11 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    afx_msg void OnNMClickLnk02(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnReturnLnk02(NMHDR *pNMHDR, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()
private:
    CSLoader *sLoader;

    void FireupLnk02();
};
