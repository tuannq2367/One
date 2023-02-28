#pragma once
#include <afxwin.h>
#include <vector>
#include <string>

class CCommandParse :
    public CCommandLineInfo
{
public:
	CCommandParse(void) = default;
	virtual ~CCommandParse(void) = default;
	//Sample 03: Get functions for 
	//params and switches

	BOOL CheckParam(const std::string& param);
	BOOL GetParam(const std::string& param, std::string& value);
private:
	//Sample 01: Private Members
	std::vector<std::string> m_params;
	std::vector<std::pair<std::string, std::string>> m_opts;
	//Sample 02: Override for Base class virtual
	BOOL m_bIsParamWrongFormat{ FALSE };
	void ParseParam(const TCHAR* pszParam,
		BOOL bFlag, BOOL bLast);
	void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);
};

