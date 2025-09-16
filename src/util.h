#pragma once
#include <string>

extern HWND hComboBox;
extern HWND hLabel;
extern HFONT hFont;

std::string GetHttps(const std::string& host, const std::string& path);
