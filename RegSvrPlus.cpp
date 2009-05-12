// RegSvrPlus.cpp : CRegSvrPlus µÄÊµÏÖ

#include "stdafx.h"
#include "RegSvrPlus.h"

#include "RegSvrPlusDlg.h"

extern CAniToolboxModule _AtlModule;

// CRegSvrPlus
//---------------------------------------------------------------------------
CRegSvrPlus::CRegSvrPlus ()
{
	m_hRegBmp = LoadBitmap ( _AtlModule.m_hInst, MAKEINTRESOURCE(IDB_REGISTER) );
	m_hUnregBmp = LoadBitmap ( _AtlModule.m_hInst, MAKEINTRESOURCE(IDB_UNREGISTER) );
}
//---------------------------------------------------------------------------
HRESULT CRegSvrPlus::Initialize ( 
	LPCITEMIDLIST pidlFolder,
	LPDATAOBJECT pDataObj,
	HKEY hProgID )
{
	// If a data object pointer was passed in, save it and
	// extract the file name. 
	if ( NULL == pDataObj )
	{
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG;
	}

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	// Look for CF_HDROP data in the data object.
	if ( FAILED( pDataObj->GetData ( &fmt, &stg )))
	{
		return E_INVALIDARG;
	}

	// Get a pointer to the actual data.
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// Make sure it worked.
	if ( NULL == hDrop )
	{
		return E_INVALIDARG;
	}

	// Sanity check ¨C make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
	TCHAR szFile[MAX_PATH];

	for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
	{
		// Get the next filename.
		if ( 0 == DragQueryFile ( hDrop, uFile, szFile, MAX_PATH ))
			continue;

		m_listFiles.AddTail ( szFile );
	}

	// Release resources.
	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );

	// If we found any files we can work with, return S_OK.  Otherwise,
	// return E_INVALIDARG so we don't get called again for this right-click
	// operation.

	return ( m_listFiles.GetCount() > 0 ) ? S_OK : E_INVALIDARG;
}
//---------------------------------------------------------------------------
HRESULT CRegSvrPlus::QueryContextMenu ( 
	HMENU hmenu,
	UINT  uMenuIndex,
	UINT  uidFirstCmd,
	UINT  uidLastCmd,
	UINT  uFlags )
{
	UINT uCmdID = uidFirstCmd;

	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if ( uFlags & CMF_DEFAULTONLY )
	{
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	}

	// Add our register/unregister items.
	CString strTmp;
	strTmp.LoadString ( IDS_MITEXT_REGISTER );
	InsertMenu ( hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uCmdID++, strTmp );

	// Set the bitmap for the register item.
	if ( NULL != m_hRegBmp )
	{
		SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_hRegBmp, NULL );
	}

	uMenuIndex++;

	strTmp.LoadString ( IDS_MITEXT_UNREGISTER );
	InsertMenu ( hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uCmdID++, strTmp );

	// Set the bitmap for the unregister item.
	if ( NULL != m_hUnregBmp )
	{
		SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_hUnregBmp, NULL );
	}

	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, uCmdID - uidFirstCmd/*=2*/ );
}
//---------------------------------------------------------------------------
HRESULT CRegSvrPlus::GetCommandString ( 
	UINT  uCmdID,
	UINT  uFlags, 
	UINT* puReserved,
	LPSTR szName,
	UINT  cchMax )
{
	CString strTmp;

	USES_CONVERSION;

	if ( uFlags & GCS_HELPTEXT )
	{
		switch ( uCmdID )
		{
		case 0:
			strTmp.LoadString ( IDS_MIPROMPT_REGISTER );
			break;

		case 1:
			strTmp.LoadString ( IDS_MIPROMPT_UNREGISTER );
			break;

		default:
			ATLASSERT(0);			// should never get here
			return E_INVALIDARG;
			break;
		}

		// Copy the help text into the supplied buffer.  If the shell wants
		// a Unicode string, we need to case szName to an LPCWSTR.

		if ( uFlags & GCS_UNICODE )
		{
			StringCchCopyW ( (LPWSTR) szName, cchMax, T2CW(strTmp) );
		}
		else
		{
			StringCchCopyA ( szName, cchMax, T2CA(strTmp) );
		}
	}
	else if ( uFlags & GCS_VERB )
	{
		// Copy the verb name into the supplied buffer.  If the shell wants
		// a Unicode string, we need to case szName to an LPCWSTR.

		switch ( uCmdID )
		{
		case 0:
			strTmp.LoadString (IDS_MIVERB_REGISTER);
			break;

		case 1:
			strTmp.LoadString (IDS_MIVERB_UNREGISTER);
			break;

		default:
			ATLASSERT(0);			// should never get here
			return E_INVALIDARG;
			break;
		}

		if ( uFlags & GCS_UNICODE )
		{
			StringCchCopyW ( (LPWSTR) szName, cchMax, T2CW(strTmp) );
		}
		else
		{
			StringCchCopyA ( szName, cchMax, T2CA(strTmp) );
		}
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT CRegSvrPlus::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	// If lpVerb really points to a string, ignore this function call and bail out.
	if ( 0 != HIWORD( pCmdInfo->lpVerb ))
		return E_INVALIDARG;

	CRegSvrPlusDlg dlgRegSvr;

	// Check that lpVerb is one of our commands (0 or 1)
	switch ( LOWORD( pCmdInfo->lpVerb ))
	{
	case 0:
		{
			dlgRegSvr.InitData (m_listFiles, TRUE);
		}
		break;
	case 1:
		{
			dlgRegSvr.InitData (m_listFiles, FALSE);
		}
		break;

	default:
		{
			ATLASSERT(FALSE);			// should never get here
			return E_INVALIDARG;
		}
	}

	dlgRegSvr.DoModal (::GetActiveWindow());

	return S_OK;
}
/*
// If this is the first time thru the for loop, we need to
// change the current directory to the one that was open in
// explorer.  This is done so the DLLs we're about to load
// can load other DLLs they depend on to be in the same
// directory.

if ( !bChangedDir )
{
bChangedDir = true;

// Take the current filename and remove the last component.
// I could do this using PathRemoveFileSpec() but that's only
// available if you have IE 3+ installed.

lstrcpy ( szFolder, szFile );

// Find the last backslash and replace it with a zero character.

pszLastBackslash = _tcsrchr ( szFolder, '\\' );
ATLASSERT ( NULL != pszLastBackslash );

*pszLastBackslash = '\0';

// But, if szFolder is now just a drive, like "C:", we need
// to put the backslash back to make it a valid directory
// name.

if ( lstrlen ( szFolder ) == 2 )
lstrcat ( szFolder, _T("\\") );

// Save the current directory, and then change it.

GetCurrentDirectory ( MAX_PATH, szCurrDir );
SetCurrentDirectory ( szFolder );
}   // end if

// Try & load the DLL.

hinst = LoadLibrary ( szFile );

if ( NULL == hinst )
continue;

// Get the address of DllRegisterServer();

(FARPROC&) pfn = GetProcAddress ( hinst, "DllRegisterServer" );

// If it wasn't found, skip the file.

if ( NULL == pfn )
{
FreeLibrary ( hinst );
continue;
}
*/