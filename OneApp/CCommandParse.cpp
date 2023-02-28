#include "pch.h"
#include "CCommandParse.h"
#include <Mq.h>
#include <codecvt>

BOOL CCommandParse::CheckParam(const std::string& param)
{
	for (auto it: m_params)
	{
		if (it.compare(param) == 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CCommandParse::GetParam(const std::string& param, std::string& value)
{
	for (auto it:m_opts)
	{
		if (it.first.compare(param) == 0)
		{
			value = it.second;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL ParseKeyValue(const std::string& input, std::string& key, std::string& value)
{
	int equalSignPos = input.find("=");
	if (equalSignPos == -1)
		return FALSE;

	int keyStart = input.find("-");
	if (keyStart == -1)
		return FALSE;

	keyStart += 1;
	key = input.substr(keyStart, equalSignPos - keyStart);
	value = input.substr(equalSignPos + 1);
	return TRUE;
}

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> _convert;

void CCommandParse::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	std::string param = _convert.to_bytes((char16_t*)pszParam);
	return ParseParam(param.c_str(), bFlag, bLast);
}

void CCommandParse::ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
{
	std::string param_or_switch(pszParam);
	if (bFlag)
	{
		std::string key, val;
		if (ParseKeyValue(param_or_switch, key, val))
		{
			m_opts.emplace_back(key, val);
		}
		else
		{
			m_bIsParamWrongFormat = TRUE;
		}
	}
	else
	{
		m_params.emplace_back(param_or_switch);
	}
}
