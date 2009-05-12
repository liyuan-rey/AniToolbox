// Config.h : CConfig µÄÉùÃ÷

#pragma once

class CConfig
{
public:
	CConfig(void);
	virtual ~CConfig(void);

public:
	BOOL m_bUseDllInstall;
	CString m_strDllInstallArgs;
	BOOL m_bCallDllRegisterServer;
	BOOL m_bAlwaysOnTop;
	BOOL m_bStartWithCompact;
	BOOL m_bAutoShowError;
	BOOL m_bAutoFilterFiles;
};
