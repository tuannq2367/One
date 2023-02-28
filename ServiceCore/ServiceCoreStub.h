#pragma once
#include <grpcpp/grpcpp.h>
#include "proto\ServiceCore.grpc.pb.h"

class ServiceCoreStub
{
public:
	ServiceCoreStub(std::shared_ptr<grpc::Channel> channel);
	~ServiceCoreStub();

	bool RegisterService(const One::ServiceInfo& servieInfo);
	bool ServiceKeepAlive(const One::KeepAlive& keepAlive);
	void InitContext(const std::string& token);
private:
	//grpc::ClientContext context;
	std::string _token;
	std::unique_ptr<One::CoreService::Stub> _pStub;
};

