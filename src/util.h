#pragma once
#include <string>

extern HWND hComboBox;
extern HWND hLabel;
extern HFONT hFont;

std::string HttpGet(const std::string& host, const std::string& path);
