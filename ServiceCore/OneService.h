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
class ServicesManager;

class ServiceCore_Class OneService
{
	friend class ServiceManager;
public:
	OneService(const std::string& token, const std::string& serviceName, int mPort, const std::string& mHost, int sPort, uint32_t security, uint32_t type);
	~OneService();

	virtual BOOL InitInstance();
	virtual BOOL ExitInitInstance();
	void InstallService(grpc::Service* pService);
	void Stop();

	const std::string& GetServiceName() const { return m_strServiceName; };
	const std::string& GetDescriptions() const { return m_strDescriptions; };
	const std::string& GetManagerHost() const { return m_mHost; };
	virtual const std::string& QueryStatus() const;
	virtual int GetApiVersion();
	virtual const std::string& GetApiDescription() const;
	static void StartService(OneService*& pService);
protected:
	BOOL ValidateService();
	BOOL InitCommunication();
	virtual void Run();
	std::string m_strStatus{ "OK" };
	std::string m_strDescriptions{ "This is descriptions for service" };
private:
	const std::string m_strToken;
	const std::string m_strServiceName;
	const std::string m_mHost;
	int m_mPort;
	int m_sPort;
	uint32_t m_security;
	uint32_t m_type;
	uint64_t m_nSequenceNumber{0};
	void KeepAlive();
	std::unique_ptr <grpc::Server> m_pServer;
	std::unique_ptr <grpc::ServerBuilder> m_pBuilder;
	std::unique_ptr<ServiceCoreStub> m_pStub;
	std::unique_ptr<ServiceControlImpl> m_pServiceControl;
};

