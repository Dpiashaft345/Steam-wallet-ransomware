#define CURL_STATICLIB
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <curl.h>
#include <Windows.h>
#include "network.h"
#include <unordered_map>
#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"advapi32.lib")

using namespace std;

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = static_cast<std::string*>(userdata);
    response->append(ptr, size * nmemb);
    return size * nmemb;
}


string CheckCon(string url)
{
    CURL* curl;
    CURLcode res{};
    std::string response;
    std::unordered_map<std::string, std::string> proxies;

    curl = curl_easy_init();
    if (curl) {
        proxies["http"] = "socks5h://127.0.0.1:9050";
        // proxies["https"] = "socks5h://localhost:9050";

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_PROXY, proxies["http"].c_str());
       // curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000);
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
            //curl_easy_strerror(res);
            //MessageBox(0, "Fail to connect!", "Fail to connect!", MB_ICONERROR | MB_TOPMOST);
        }

        curl_easy_cleanup(curl);
    }

    return curl_easy_strerror(res);


}


std::string getPayStatus(std::string url)
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
            //curl_easy_strerror(res);
           // MessageBox(0, "Fail to connect!", "Fail to connect!", MB_ICONERROR | MB_TOPMOST);
        }

        curl_easy_cleanup(curl);
    }

    return response;

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
           //curl_easy_strerror(res);
            MessageBox(0, "Fail to connect!", "Fail to connect!", MB_TOPMOST|MB_ICONERROR);
        }

        curl_easy_cleanup(curl);
    }

    return response;

}


