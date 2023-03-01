#include "pch.h"
#include "TokenPool.h"

#include <time.h>

#pragma comment(lib, "rpcrt4.lib") 

#define MAX_TIME_DELAY_INVOKE 60000

TokenPool::TokenPool()
{

}

TokenPool::~TokenPool()
{
	_activeTokens.clear();
	_invokeTokens.clear();
	_revokeTokens.clear();
}

std::string TokenPool::InvokeToken()
{
	UUID uuid;
	UuidCreate(&uuid);
	char* str;
	UuidToStringA(&uuid, (RPC_CSTR*)&str);

	std::string token(str);
	time_t now;
	localtime(&now);

	_invokeTokens.emplace(token, now);
	return token;
}

bool TokenPool::ActiveToken(const std::string& token)
{
	auto it = _invokeTokens.find(token);
	if (it == _invokeTokens.end())
		return false;

	time_t now = time(0);

	time_t old = it->second;
	_invokeTokens.erase(it);
	if (difftime(now, old) > MAX_TIME_DELAY_INVOKE) // out date
	{
		_revokeTokens.push_back(token);
		return false;
	}
	else
	{
		_activeTokens.push_back(token);
	}

	return true;
}

bool TokenPool::RevokeToken(const std::string& token)
{
	for (auto it = _activeTokens.begin(); it != _activeTokens.end(); it++)
	{
		if (token.compare(*it) == 0)
		{
			_activeTokens.erase(it);
			_revokeTokens.push_back(token);
			return true;
		}
	}

	return false;
}
