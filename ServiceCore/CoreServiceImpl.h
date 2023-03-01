#pragma once
#include "proto/ServiceCore.grpc.pb.h"

class ServicesManager;

class CoreServiceImpl: public One::CoreService::Service
{
public:
	CoreServiceImpl(ServicesManager* pServiceManager);
	~CoreServiceImpl() = default;
	virtual ::grpc::Status RegisterService(::grpc::ServerContext* context, const ::One::ServiceInfo* request, ::google::protobuf::BoolValue* response);
	virtual ::grpc::Status KeepAliveService(::grpc::ServerContext* context, const ::One::KeepAlive* request, ::google::protobuf::BoolValue* response);
private:
	ServicesManager* m_pServiceManager;
};

