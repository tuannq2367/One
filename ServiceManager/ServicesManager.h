#pragma once
#include <memory>
#include <unordered_map>

typedef struct CoreServiceInfo CoreServiceInfo;

class TokenPool;

class ServicesManager;
class ServiceObserver;

class ServicesManager
{
	static ServicesManager* _instance;
	ServicesManager();
	~ServicesManager();
public:
	bool StartService(std::string& servName, int nPort = 1101);
	bool ActiveService(std::string& token, const CoreServiceInfo& info);
	bool DeactiveService(const std::string& token);
	bool KeepAliveService(const std::string& token, const std::string& status, uint64_t seq);
	std::string InvokeToken();

	void AttachObserver(ServiceObserver* pObs);
	void DetachObserver(ServiceObserver* pObs);

	static bool CheckServiceTimeout(const CoreServiceInfo& info);
	static ServicesManager* Instance();
	static void Create();
	static void Destroy();
private:
	void NotifyServiceAdd(const CoreServiceInfo& info);
	void NotifyServiceRemove(const CoreServiceInfo& info);
	void NotifyServiceChange(const CoreServiceInfo& info);
	std::unique_ptr<TokenPool> m_pTokensPool;
	std::unordered_map<std::string, CoreServiceInfo> m_activeServices;
	std::vector<ServiceObserver*> m_listObs;
};

inline ServicesManager* GetServiceManager()
{
	return ServicesManager::Instance();
}
