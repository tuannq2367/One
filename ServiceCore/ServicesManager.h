#pragma once
#include "ServiceCoreExport.h"
#include <memory>
#include <unordered_map>
#include <mutex>

typedef struct CoreServiceInfo CoreServiceInfo;

class TokenPool;

class ServicesManager;
class ServiceObserver;
class ServiceEntryPoint;
class OneService;

class ServiceCore_Class ServicesManager
{
	static ServicesManager* _instance;
	ServicesManager();
	~ServicesManager();
public:
	static constexpr int ServiceEntryPort = 1403;
	bool StartProgram(const CString& executePath, std::string& servName, int nPort = 1101);
	bool ActiveService(std::string& token, const CoreServiceInfo& info);
	bool DeactiveService(const std::string& token);
	bool KeepAliveService(const std::string& token, const std::string& status, uint64_t seq);
	std::string InvokeToken();

	void StartServiceEntryPoint();
	void StopEntryPoint();
	int GetServiceEntryPort() { return ServiceEntryPort; }
	void AttachObserver(ServiceObserver* pObs);
	void DetachObserver(ServiceObserver* pObs);
	void CheckServicesTimeout();

	static bool IsServiceTimeout(const CoreServiceInfo& info);
	static ServicesManager* Instance();
	static void Create();
	static void Destroy();
private:
	std::mutex m_mutex;
	void NotifyServiceAdd(const CoreServiceInfo& info);
	void NotifyServiceRemove(const CoreServiceInfo& info);
	void NotifyServiceChange(const CoreServiceInfo& info);
	std::unique_ptr<ServiceEntryPoint> m_pServiceEntryPoint;
	std::unique_ptr<TokenPool> m_pTokensPool;
	std::unordered_map<std::string, CoreServiceInfo> m_activeServices;
	std::vector<ServiceObserver*> m_listObs;
};

inline ServicesManager* GetServiceManager()
{
	return ServicesManager::Instance();
}
