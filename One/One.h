
// One.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "CCommandParse.h"

// OneApp:
// See One.cpp for the implementation of this class
//

class OneApp : public CWinApp
{
public:
	OneApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int Run();
// Implementation

	DECLARE_MESSAGE_MAP()
private:
	BOOL InitService();
	BOOL InitMain();
	CCommandParse m_cmdParse;
};

extern OneApp theApp;
