#pragma once
#include <grpcpp/grpcpp.h>
#include "proto\ServiceCore.grpc.pb.h"

class ServiceCoreStub
{
public:
	ServiceCoreStub(std::shared_ptr<grpc::Channel> channel);
	~ServiceCoreStub();

	bool RegisterService();
	bool ServiceKeepAlive();
	void InitContext(const std::string& token);
private:
	grpc::ClientContext context;
	std::shared_ptr< ::grpc::ChannelInterface> channel_;
	std::unique_ptr<One::CoreService::Stub> _pStub;
};

