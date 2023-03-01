#pragma once
#include "ServiceCoreExport.h"

typedef struct CoreServiceInfo CoreServiceInfo;

class ServiceCore_Class ServiceObserver
{
public:
	ServiceObserver() = default;
	~ServiceObserver() = default;

	virtual void OnServiceAdded(const CoreServiceInfo& info) {};
	virtual void OnServiceRemoved(const CoreServiceInfo& info) {};
	virtual void OnServiceChanged(const CoreServiceInfo& info) {};
};
