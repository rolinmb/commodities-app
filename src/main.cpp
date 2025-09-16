#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include "util.h"

// Global handles
HWND hComboBox = nullptr;
HWND hLabel = nullptr;
HWND hImageView = nullptr;
HFONT hFont = nullptr;
HBITMAP hCurrentBmp = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hLabel = CreateWindow(
            TEXT("STATIC"), TEXT("Please select an option"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            20, 20, 300, 30,
            hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

        // Big font
        hFont = CreateFont(
            24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, TEXT("Segoe UI"));
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

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

        hImageView = CreateWindowExA(0, "STATIC", NULL,
            WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZEIMAGE,
            300, 240, 350, 200, hwnd, NULL, GetModuleHandle(NULL), NULL
        );
        break;
    }
    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 1) {
            int sel = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
            if (sel != CB_ERR) {
                TCHAR text[256];
                SendMessage(hComboBox, CB_GETLBTEXT, sel, (LPARAM)text);
                SetWindowText(hLabel, text);
                MessageBox(hwnd, text, TEXT("Fetching Data for:"), MB_OK | MB_ICONINFORMATION);

                std::string apiKey;
                std::ifstream keyFile("src/key.txt");
                if (keyFile.is_open()) {
                    std::getline(keyFile, apiKey);
                    keyFile.close();
                } else {
                    MessageBox(hwnd, TEXT("Could not read API key!"), TEXT("Error"), MB_OK | MB_ICONERROR);
                    return 0;
                }

                std::string path;
                std::string asset;
                switch(sel) {
                    case 0: path = "/query?function=WTI&interval=monthly&apikey="; asset="wti"; break;
                    case 1: path = "/query?function=BRENT&interval=monthly&apikey=";  asset="brent"; break;
                    case 2: path = "/query?function=NATURAL_GAS&interval=monthly&apikey=";  asset="natural_gas"; break;
                    case 3: path = "/query?function=COPPER&interval=monthly&apikey=";  asset="copper"; break;
                    case 4: path = "/query?function=ALUMINUM&interval=monthly&apikey=";  asset="aluminum"; break;
                    case 5: path = "/query?function=WHEAT&interval=monthly&apikey=";  asset="wheat"; break;
                    case 6: path = "/query?function=CORN&interval=monthly&apikey=";  asset="corn"; break;
                    case 7: path = "/query?function=SUGAR&interval=monthly&apikey=";  asset="sugar"; break;
                    case 8: path = "/query?function=COFFEE&interval=monthly&apikey=";  asset="coffee"; break;
                    case 9: path = "/query?function=ALL_COMMODITIES&interval=monthly&apikey="; asset="all_commodities";  break;
                    default: path = "/query?function=ALL_COMMODITIES&interval=monthly&apikey="; asset="all_commodities";
                }
                std::string fullPath = "https://www.alphavantage.co" + path + apiKey;
                std::string command = "python scripts/main.py \"" + fullPath;

                int result = system(command.c_str());

                if (result == 0) {
                    LoadAndShowBMP(hwnd, asset);
                } else {
                    MessageBox(hwnd, TEXT("Failed to run Python script to generate bitmap chart image."),
                        TEXT("Error"), MB_OK | MB_ICONERROR);
                }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const TCHAR szClassName[] = TEXT("commodities-app");
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szClassName;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(szClassName, TEXT("commodities-app"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        700, 700, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
