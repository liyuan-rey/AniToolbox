#include "StdAfx.h"
#include ".\childsheet.h"

//----------------------------------------------------------------------------
HWND CChildSheet::CreateAsChild(const HWND hWndParent)
{
	ATLASSERT(NULL != hWndParent);
	ATLASSERT(NULL == m_hWnd);

	m_psh.dwFlags	    = PSH_NOAPPLYNOW | PSH_MODELESS | PSH_USECALLBACK;
	m_psh.hwndParent	= hWndParent;
	m_psh.phpage		= (HPROPSHEETPAGE*)m_arrPages.GetData();
	m_psh.nPages		= m_arrPages.GetSize();
	m_psh.pfnCallback	= CChildSheet::PropSheetCallback;

#if (_ATL_VER >= 0x0700)
	ATL::_AtlWinModule.AddCreateWndData(&m_thunk.cd, this);
#else //!(_ATL_VER >= 0x0700)
	_Module.AddCreateWndData(&m_thunk.cd, this);
#endif //!(_ATL_VER >= 0x0700)

	HWND hWnd = (HWND)::PropertySheet(&m_psh);
	_CleanUpPages();	// ensure clean-up, required if call failed

	ATLASSERT(m_hWnd == hWnd);

	return hWnd;
}
//----------------------------------------------------------------------------
int CALLBACK CChildSheet::PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam)
{
	int nRetVal = 0;

	if (uMsg == PSCB_INITIALIZED)
	{		
		ATLASSERT(hWnd != NULL);

#if (_ATL_VER >= 0x0700)
		CChildSheet* pT = (CChildSheet*)ATL::_AtlWinModule.ExtractCreateWndData();
#else //!(_ATL_VER >= 0x0700)
		CChildSheet* pT = (CChildSheet*)_Module.ExtractCreateWndData();
#endif //!(_ATL_VER >= 0x0700)

		pT->SubclassWindow(hWnd);	// subclass the sheet window		
		pT->_CleanUpPages();		// remove page handles array

		// hide OK/Cancel button
		pT->GetDlgItem(IDOK).ShowWindow (SW_HIDE);
		pT->GetDlgItem(IDCANCEL).ShowWindow (SW_HIDE);
	}
	else if (uMsg == PSCB_PRECREATE)
	{
		LPDLGTEMPLATE pTemplate = (LPDLGTEMPLATE)lParam;
		ATLASSERT(pTemplate);

		DWORD dwRemove	= WS_POPUP|WS_SYSMENU|WS_CAPTION|DS_MODALFRAME;
		DWORD dwAdd		= WS_CHILD|WS_VISIBLE|WS_TABSTOP|DS_CONTEXTHELP|DS_3DLOOK;//|DS_CONTROL;

		pTemplate->style			 = (pTemplate->style & ~dwRemove) | dwAdd;
		pTemplate->dwExtendedStyle	|= WS_EX_CONTROLPARENT;

		nRetVal = 1;
	}

	return nRetVal;
}	
