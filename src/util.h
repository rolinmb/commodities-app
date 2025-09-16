#pragma once
#include <windows.h>
#include <string>
#include <iostream>

// Globals
extern HWND hComboBox;
extern HWND hLabel;
extern HFONT hFont;

std::string WideToUTF8(const std::wstring& wstr);

void SetupComboBox(HWND hwnd);
