#include "ArtifactPCH.h"
#include "Window.h"
#include "../../../resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

Window::Window(LONG windowWidth, LONG windowHeight, LPCSTR windowName, HINSTANCE *hInstance, int *cmdShow) {
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;
	m_WindowName = windowName;

	m_hInstance = hInstance;
	m_cmdShow = cmdShow;
}

Window::~Window() {
}

/**
* Initialize the application Window.
*/
int Window::InitWindow() {
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = &WndProc;
	wndClass.hInstance = *m_hInstance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hIcon = LoadIcon(*m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = g_WindowClassName;

	if (!RegisterClassEx(&wndClass)) { return -1; }

	RECT windowRect = { 0, 0, m_WindowWidth, m_WindowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	g_WindowHandle = CreateWindowA(g_WindowClassName, m_WindowName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, *m_hInstance, nullptr);

	if (!g_WindowHandle) { return -1; }

	ShowWindow(g_WindowHandle, *m_cmdShow);
	UpdateWindow(g_WindowHandle);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message) {
	case WM_PAINT: {
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
	}
				   break;
	case WM_DESTROY: { PostQuitMessage(0); }
					 break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}