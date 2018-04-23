#include "ArtifactPCH.h"

#include "GameParent.h"
#include "Input/Events.h"

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;

LPCSTR g_WindowName = "Game";

Input* mainInput = new Input();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

GameParent::GameParent(BOOL enableVSync, HINSTANCE* hInstance, int* cmdShow, Input* input) : m_Context(Context(
	                                                                               hInstance, cmdShow, g_WindowWidth,
	                                                                               g_WindowHeight, g_WindowName,
	                                                                               enableVSync, &WndProc)), m_Input(input),
                                                                               m_HInstance(hInstance) {}

GameParent::~GameParent() { delete(mainInput); }

int GameParent::Run() {
	MSG msg = {nullptr};

	static DWORD previousTime = timeGetTime();
	static const float targetFramerate = 60.0f;
	static const float maxTimeStep = 1.0f / targetFramerate;

	while (msg.message != WM_QUIT) {
		// Check for windows messages to make sure the X button hasnt been pressed on the application. In the case there isnt one the game will continue to setp the frame
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - previousTime) / 1000.0f;
			previousTime = currentTime;

			// Cap the delta time to the max time step
			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			// Set deltatime in the main input class
			mainInput->SetDeltaTime(deltaTime);
			Update(deltaTime);
			m_Context.Frame();
		}
	}

	return static_cast<int>(msg.wParam);
}

int GameParent::Init() {
	mainInput = m_Input;

	// Make sure the CPU is supported by DirectX before we even attempt to start and if not fail early
	if (!DirectX::XMVerifyCPUSupport()) {
		MessageBox(nullptr, TEXT("Failed to verify GameParent Math library support."), TEXT("Error"), MB_OK);
		return false;
	}

	// Initialise DirectX. Fail early if it doesnt succeed
	if (!m_Context.InitDirectX()) {
		MessageBox(nullptr, TEXT("Could not initialize DirectX."), TEXT("Error"), MB_OK);
		return false;
	}

	return true;
}

/* 
 * Winproc callback. This function is passed through to the window object so it can be contained within the GameParent class
 * This class is also responsible for building and dispatching events to the event driven input system.
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message) {
	case WM_PAINT: {
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
	}
		break;
	case WM_KEYDOWN: {
		MSG charMsg;
		// Get the unicode character (UTF-16)
		unsigned int c = 0;
		// For printable characters, the next message will be WM_CHAR.
		// This message contains the character code we need to send the KeyPressed event.
		// Inspired by the SDL 1.2 implementation.
		if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR) {
			GetMessage(&charMsg, hwnd, 0, 0);
			c = charMsg.wParam;
		}
		bool shift = GetAsyncKeyState(VK_SHIFT) > 0;
		bool control = GetAsyncKeyState(VK_CONTROL) > 0;
		bool alt = GetAsyncKeyState(VK_MENU) > 0;
		auto key = KeyCode::Key(wParam);
		unsigned int scanCode = (lParam & 0x00FF0000) >> 16;
		KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Pressed, shift, control, alt);
		mainInput->OnKeyPressed(keyEventArgs);
	}
		break;
	case WM_KEYUP: {
		bool shift = GetAsyncKeyState(VK_SHIFT) > 0;
		bool control = GetAsyncKeyState(VK_CONTROL) > 0;
		bool alt = GetAsyncKeyState(VK_MENU) > 0;
		auto key = KeyCode::Key(wParam);
		unsigned int c = 0;
		unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

		// Determine which key was released by converting the key code and the scan code
		// to a printable character (if possible).
		// Inspired by the SDL 1.2 implementation.
		unsigned char keyboardState[256];
		GetKeyboardState(keyboardState);
		wchar_t translatedCharacters[4];
		if (int result = ToUnicodeEx(wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0) {
			c = translatedCharacters[0];
		}

		KeyEventArgs keyEventArgs(key, c, KeyEventArgs::Released, shift, control, alt);
		mainInput->OnKeyReleased(keyEventArgs);
	}
		break;
	case WM_MOUSEMOVE: {
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = int(short(LOWORD(lParam)));
		int y = int(short(HIWORD(lParam)));

		MouseMotionEventArgs mouseMotionEventArgs(lButton, mButton, rButton, control, shift, x, y);
		mainInput->OnMouseMoved(mouseMotionEventArgs);
	}
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN: {
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = int(short(LOWORD(lParam)));
		int y = int(short(HIWORD(lParam)));

		MouseButtonEventArgs mouseButtonEventArgs(Input::DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton,
		                                          mButton,
		                                          rButton, control, shift, x, y);
		mainInput->OnMouseButtonPressed(mouseButtonEventArgs);
	}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP: {
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = int(short(LOWORD(lParam)));
		int y = int(short(HIWORD(lParam)));

		MouseButtonEventArgs mouseButtonEventArgs(Input::DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton,
		                                          mButton, rButton, control, shift, x, y);
		mainInput->OnMouseButtonReleased(mouseButtonEventArgs);
	}
		break;
	case WM_MOUSEWHEEL: {
		// The distance the mouse wheel is rotated.
		// A positive value indicates the wheel was rotated to the right.
		// A negative value indicates the wheel was rotated to the left.
		float zDelta = int(short(HIWORD(wParam))) / float(WHEEL_DELTA);
		short keyStates = short(LOWORD(wParam));

		bool lButton = (keyStates & MK_LBUTTON) != 0;
		bool rButton = (keyStates & MK_RBUTTON) != 0;
		bool mButton = (keyStates & MK_MBUTTON) != 0;
		bool shift = (keyStates & MK_SHIFT) != 0;
		bool control = (keyStates & MK_CONTROL) != 0;

		int x = int(short(LOWORD(lParam)));
		int y = int(short(HIWORD(lParam)));

		// Convert the screen coordinates to client coordinates.
		POINT clientToScreenPoint;
		clientToScreenPoint.x = x;
		clientToScreenPoint.y = y;
		ScreenToClient(hwnd, &clientToScreenPoint);

		MouseWheelEventArgs mouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift, int(clientToScreenPoint.x),
		                                        int(clientToScreenPoint.y));
		mainInput->OnMouseWheel(mouseWheelEventArgs);
	}
		break;
	case WM_SIZE: {
		int width = int(short(LOWORD(lParam)));
		int height = int(short(HIWORD(lParam)));

		ResizeEventArgs resizeEventArgs(width, height);
		mainInput->OnResize(resizeEventArgs);
	}
		break;
	case WM_DESTROY: { PostQuitMessage(0); }
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
