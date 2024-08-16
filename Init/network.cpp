#define CURL_STATICLIB
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <curl.h>
#include <string>
#include "network.h"
#include <unordered_map>
#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"advapi32.lib")
#include <string.h>
#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <WinDNS.h>
#include <iphlpapi.h> 
#include <stdio.h> 
#include <ws2tcpip.h>
#include <locale>
#include <sstream>

using namespace std;





size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = static_cast<std::string*>(userdata);
    response->append(ptr, size * nmemb);
    return size * nmemb;
}


std::string ReqToUrl(std::string url)
{
    CURL* curl;
    CURLcode res;
    std::string response;
    std::unordered_map<std::string, std::string> proxies;
   
    curl = curl_easy_init();
    if (curl) {
        proxies["http"] = "socks5h://127.0.0.1:9050";
        // proxies["https"] = "socks5h://localhost:9050";

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_PROXY, proxies["http"].c_str());
        curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
           // long http_code;
            //curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
           // std::cout << "HTTP Status Code: " << http_code << std::endl;
           // std::cout << "Response Headers:" << std::endl;
            // Print the response headers
          // std::cout << response << std::endl;
        }
        else {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return response;

}


string website_HTML;
locale local;
void get_Website(string url);
char lineBuffer[200][80] = { ' ' };
char buffer[10000];
char ip_address[16];
int i = 0, bufLen = 0, j = 0, lineCount = 0;
int lineIndex = 0, posIndex = 0;

string getIp()
{
 
    get_Website("api.ipify.org");
    for (size_t i = 0; i < website_HTML.length(); ++i) website_HTML[i] = tolower(website_HTML[i], local);

    istringstream ss(website_HTML);
    string stoken;

    while (getline(ss, stoken, '\n')) {

        strcpy(lineBuffer[lineIndex], stoken.c_str());
        int dot = 0;
        for (int ii = 0; ii < strlen(lineBuffer[lineIndex]); ii++) {

            if (lineBuffer[lineIndex][ii] == '.') dot++;
            if (dot >= 3) {
                dot = 0;
                strcpy(ip_address, lineBuffer[lineIndex]);
            }
        }
        lineIndex++;
    }


    string ip = ip_address;
    
    return ip;

}


void get_Website(string url) {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int lineCount = 0;
    int rowCount = 0;
    struct hostent* host;
    string get_http;


    get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
       
        system("pause");
        
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    host = gethostbyname(url.c_str());

    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        
        system("pause");
      
    }
    send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

            website_HTML += buffer[i];
            i += 1;
        }
    }

    closesocket(Socket);
    WSACleanup();

}


string get_lan_ip()
{
    char ac[80];
    struct in_addr addr;
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        //cerr << "Error " << WSAGetLastError() <<
           // " when getting local host name." << endl;
       
    }
 
    struct hostent* phe = gethostbyname(ac);
    if (phe == 0) {
       // cerr << "Yow! Bad host lookup." << endl;   
    }

    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
  
        
    }

    char* ip = inet_ntoa(addr);
 
    string str_ip(ip);

   return str_ip;
}