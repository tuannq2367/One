#include "pch.h"
#include "OneService.h"
#include <grpcpp/grpcpp.h>

OneService::OneService(const CString& tokenKey)
{

}

OneService::~OneService()
{

}

void OneService::InstallService(grpc::Service* pService)
{
	assert(m_pBuilder);
	m_pBuilder->RegisterService(pService);
}

BOOL OneService::InitInstance()
{
	if (!InitCommunication())
		return FALSE;

	m_pBuilder.reset(new grpc::ServerBuilder);
	m_pBuilder->AddListeningPort(std::string("0.0.0.0:1403"), grpc::InsecureServerCredentials());
	return TRUE;
}

void OneService::Run()
{
	if (!ValidateService())
		return;

	m_pServer = std::move(m_pBuilder->BuildAndStart());

	m_pServer->Wait();
}

BOOL OneService::ExitInitInstance()
{
	if (m_pServer)
	{
		Stop();
	}
	
	return TRUE;
}

void OneService::Stop()
{
	assert(m_pServer);
	m_pServer->Shutdown();
	m_pServer.reset();
}

void OneService::StartServer(OneService*& pService)
{
	if (!pService)
		return;

	if (!pService->InitInstance())
	{
		pService->ExitInitInstance();
		return;
	}

	pService->Run();

	pService->ExitInitInstance();

	delete pService;
	pService = nullptr;
}

BOOL OneService::InitCommunication()
{
	return TRUE;
}

BOOL OneService::ValidateService()
{
	return TRUE;
}
