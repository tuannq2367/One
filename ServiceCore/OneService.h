#pragma once
#include <memory>

namespace grpc
{
	class ServerBuilder;
	class Service;
	class Server;
}

class OneService
{
public:
	OneService(const CString& tokenKey);
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
	std::unique_ptr <grpc::Server> m_pServer;
	std::unique_ptr <grpc::ServerBuilder> m_pBuilder;
	friend class ServiceManager;
	virtual void Run();
};

