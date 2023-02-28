#include "pch.h"
#include "ServicesManager.h"
#include "TokenPool.h"
#include "CoreServiceInfo.h"
#include "Observers.h"

#include <codecvt>

#define SERVICE_TIME_OUT 5
#define MANAGER_PORT 1403
std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

ServicesManager::ServicesManager()
{
	m_pTokensPool.reset(new TokenPool());
}

ServicesManager::~ServicesManager()
{

}

bool ServicesManager::StartService(std::string& servName, int nPort)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	
	si.hStdError = nullptr;
	si.hStdOutput = nullptr;
	si.hStdInput = nullptr;
	si.wShowWindow = SW_HIDE;
	//si.dwFlags |= STARTF_USESTDHANDLES;

	ServiceParam param;
	param.serviceName = servName;
	param.managerAddress = "127.0.0.1";
	param.managerPort = MANAGER_PORT;
	param.security = 0;
	param.servicePort = nPort;
	param.serviceType = 1;
	param.token = InvokeToken();
	std::string cmd = param.MakeParam();
	if (!CreateProcess(L"C:\\Workspaces\\One\\Debug64\\OneD.exe", (LPWSTR)convert.from_bytes(cmd).c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		AfxMessageBox(L"Fail to create child-process");
	}
	return true;
}

bool ServicesManager::ActiveService(std::string& token, const CoreServiceInfo& info)
{
	if (!m_pTokensPool->ActiveToken(token))
		return false;

	m_activeServices.emplace(token, info);

	NotifyServiceAdd(info);
	return true;
}

bool ServicesManager::DeactiveService(const std::string& token)
{
	auto it = m_activeServices.find(token);
	if (it == m_activeServices.end())
		return false;
	
	NotifyServiceRemove(it->second);
	m_activeServices.erase(it);
	m_pTokensPool->RevokeToken(token);
	
	return true;
}

bool ServicesManager::KeepAliveService(const std::string& token, const std::string& status, uint64_t seq)
{
	auto it = m_activeServices.find(token);
	if (it == m_activeServices.end())
		return false;

	CoreServiceInfo& info = it->second;
	info.timestamp = time(0);
	info.seq = seq;
	info.serviceStatus = status;

	NotifyServiceChange(info);
	return true;
}

std::string ServicesManager::InvokeToken()
{
	return m_pTokensPool->InvokeToken();
}

void ServicesManager::AttachObserver(ServiceObserver* pObs)
{
	for (auto it = m_listObs.begin(); it != m_listObs.end(); it++)
	{
		if (*it == pObs)
			return;
	}

	m_listObs.emplace_back(pObs);
}

void ServicesManager::DetachObserver(ServiceObserver* pObs)
{
	for (auto it = m_listObs.begin(); it != m_listObs.end(); it++)
	{
		if (*it == pObs)
		{	
			m_listObs.erase(it);
			return;
		}
	}
}

bool ServicesManager::CheckServiceTimeout(const CoreServiceInfo& info)
{
	time_t now;
	localtime(&now);

	if (now - info.timestamp > SERVICE_TIME_OUT)
		return false;

	return true;
}

ServicesManager* ServicesManager::_instance = nullptr;

ServicesManager* ServicesManager::Instance()
{
	return _instance;
}

void ServicesManager::Create()
{
	if (!_instance)
	{
		_instance = new ServicesManager;
	}
}

void ServicesManager::Destroy()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

void ServicesManager::NotifyServiceAdd(const CoreServiceInfo& info)
{
	for (auto it: m_listObs)
	{
		it->OnServiceAdded(info);
	}
}

void ServicesManager::NotifyServiceRemove(const CoreServiceInfo& info)
{
	for (auto it : m_listObs)
	{
		it->OnServiceRemoved(info);
	}
}

void ServicesManager::NotifyServiceChange(const CoreServiceInfo& info)
{
	for (auto it : m_listObs)
	{
		it->OnServiceChanged(info);
	}
}
