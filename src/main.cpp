#include "util.h"

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
