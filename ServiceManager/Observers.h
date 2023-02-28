#pragma once

typedef struct CoreServiceInfo CoreServiceInfo;

class ServiceObserver
{
public:
	ServiceObserver() = default;
	~ServiceObserver() = default;

	virtual void OnServiceAdded(const CoreServiceInfo& info) {};
	virtual void OnServiceRemoved(const CoreServiceInfo& info) {};
	virtual void OnServiceChanged(const CoreServiceInfo& info) {};
};
