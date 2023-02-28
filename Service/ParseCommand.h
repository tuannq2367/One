#pragma once
#include <string>
#include <vector>

class ParseCommand
{
public:
	ParseCommand() = default;
	~ParseCommand() = default;

	bool CheckParam(const std::string& param);
	bool GetParam(const std::string& param, std::string& value);
	bool Parse(const std::string& input);
private:
	std::vector<std::string> m_params;
	std::vector<std::pair<std::string, std::string>> m_opts;
	bool m_bIsParamWrongFormat{ false };
};

