
// ServiceManager.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <memory>

// ServiceManagerApp:
// See ServiceManager.cpp for the implementation of this class
//

namespace grpc
{
	class ServerBuilder;
	class Server;
}
class CoreServiceImpl;

class ManagerApp : public CWinApp
{
public:
	ManagerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); // return app exit code
// Implementation

	DECLARE_MESSAGE_MAP()
private:
	void InitService();
	std::unique_ptr<CoreServiceImpl> m_pServiceImpl;
	std::unique_ptr <grpc::Server> m_pServer;
	std::unique_ptr <grpc::ServerBuilder> m_pBuilder;
};

extern ManagerApp theApp;
