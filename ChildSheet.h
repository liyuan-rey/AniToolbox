#pragma once

class CChildSheet : public CPropertySheetImpl<CChildSheet>
{
public:
	HWND CreateAsChild(const HWND hWndParent);	
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);

protected:
	CRect m_rc;
};
