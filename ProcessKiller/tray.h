#pragma once
#include "DevAttribute\devattribute.h";
#include <thread>
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY Tray();
bool statusCheck();