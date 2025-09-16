#include "util.h"

HWND hComboBox = nullptr;
HWND hLabel = nullptr;
HFONT hFont = nullptr;

// helper: convert wide string to UTF-8
std::string WideToUTF8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0,
        wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(),
        &result[0], size_needed, NULL, NULL);
    return result;
}

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