#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <string>
#include <iostream>
#include "util.h"

#pragma comment(lib, "winhttp.lib")

void SetupComboBox(HWND hwnd) {
    hComboBox = CreateWindow(
        TEXT("COMBOBOX"), TEXT(""),
        CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
        20, 60, 200, 200,
        hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

    const TCHAR* items[] = {
        TEXT("Crude Oil"), TEXT("Brent Oil"), TEXT("Natural Gas"),
        TEXT("Copper"), TEXT("Aluminum"), TEXT("Wheat"),
        TEXT("Corn"), TEXT("Sugar"), TEXT("Coffee"),
        TEXT("Global Commodities Index")
    };
    for (auto item : items)
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)item);
}

void LoadAndShowBMP(HWND hwndParent, const std::string& asset) {
    if (hCurrentBmp) {
        DeleteObject(hCurrentBmp);
        hCurrentBmp = NULL;
    }

    std::string bmpPath = "img/" + asset + ".bmp";
    hCurrentBmp = (HBITMAP)LoadImageA(
        NULL,
        bmpPath.c_str(),
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION
    );

    SendMessage(hImageView, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCurrentBmp);
}
