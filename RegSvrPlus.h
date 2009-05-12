// RegSvrPlus.h : CRegSvrPlus µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "AniToolbox.h"

// CRegSvrPlus

class ATL_NO_VTABLE CRegSvrPlus : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRegSvrPlus, &CLSID_RegSvrPlus>,
	public IDispatchImpl<IRegSvrPlus, &IID_IRegSvrPlus, &LIBID_AniToolboxLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CRegSvrPlus();

DECLARE_REGISTRY_RESOURCEID(IDR_REGSVRPLUS)


BEGIN_COM_MAP(CRegSvrPlus)
	COM_INTERFACE_ENTRY(IRegSvrPlus)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	// IShellExtInit
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	// IContextMenu
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);

protected:
	HBITMAP			m_hRegBmp;
	HBITMAP			m_hUnregBmp;
	CAtlStringList	m_listFiles;
};

OBJECT_ENTRY_AUTO(__uuidof(RegSvrPlus), CRegSvrPlus)
