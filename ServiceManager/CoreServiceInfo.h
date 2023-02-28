#pragma once
#include <string>

typedef struct CoreServiceInfo
{
	std::string token;
	std::string serviceName;
	std::string serviceStatus;
	uint32_t bindPort{1101};
	std::string address;
	uint32_t type{0};
	std::string description;
	std::string license;
	uint32_t apiVersion{1};
	std::string apiInfo;
	time_t timestamp{0};
	uint64_t seq{0};
} CoreServiceInfo;

typedef struct ServiceParam
{
	std::string serviceName;
	std::string token;
	int managerPort;
	int servicePort;
	std::string managerAddress;
	int security;
	uint32_t serviceType;

	std::string MakeParam()
	{
		std::string param("service");
		param += " -name=";
		param += serviceName;
		param += " -token=";
		param += token;
		param += " -mPort=";
		param += std::to_string(managerPort);
		param += " -mHost=";
		param += managerAddress;
		param += " -sPort=";
		param += std::to_string(servicePort);
		param += " -sec=";
		param += std::to_string(security);
		param += " -type=";
		param += std::to_string(serviceType);

		return param;
	}
};
