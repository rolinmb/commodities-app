#define _WIN32_WINNT 0x0601
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <iostream>
#include "util.h"

#pragma comment(lib, "ws_32.lib")

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

std::string HttpGet(const std::string& host, const std::string& path) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) return "";

    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* result;
    if (getaddrinfo(host.c_str(), "80", &hints, &result) != 0) {
        WSACleanup();
        return "";
    }

    SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET) { freeaddrinfo(result); WSACleanup(); return ""; }
    if (connect(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        closesocket(sock); freeaddrinfo(result); WSACleanup(); return "";
    }
    freeaddrinfo(result);

    std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    send(sock, request.c_str(), (int)request.size(), 0);

    char buffer[4096];
    std::string response;
    int bytes;
    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0)
        response.append(buffer, bytes);

    closesocket(sock);
    WSACleanup();

    size_t pos = response.find("\r\n\r\n");
    if (pos != std::string::npos) return response.substr(pos + 4);
    return response;
}