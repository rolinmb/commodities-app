#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <winhttp.h>
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

std::string GetHttps(const std::string& host, const std::string& path) {
    HINTERNET hSession = WinHttpOpen(L"MyApp/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return "";

    std::wstring wHost(host.begin(), host.end());
    std::wstring wPath(path.begin(), path.end());

    HINTERNET hConnect = WinHttpConnect(hSession, wHost.c_str(),
                                        INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return ""; }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wPath.c_str(),
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);
    if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return ""; }

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS,
                            0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        return "";
    }

    std::string response;
    char buffer[4096];
    DWORD bytesRead = 0;
    while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
        response.append(buffer, bytesRead);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return response;
}