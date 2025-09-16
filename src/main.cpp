#include <windows.h>
#include <string>

HWND hComboBox;
HWND hLabel;
HFONT hFont;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hLabel = CreateWindow(
            TEXT("STATIC"), TEXT("Please select an option"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            20, 20, 300, 30,
            hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

        // Create a bigger font for label
        hFont = CreateFont(
            24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, TEXT("Segoe UI"));
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);


        hComboBox = CreateWindow(
            TEXT("COMBOBOX"), TEXT(""),
            CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
            20, 50, 200, 200, // position and size
            hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Crude Oil"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Brent Oil"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Natural Gas"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Copper"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Aluminun"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Wheat"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Corn"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Sugar"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Coffee"));
        SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("Global Commodities Index"));
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 1) {
            int sel = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
            if (sel != CB_ERR) {
                TCHAR text[256];
                SendMessage(hComboBox, CB_GETLBTEXT, sel, (LPARAM)text);
                SetWindowText(hwnd, text);
                SetWindowText(hLabel, text);
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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const TCHAR szClassName[] = TEXT("commodities-app");
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szClassName;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        szClassName, TEXT("commodities-app"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
