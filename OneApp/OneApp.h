
// OneApp.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "CCommandParse.h"
// OneApp:
// See OneApp.cpp for the implementation of this class
//

class OneApp : public CWinApp
{
public:
	OneApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
private:
	CCommandParse m_cmdParse;
	BOOL InitApplicationMode();
	BOOL InitServiceEntryPoint();
	BOOL InitServiceMode();
};

extern OneApp theApp;
