#include "pch.h"
#include "ServiceEntryPoint.h"
#include "CoreServiceImpl.h"
#include "ServicesManager.h"
#include <grpcpp/grpcpp.h>

ServiceEntryPoint::ServiceEntryPoint()
{

}

ServiceEntryPoint::~ServiceEntryPoint()
{
	if (isRunning)
	{
		Stop();
	}
}

bool ServiceEntryPoint::Start(int nPort)
{
	if (isRunning)
	{
		return false;
	}else {
		isRunning = true;
	}

	if (!m_pBuilder)
	{
		m_pBuilder.reset(new grpc::ServerBuilder);
	}

	if (!m_pServiceImpl)
	{
		m_pServiceImpl.reset(new CoreServiceImpl(GetServiceManager()));
	}

	std::string str_uri("0.0.0.0:");
	str_uri += std::to_string(nPort);
	m_pBuilder->AddListeningPort(str_uri, grpc::InsecureServerCredentials());
	m_pBuilder->RegisterService(m_pServiceImpl.get());

	m_pServer = std::move(m_pBuilder->BuildAndStart());
	if (m_pServer)
	{
		std::thread th(&grpc::Server::Wait, m_pServer.get());
		th.detach();

		return true;
	}
	else
	{
		isRunning = false;
		return false;
	}

	return false;
}

void ServiceEntryPoint::Stop()
{
	m_pServer->Shutdown();
}
