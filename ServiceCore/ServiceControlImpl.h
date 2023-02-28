#pragma once
#include "proto/ServiceCore.grpc.pb.h"

class ServiceControlImpl: public One::ServiceControl::Service
{
public:
	ServiceControlImpl() = default;
	~ServiceControlImpl() = default;

	virtual ::grpc::Status Shutdown(::grpc::ServerContext* context, const ::google::protobuf::Int32Value* request, ::google::protobuf::BoolValue* response);
	virtual ::grpc::Status QueryState(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::google::protobuf::Int32Value* response);
};

