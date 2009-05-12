#pragma once

class CAboutPage : public CPropertyPageImpl<CAboutPage>
{
public:
    enum { IDD = IDD_PROPPAGE_ABOUT };

	CAboutPage(void);
	~CAboutPage(void);
};
