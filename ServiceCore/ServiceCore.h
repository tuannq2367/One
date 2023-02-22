// ServiceCore.h : main header file for the ServiceCore DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CServiceCoreApp
// See ServiceCore.cpp for the implementation of this class
//

class CServiceCoreApp : public CWinApp
{
public:
	CServiceCoreApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
