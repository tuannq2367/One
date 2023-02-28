#include "ParseCommand.h"

bool ParseKeyValue(const std::string& input, std::string& key, std::string& value)
{
	int equalSignPos = input.find("=");
	if (equalSignPos == -1)
		return false;

	int keyStart = input.find("-");
	if (keyStart == -1)
		return false;

	keyStart += 1;
	key = input.substr(keyStart, equalSignPos - keyStart);
	value = input.substr(equalSignPos + 1);
	return true;
}

bool ParseCommand::CheckParam(const std::string& param)
{
	for (auto it : m_params)
	{
		if (it.compare(param) == 0)
			return true;
	}

	return false;
}

bool ParseCommand::GetParam(const std::string& param, std::string& value)
{
	for (auto it : m_opts)
	{
		if (it.first.compare(param) == 0)
		{
			value = it.second;
			return true;
		}
	}

	return false;
}

bool ParseCommand::Parse(const std::string& input)
{
	int equalSignPos = input.find("=");
	if (equalSignPos == -1)
	{
		m_params.emplace_back(input);
	}
	else
	{
		std::string key, val;
		if (ParseKeyValue(input, key, val))
		{
			m_opts.emplace_back(key, val);
		}
		else
		{
			m_bIsParamWrongFormat = true;
			return false;
		}
	}

	return true;
}
