// OneApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "OneApp.h"
#include "CCommandParse.h"
#include "App.h"
#include "OneService.h"
#include <string>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

App theApp;

std::string token("test-key"), name("OpcUaProxy"), mHost("localhost");
int mPort{ 1403 }, sPort{ 1101 };
uint32_t security{ 0 }, type{ 0 };

void StartService()
{
    /*
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
    */
	OneService* pService = new OneService(token, name, mPort, mHost, sPort, security, type);

	OneService::StartServer(pService);

	//OneService* pService = new OneService("abcdefgh", "OpcUaProxy", 1403, "localhost", 1103, 0, 0);
	//OneService::StartServer(pService);
}

int _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, int nCmdShow)
{
    int nRetCode = 0;
    CString param(lpCmdLine);

	if (param.Find(L"service") > -1)
	{
		OneService* pService = new OneService("", "", 1234, "", 1101, 0, 1);
		OneService::StartServer(pService);
	}

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
            
            if (param.Find(L"app") > -1)
            {
                theApp.InitApplication();

                theApp.InitInstance();

                theApp.Run();

                theApp.ExitInstance();
            }
            
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

	if (param.Find(L"service") > -1)
	{
        OneService* pService = new OneService("abcdefgh", "OpcUaProxy", 1403, "localhost", 1103, 0, 0);
		OneService::StartServer(pService);
	}
    return nRetCode;
}
