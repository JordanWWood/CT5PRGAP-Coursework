#include "ArtifactPCH.h"

#include "Window.h"
#include "resource.h"
#include "../GameParent.h"

namespace Artefact {
    Window::Window(LONG p_WindowWidth, LONG p_WindowHeight, LPCSTR p_WindowName, HINSTANCE *p_HInstance, int *p_CmdShow) {
        m_WindowWidth = p_WindowWidth;
        m_WindowHeight = p_WindowHeight;
        m_WindowName = p_WindowName;

        m_hInstance = p_HInstance;
        m_cmdShow = p_CmdShow;
    }

    Window::~Window() {
        delete(m_hInstance);
        delete(m_cmdShow);

        m_hInstance = NULL;
        m_cmdShow = NULL;
    }

    /**
    * Initialize the application Window.
    */
    int Window::InitWindow(WNDPROC WndProc) {
        WNDCLASSEX wndClass = { 0 };
        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_HREDRAW | CS_VREDRAW;
        wndClass.lpfnWndProc = WndProc;
        wndClass.hInstance = *m_hInstance;
        wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wndClass.hIcon = LoadIcon(*m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
        wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wndClass.lpszMenuName = nullptr;
        wndClass.lpszClassName = m_WindowClassName;

        if (!RegisterClassEx(&wndClass)) { return -1; }

        RECT windowRect = { 0, 0, m_WindowWidth, m_WindowHeight };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        m_WindowHandle = CreateWindowA(m_WindowClassName, m_WindowName,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr, nullptr, *m_hInstance, nullptr);

        if (!m_WindowHandle) { return -1; }

        ShowWindow(m_WindowHandle, *m_cmdShow);
        UpdateWindow(m_WindowHandle);

        SetForegroundWindow(m_WindowHandle);
        SetFocus(m_WindowHandle);

        //ShowCursor(false);
        return 0;
    }
}