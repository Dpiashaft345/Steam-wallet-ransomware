#pragma once
#include <Windows.h>

#include <string>

TCHAR* GetUser();
std::string GenerateId(int lenght);
std::string GetCpuInfo();
float GetRamInfo();
const std::string GetSysTime();
std::string Grabber_Info();
std::string WStringToString(const std::wstring& wstr);