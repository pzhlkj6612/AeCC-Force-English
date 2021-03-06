
// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../AeCC-Force-English.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "../utilities/URLs.h"


// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(DLG_11, pParent)
{
    sLoader = nullptr;
}

CAboutDlg::~CAboutDlg()
{
    delete sLoader;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


// C26454
// https://stackoverflow.com/questions/51139422/warning-c26454-arithmetic-overflow-operation-produces-a-negative-unsigned
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
    ON_NOTIFY(NM_CLICK, DLG_11_LNK_02, &CAboutDlg::OnNMClickLnk02)
    ON_NOTIFY(NM_RETURN, DLG_11_LNK_02, &CAboutDlg::OnReturnLnk02)
END_MESSAGE_MAP()

// CAboutDlg message handlers


BOOL CAboutDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    sLoader = new CSLoader(GetSafeHwnd());
    sLoader->SetAllText(DLG_11);

    GetDlgItem(DLG_11_GRP_00)->SetFocus();//Help Link Control to avoid getting focus

    return FALSE;// TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnNMClickLnk02(NMHDR *pNMHDR, LRESULT *pResult)
{
    FireupLnk02();
    *pResult = 0;
}

void CAboutDlg::OnReturnLnk02(NMHDR *pNMHDR, LRESULT *pResult)
{
    FireupLnk02();
    *pResult = 0;
}


void CAboutDlg::FireupLnk02() {
    ShellExecute(NULL, _T("open"), urls::Repo_Page, NULL, NULL, SW_SHOWNORMAL);
}
