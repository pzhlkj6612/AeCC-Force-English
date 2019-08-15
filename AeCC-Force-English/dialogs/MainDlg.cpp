
// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../AeCC-Force-English.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "AboutDlg.h"
#include "../utilities/URLs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainDlg dialog



CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(DLG_10, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(ICO_01);

    sLoader = nullptr;

    m_gotFilePath = false;
    m_gotVersionInfo = false;

    m_versionResultEnum = VersionFunc::VERSION::VERSION_NO_NEWER;
    myVersionNumber = new VersionFunc::FourIntegerVersionNumber;
    onlineVersionNumber = new VersionFunc::FourIntegerVersionNumber;

    getVersionInfoThread = nullptr;
}

CMainDlg::~CMainDlg()
{
    delete sLoader;

    delete myVersionNumber;
    delete onlineVersionNumber;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, DLG_10_EDT_02, m_DDX_txtFilePath);
    DDX_Text(pDX, DLG_10_STC_03, m_DDX_ForceEnglishState);
    DDX_Text(pDX, DLG_10_BTN_04, m_DDX_ButtonState);
    DDX_Text(pDX, DLG_10_LNK_05, m_DDX_versionCheckState);
}


// C26454
// https://stackoverflow.com/questions/51139422/warning-c26454-arithmetic-overflow-operation-produces-a-negative-unsigned
BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_CHECKEDVERSION, &CMainDlg::OnCheckedversion)
    ON_BN_CLICKED(DLG_10_BTN_04, &CMainDlg::OnBnClickedBtn04)
    ON_NOTIFY(NM_CLICK, DLG_10_LNK_05, &CMainDlg::OnNMClickLnk05)
    ON_NOTIFY(NM_RETURN, DLG_10_LNK_05, &CMainDlg::OnReturnLnk05)
    ON_NOTIFY(NM_CLICK, DLG_10_LNK_06, &CMainDlg::OnNMClickLnk06)
    ON_NOTIFY(NM_RETURN, DLG_10_LNK_06, &CMainDlg::OnReturnLnk06)
END_MESSAGE_MAP()

// CMainDlg message handlers


