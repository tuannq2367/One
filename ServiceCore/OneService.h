#pragma once
#include "ServiceCoreExport.h"
#include <memory>
#include <string>

namespace grpc
{
	class ServerBuilder;
	class Service;
	class Server;
}

class ServiceCoreStub;
class ServiceControlImpl;

class ServiceCore_Class OneService
{
public:
	OneService(const std::string& token, const std::string& serviceName, int mPort, const std::string& mHost, int sPort, uint32_t security, uint32_t type);
	~OneService();

	void InstallService(grpc::Service* pService);

	virtual BOOL InitInstance();
	
	virtual BOOL ExitInitInstance();
	void Stop();

	static void StartServer(OneService*& pService);
protected:
	BOOL ValidateService();
	BOOL InitCommunication();
private:
	const std::string m_strToken;
	const std::string m_strServiceName;
	std::string m_strDescriptions{"This is descriptions for service"};
	int m_mPort;
	const std::string m_mHost;
	int m_sPort;
	uint32_t m_security;
	uint32_t m_type;
	void KeepAlive();
	std::unique_ptr <grpc::Server> m_pServer;
	std::unique_ptr <grpc::ServerBuilder> m_pBuilder;
	std::unique_ptr<ServiceCoreStub> m_pStub;
	std::unique_ptr<ServiceControlImpl> m_pServiceControl;
	friend class ServiceManager;
	virtual void Run();
};

