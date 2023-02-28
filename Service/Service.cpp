// Service.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <memory>
#include <iostream>
#include "OneService.h"
#include "ParseCommand.h"

#pragma comment(lib,"user32.lib") 

std::string token("test-key"), name("OpcUaProxy"), mHost("localhost");
int mPort{ 1403 }, sPort{ 1101 };
uint32_t security{ 0 }, type{ 0 };

int main(int argc, char* argv[])
{
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
    ParseCommand parse;
    for (int i = 1; i < argc; i++)
    {
        parse.Parse(std::string(argv[i]));
    }
	parse.GetParam("token", token);
	parse.GetParam("name", name);
	parse.GetParam("mHost", mHost);
	std::string tmp;
	parse.GetParam("mPort", tmp);
	mPort = std::stoi(tmp);
	parse.GetParam("sPort", tmp);
	sPort = std::stoi(tmp);
	parse.GetParam("sec", tmp);
	security = std::stoul(tmp);
	parse.GetParam("type", tmp);
	type = std::stoul(tmp);
    OneService* pService = new OneService(token, name, mPort, mHost,sPort , security, type);
    OneService::StartServer(pService);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
