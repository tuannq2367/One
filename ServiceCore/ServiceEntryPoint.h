#pragma once
#include <memory>
#include <atomic>

namespace grpc
{
	class ServerBuilder;
	class Server;
}
class CoreServiceImpl;

class ServiceEntryPoint
{
public:
	ServiceEntryPoint();
	~ServiceEntryPoint();

	bool Start(int nPort);
	void Stop();
private:
	std::atomic_bool isRunning{ false };
	std::unique_ptr<CoreServiceImpl> m_pServiceImpl;
	std::unique_ptr <grpc::Server> m_pServer;
	std::unique_ptr <grpc::ServerBuilder> m_pBuilder;
};

