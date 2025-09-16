#pragma once

extern HWND hComboBox;
extern HWND hLabel;
extern HWND hImageView;
extern HFONT hFont;
extern HBITMAP hCurrentBmp;

void SetupComboBox(HWND hwnd);

void LoadAndShowBMP(HWND hwndParent, const std::string& asset);
