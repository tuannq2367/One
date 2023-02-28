
// One.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "One.h"
#include "OneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "OneService.h"

// OneApp

BEGIN_MESSAGE_MAP(OneApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

static OneApp theApp;

// OneApp construction

OneApp::OneApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only OneApp object


// OneApp initialization

BOOL OneApp::InitInstance()
{
	ParseCommandLine(m_cmdParse);
	if (m_cmdParse.CheckParam("app"))
	{
		return InitMain();
	}
	else if (m_cmdParse.CheckParam("service"))
	{
		return InitService();
	}
}

int OneApp::Run()
{
	std::string token, name, mHost;
	int mPort, sPort;
	uint32_t security, type;
	m_cmdParse.GetParam("token", token);
	m_cmdParse.GetParam("name", name);
	m_cmdParse.GetParam("mHost", mHost);
	std::string tmp;
	m_cmdParse.GetParam("mPort", tmp);
	mPort = std::stoi(tmp);
	m_cmdParse.GetParam("sPort", tmp);
	sPort = std::stoi(tmp);
	m_cmdParse.GetParam("sec", tmp);
	security = std::stoul(tmp);
	m_cmdParse.GetParam("type", tmp);
	type = std::stoul(tmp);
	OneService* pService = new OneService(token, name, mPort, mHost, sPort, security, type);

	OneService::StartServer(pService);
	return 0;
}

BOOL OneApp::InitService()
{
	CWinApp::InitInstance();

	Run();
	
	return FALSE;
}

BOOL OneApp::InitMain()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager* pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	OneDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

