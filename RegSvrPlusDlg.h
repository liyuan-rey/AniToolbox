// RegSvrPlusDlg.h : CRegSvrPlusDlg µÄÉùÃ÷

#pragma once

#include "resource.h"       // Ö÷·ûºÅ

#include "Config.h"
#include "ChildSheet.h"

// CRegSvrPlusDlg

class CRegSvrPlusDlg : public CDialogImpl<CRegSvrPlusDlg>, 
						public CWinDataExchange<CRegSvrPlusDlg>, 
						public CDialogResize<CRegSvrPlusDlg>
{
public:
	enum { IDD = IDD_REGSVRPLUSDLG };

	// Message Map
	BEGIN_MSG_MAP(CRegSvrPlusDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_BTN_PERFORM, OnPerform)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CDialogResize<CRegSvrPlusDlg>)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPerform(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	// WTL DDX
	BEGIN_DDX_MAP(CRegSvrPlusDlg)
		DDX_CONTROL_HANDLE(IDC_LST_FILES, m_lvwFiles)
		DDX_CHECK(IDC_CHK_VIEWMODE, m_bViewMode)
		DDX_RADIO(IDC_OPT_REGIST, m_nRegComponent)
	END_DDX_MAP()

	CListViewCtrl m_lvwFiles;
	int m_bViewMode;
	int m_nRegComponent;

	// WTL dialog Resizing way
	BEGIN_DLGRESIZE_MAP(CRegSvrPlusDlg)
		DLGRESIZE_CONTROL(IDC_LST_FILES, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_CHK_VIEWMODE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_OPT_REGIST, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_OPT_UNREGIST, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BTN_PERFORM, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_PLACEHOLDER, DLSZ_SIZE_X | DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

public:
	BOOL InitData(const CAtlStringList& listFiles, BOOL bReg = TRUE);

protected:
	CChildSheet m_sheet;
	CPropertyPage<IDD_PROPPAGE_ABOUT> m_pgAbout;

	CAtlStringList m_listFiles;
	CConfig m_config;
};
