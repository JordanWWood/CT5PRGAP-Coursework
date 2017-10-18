#pragma once

// Creation and managment of the m_window
class Window {
	
public:
	Window(LONG windowWidth, LONG windowHeight, LPCSTR windowName, HINSTANCE *hInstance, int *cmdShow);
	virtual ~Window();

	int InitWindow();

	HWND GetWindowHandle() const { return g_WindowHandle; }
private:
	HINSTANCE *m_hInstance;
	int *m_cmdShow;

	LONG m_WindowWidth;
	LONG m_WindowHeight;
	LPCSTR g_WindowClassName = "DirectXWindowClass";
	LPCSTR m_WindowName;

	HWND g_WindowHandle = nullptr;;
};

