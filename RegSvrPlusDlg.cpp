// RegSvrPlusDlg.cpp : CRegSvrPlusDlg 的实现

#include "stdafx.h"
#include "RegSvrPlusDlg.h"
#include ".\regsvrplusdlg.h"

extern CAniToolboxModule _AtlModule;

// CRegSvrPlusDlg
//---------------------------------------------------------------------------
LRESULT CRegSvrPlusDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Create sheet
	HWND hwndPlaceHolder = GetDlgItem (IDC_PLACEHOLDER);

	m_sheet.AddPage (m_pgAbout);
	m_sheet.CreateAsChild (m_hWnd);

	CRect rc;
	::GetWindowRect (hwndPlaceHolder, &rc);
	ScreenToClient (&rc);

	m_sheet.MoveWindow (rc, FALSE);

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	// Init the CDialogResize code
	DlgResize_Init();

	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_AtlModule.m_hInst, MAKEINTRESOURCE(IDI_REGSVRPLUS), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);

	HICON hIconSmall = (HICON)::LoadImage(_AtlModule.m_hInst, MAKEINTRESOURCE(IDI_REGSVRPLUS), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// init file list head
	m_lvwFiles.AddColumn (_T("文件"), 0);
	m_lvwFiles.AddColumn (_T("状态"), 1);
	m_lvwFiles.SetColumnWidth (0, 340);
	m_lvwFiles.SetColumnWidth (1, 86);

	// init file list data
	POSITION pos = m_listFiles.GetHeadPosition ();
	int index = 0;
	while (NULL != pos)
	{
		m_lvwFiles.InsertItem (index++, m_listFiles.GetNext (pos));
	}

	return TRUE;
}
//---------------------------------------------------------------------------
LRESULT CRegSvrPlusDlg::OnPerform(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	MessageBox ("to register component", "TODO");
	return 0;
}
//---------------------------------------------------------------------------
LRESULT CRegSvrPlusDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

BOOL CRegSvrPlusDlg::InitData(const CAtlStringList& listFiles, BOOL bReg/* = TRUE*/)
{
	m_nRegComponent = (bReg == TRUE ? 0 : 1);

	// 过滤不支持的文件
	if (m_config.m_bAutoFilterFiles)
	{
		POSITION pos = listFiles.GetHeadPosition ();
		int index = 0;
		CPath path;

		while (NULL != pos)
		{
			path = (LPCTSTR)listFiles.GetNext (pos);
			path.GetExtension ();
		}

		m_listFiles.AddTailList (&listFiles);
	}

	return TRUE;
}
