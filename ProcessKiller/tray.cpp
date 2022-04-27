#include "tray.h"
#include "resource.h"

bool status = 0;
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case SC_MINIMIZE: return 0;
	case WM_USER:
		if (lParam == WM_LBUTTONDBLCLK && status == 1) {
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			status = 0;
			break;
		}
		if (lParam == WM_LBUTTONDBLCLK && status == 0) {
			ShowWindow(GetConsoleWindow(), SW_HIDE);
			status = 1;
			break;
		}
		break;
	default:
		return DefWindowProc(window, message, wParam, lParam);
	}
	return 0;
}
bool statusCheck() {
	return status;
}


int APIENTRY Tray() {
	HINSTANCE instance{};
	NOTIFYICONDATA icon{};
	WNDCLASSEX main{};
	// Класс окна
	main.cbSize = sizeof(WNDCLASSEX);
	main.hInstance = instance;
	main.lpszClassName = TEXT("ProcessKiller");
	main.lpfnWndProc = WndProc;
	RegisterClassEx(&main);
	HWND window = CreateWindowEx(0, TEXT("ProcessKiller"),NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

	// Создание значка
	icon.cbSize = sizeof(icon);
	icon.hWnd = window;
	icon.uCallbackMessage = WM_USER;
	icon.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	icon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	strcpy(icon.szTip, "ProcessKiller");
	Shell_NotifyIcon(NIM_ADD, &icon);
	MSG message;
	while (GetMessage(&message,NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	Shell_NotifyIcon(NIM_DELETE, &icon);
	return 0;
}