#include "pch.h"
#include "ServiceControlImpl.h"

::grpc::Status ServiceControlImpl::Shutdown(::grpc::ServerContext* context, const ::google::protobuf::Int32Value* request, ::google::protobuf::BoolValue* response)
{
	return grpc::Status(grpc::StatusCode::OK, "OK");
}

::grpc::Status ServiceControlImpl::QueryState(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::google::protobuf::Int32Value* response)
{
	return grpc::Status(grpc::StatusCode::OK, "OK");
}
