#pragma once
#include <string>
#include <unordered_map>

class TokenPool
{
public:
	TokenPool();
	~TokenPool();

	std::string InvokeToken();
	bool ActiveToken(const std::string& token);
	bool RevokeToken(const std::string& token);
private:
	std::unordered_map<std::string, time_t> _invokeTokens;
	std::vector<std::string> _activeTokens;
	std::vector<std::string> _revokeTokens;
};

