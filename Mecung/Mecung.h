
// Mecung.h : main header file for the Mecung application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMecungApp:
// See Mecung.cpp for the implementation of this class
//

class CMecungApp : public CWinApp
{
public:
	CMecungApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMecungApp theApp;
