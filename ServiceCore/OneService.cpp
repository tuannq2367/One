#include "pch.h"
#include "OneService.h"
#include "ServiceCoreStub.h"
#include "ServiceControlImpl.h"
#include "OneTimer.h"

#include <grpcpp/grpcpp.h>

OneService::OneService(const std::string& token, const std::string& serviceName, int mPort, const std::string& mHost, int sPort, uint32_t security, uint32_t type):
	m_strToken(token), m_strServiceName(serviceName), m_mPort(mPort), m_mHost(mHost),m_sPort(sPort), m_security(security), m_type(type)
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

	std::string addr = "0.0.0.0:";
	addr += std::to_string(m_sPort);
	m_pBuilder.reset(new grpc::ServerBuilder);
	m_pBuilder->AddListeningPort(addr, grpc::InsecureServerCredentials());
	m_pServiceControl.reset(new ServiceControlImpl);
	m_pBuilder->RegisterService(m_pServiceControl.get());
	return TRUE;
}

void OneService::KeepAlive()
{
	One::KeepAlive keepAlive;
	keepAlive.set_status("OK");
	keepAlive.set_seq(++m_nSequenceNumber);

	if (!m_pStub->ServiceKeepAlive(keepAlive))
		Stop();
}

void OneService::Run()
{
	if (!ValidateService())
	{	
		return;
	}
	std::unique_ptr<OneTimer> keepAliveTimer;
	keepAliveTimer = std::make_unique<OneTimer>(2/*2 second*/, true, [this]()
		{
			KeepAlive();
		});

	keepAliveTimer->Start();

	m_pServer = std::move(m_pBuilder->BuildAndStart());
	if(m_pServer)
	{
		m_pServer->Wait();
	}

	keepAliveTimer->Stop();
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
	if(m_pServer)
	{
		m_pServer->Shutdown();
		m_pServer.reset();
	}
}

void OneService::StartService(OneService*& pService)
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
	std::string target = m_mHost;
	target += ":";
	target += std::to_string(m_mPort);
	m_pStub.reset(new ServiceCoreStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials())));
	m_pStub->InitContext(m_strToken);

	return TRUE;
}

BOOL OneService::ValidateService()
{
	One::ServiceInfo info;
	info.set_servicename(m_strServiceName);
	info.set_bindport(m_sPort);
	info.set_description(m_strDescriptions);
	info.set_url("localhost");
	info.set_apiinfo("This API query data from Server OPCUA");
	info.set_apiversion(1);
	info.set_license("ATS-License 4.0");
	info.set_type(m_type);
	info.set_security(m_security);
	//std::string m_strToken;
	
	uint32_t m_security{0};
	uint32_t m_type{0};
	if (!m_pStub->RegisterService(info))
		return FALSE;

	return TRUE;
}
