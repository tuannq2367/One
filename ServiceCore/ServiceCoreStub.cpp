#include "pch.h"
#include "ServiceCoreStub.h"

ServiceCoreStub::ServiceCoreStub(std::shared_ptr<grpc::Channel> channel):
	_pStub(One::CoreService::NewStub(channel))
{
	channel_ = channel;
}

ServiceCoreStub::~ServiceCoreStub()
{

}

bool ServiceCoreStub::RegisterService()
{
	grpc_connectivity_state st = channel_->GetState(true);
	One::ServiceInfo servieInfo;
	google::protobuf::BoolValue ret;
	_pStub->RegisterService(&context, servieInfo, &ret);

	return true;
}

bool ServiceCoreStub::ServiceKeepAlive()
{
	grpc_connectivity_state st = channel_->GetState(true);
	context.AddMetadata("token", "abcd");
	One::KeepAlive keepAlive;
	
	google::protobuf::BoolValue ret;
	_pStub->ServiceKeepAlive(&context, keepAlive, &ret);

	return true;
}

void ServiceCoreStub::InitContext(const std::string& token)
{
	context.AddMetadata("token", token);
}
