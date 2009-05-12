// AniToolboxModule.h : CAniToolboxModule µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "AniToolbox.h"

// CAniToolboxModule

class CAniToolboxModule : public CAtlDllModuleT< CAniToolboxModule >
{
public:
	DECLARE_LIBID(LIBID_AniToolboxLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ANITOOLBOX, "{210682DE-8878-4006-813F-DE54C65A2746}")

	HINSTANCE m_hInst;
	CMessageLoop* GetMessageLoop() /*const*/
	{
		return &m_MsgLoop;
	}

protected:
	CMessageLoop m_MsgLoop;
};
