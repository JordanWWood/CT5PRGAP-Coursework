#pragma once

// Creation and managment of the m_window
class Window {
	
public:
	Window(LONG p_WindowWidth, LONG p_WindowHeight, LPCSTR p_WindowName, HINSTANCE *p_HInstance, int *p_cmdShow);
	virtual ~Window();

	int InitWindow();

	HWND GetWindowHandle() const { return m_WindowHandle; }
	LONG GetHeight() const { return m_WindowHeight; }
	LONG GetWidth() const { return m_WindowWidth; }

private:
	HINSTANCE *m_hInstance;
	int *m_cmdShow;

	LONG m_WindowWidth;
	LONG m_WindowHeight;
	LPCSTR m_WindowClassName = "DirectXWindowClass";
	LPCSTR m_WindowName;

	HWND m_WindowHandle = nullptr;;
};

