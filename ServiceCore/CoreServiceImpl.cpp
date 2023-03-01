#include "pch.h"
#include "CoreServiceImpl.h"
#include "ServicesManager.h"
#include "CoreServiceInfo.h"
CoreServiceImpl::CoreServiceImpl(ServicesManager* pServiceManager):
	m_pServiceManager(pServiceManager)
{

}

::grpc::Status CoreServiceImpl::RegisterService(::grpc::ServerContext* context, const ::One::ServiceInfo* request, ::google::protobuf::BoolValue* response)
{
	assert(context);
	auto metadata = context->client_metadata();
	auto it = metadata.find("token");
	if (it == metadata.end())
	{
		response->set_value(false);
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Register Fail, Not found token");
	}
	std::string token(it->second.data(), it->second.length());
	
	// Active Service
	CoreServiceInfo info;
	info.token = token;
	info.timestamp = time(0);

	info.address = request->url();
	info.serviceName = request->servicename();
	info.bindPort = request->bindport();
	info.seq = 0;
	info.type = request->type();
	info.apiVersion = request->apiversion();
	info.description = request->description();

	bool ret = m_pServiceManager->ActiveService(token, info);
	response->set_value(ret);

	if (ret)
	{
		return grpc::Status(grpc::StatusCode::OK, "OK");
	}
	else
	{
		return grpc::Status(grpc::StatusCode::ABORTED, "Register Fail");
	}
}

::grpc::Status CoreServiceImpl::KeepAliveService(::grpc::ServerContext* context, const ::One::KeepAlive* request, ::google::protobuf::BoolValue* response)
{
	assert(context);
	auto metadata = context->client_metadata();
	auto it = metadata.find("token");
	if (it == metadata.end())
	{	
		response->set_value(false);
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "KeepAlive Fail, not found token");
	}

	std::string token(it->second.data(), it->second.length());
	std::string status = request->status();
	uint64_t seq = request->seq();
	
	// Update KeepAlive Service
	int ret = m_pServiceManager->KeepAliveService(token, status, seq);
	response->set_value(ret);

	if (ret)
	{
		return grpc::Status(grpc::StatusCode::OK, "OK");
	}
	else
	{
		return grpc::Status(grpc::StatusCode::ABORTED, "KeepAlive Fail");
	}
}