BOOL CMainDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    sLoader = new CSLoader(GetSafeHwnd());
    sLoader->SetAllText(DLG_10);

    m_DDX_versionCheckState = sLoader->GetString(IDS_DLG_10_LNK_05);
    getVersionInfoThread = AfxBeginThread(GetVersionInfo, this);

    if (!VersionFunc::GetMyVersion(*myVersionNumber)) {
        ::MessageBox(NULL, myVersionNumber->errorInfo, sLoader->GetString(IDS_ERROR), MB_ICONWARNING);
    }

    CString title;
    title.Format(_T("%s v%d.%d.%d.%d"),
        sLoader->GetString(IDS_DLG_10).GetString(),
        myVersionNumber->Major,
        myVersionNumber->Minor,
        myVersionNumber->Patch,
        myVersionNumber->Build);
    SetWindowText(title);

    CString errorInfo;
    if (FileFunc::GetTheFilePath(m_DDX_txtFilePath, errorInfo)) {
        m_gotFilePath = true;
        if (FileFunc::TargetExisted(m_DDX_txtFilePath)) {
            m_DDX_ForceEnglishState = sLoader->GetString(IDS_FORCELANGSTATUS) + sLoader->GetString(IDS_YES);
        }
        else {
            m_DDX_ForceEnglishState = sLoader->GetString(IDS_FORCELANGSTATUS) + sLoader->GetString(IDS_NO);
        }
        m_DDX_ButtonState = sLoader->GetString(IDS_DLG_10_BTN_04);
    }
    else {
        m_gotFilePath = false;
        m_DDX_ButtonState = sLoader->GetString(IDS_QUIT);
        ::MessageBox(NULL, errorInfo, sLoader->GetString(IDS_ERROR), MB_ICONWARNING);
    }

    UpdateData(FALSE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

afx_msg LRESULT CMainDlg::OnCheckedversion(WPARAM gotVersionInfo, LPARAM lparam)
{
    auto a = *onlineVersionNumber;
    auto b = &onlineVersionNumber;

    if (gotVersionInfo == TRUE) {
        m_gotVersionInfo = true;

        if (*myVersionNumber < *onlineVersionNumber) {
            m_versionResultEnum = VersionFunc::VERSION::VERSION_IS_OLDER;
            m_DDX_versionCheckState.Format(sLoader->GetString(IDS_VERSION_IS_OLDER),
                onlineVersionNumber->Major,
                onlineVersionNumber->Minor,
                onlineVersionNumber->Patch,
                onlineVersionNumber->Build);
        }
        else if (*myVersionNumber > *onlineVersionNumber) {
            m_versionResultEnum = VersionFunc::VERSION::VERSION_IS_BETA;
            m_DDX_versionCheckState = sLoader->GetString(IDS_VERSION_IS_BETA);
        }
        else if (*myVersionNumber == *onlineVersionNumber) {
            m_versionResultEnum = VersionFunc::VERSION::VERSION_NO_NEWER;
            m_DDX_versionCheckState = sLoader->GetString(IDS_VERSION_NO_NEWER);
        }
    }
    else {
        m_gotVersionInfo = false;

        m_versionErrorInfo = onlineVersionNumber->errorInfo;
        m_DDX_versionCheckState = sLoader->GetString(IDS_VERSION_GET_FAILED);
    }

    UpdateData(FALSE);

    return 0;
}

void CMainDlg::OnBnClickedBtn04()
{
    if (!m_gotFilePath) {
        EndDialog(0);
        //PostQuitMessage(1);
        return;
    }

    // Refresh the path
    // 
    // This call is unnecessary because of the results from my test such as
    //   moving the "Documents" folder to another location between program startup and the button clicking.
    // No matter what I did, the program always found the correct path to the txt file.
    // 
    //FileFunc::GetTheFilePath(m_txtFilePath, ...);
    //UpdateData(FALSE);
    // 

    CString errorInfo;
    switch (FileFunc::processTarget(m_DDX_txtFilePath.GetString(), errorInfo)) {
    case(FileFunc::TARGET_FILE_PROCESS_RESULT::CREATE_SUCCESS): {
        m_DDX_ForceEnglishState = sLoader->GetString(IDS_FORCELANGSTATUS) + sLoader->GetString(IDS_YES);
        break;
    }case(FileFunc::TARGET_FILE_PROCESS_RESULT::DELETE_SUCCESS): {
        m_DDX_ForceEnglishState = sLoader->GetString(IDS_FORCELANGSTATUS) + sLoader->GetString(IDS_NO);
        break;
    }case(FileFunc::TARGET_FILE_PROCESS_RESULT::OPERATE_FAILURE): {
        MessageBox(errorInfo, sLoader->GetString(IDS_ERROR), MB_ICONWARNING);
        break;
    }
    }

    UpdateData(FALSE);
}

void CMainDlg::OnReturnLnk05(NMHDR *pNMHDR, LRESULT *pResult)
{
    FireupLnk05();
    *pResult = 0;
}

void CMainDlg::OnNMClickLnk05(NMHDR *pNMHDR, LRESULT *pResult)
{
    FireupLnk05();
    *pResult = 0;
}

void CMainDlg::OnReturnLnk06(NMHDR *pNMHDR, LRESULT *pResult)
{
    FireupLnk06();
    *pResult = 0;
}

void CMainDlg::OnNMClickLnk06(NMHDR *pNMHDR, LRESULT *pResult)
{
    FireupLnk06();
    *pResult = 0;
}

void CMainDlg::FireupLnk05()
{
    if (m_gotVersionInfo) {
        switch (m_versionResultEnum) {
        case VersionFunc::VERSION::VERSION_IS_OLDER: {
            ShellExecute(NULL, _T("open"), urls::Latest_Release_Page, NULL, NULL, SW_SHOWNORMAL);
            break;
        }
        case VersionFunc::VERSION::VERSION_IS_BETA: {
            ShellExecute(NULL, _T("open"), urls::Issues_Page, NULL, NULL, SW_SHOWNORMAL);
            break;
        }
        }
    }
    else {
        MessageBox(m_versionErrorInfo, sLoader->GetString(IDS_WHY), MB_ICONINFORMATION);
    }
}

void CMainDlg::FireupLnk06()
{
    CAboutDlg aboutDlg(FromHandle(GetSafeHwnd()));
    aboutDlg.DoModal();
}


UINT GetVersionInfo(LPVOID lpParam)
{
    CMainDlg* dlg = (CMainDlg*)lpParam;
    auto fivm = dlg->onlineVersionNumber;
    WPARAM wparam = VersionFunc::GetVersionInfo(*fivm) ? TRUE : FALSE;

    PostMessage(
        dlg->m_hWnd,
        WM_CHECKEDVERSION,
        wparam,
        0
    );

    return 0;
}
