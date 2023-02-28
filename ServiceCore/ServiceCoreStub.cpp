#include "pch.h"
#include "ServiceCoreStub.h"

ServiceCoreStub::ServiceCoreStub(std::shared_ptr<grpc::Channel> channel):
	_pStub(One::CoreService::NewStub(channel))
{
}

ServiceCoreStub::~ServiceCoreStub()
{

}

bool ServiceCoreStub::RegisterService(const One::ServiceInfo& servieInfo)
{
	grpc::ClientContext context;
	context.AddMetadata("token", _token);
	google::protobuf::BoolValue ret;
	auto status = _pStub->RegisterService(&context, servieInfo, &ret);
	if (status.ok() && ret.value())
		return true;

	return false;
}

bool ServiceCoreStub::ServiceKeepAlive(const One::KeepAlive& keepAlive)
{
	grpc::ClientContext context;
	context.AddMetadata("token", _token);
	google::protobuf::BoolValue ret;
	auto status = _pStub->KeepAliveService(&context, keepAlive, &ret);
	if (status.ok() && ret.value())
		return true;
	
	return false;
}

void ServiceCoreStub::InitContext(const std::string& token)
{
	_token = token;
}
