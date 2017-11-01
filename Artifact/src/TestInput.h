#pragma once
#include "Engine/Input/Input.h"
#include "Engine/Graphics/Camera.h"

class TestInput : public Input
{
public:
	typedef Input base;

	// Findout about passable methods or identify a better way to implement input
	TestInput(Camera* camera = nullptr);
	~TestInput();

	void OnKeyPressed(const KeyEventArgs& keyEventArgs);
	void OnKeyReleased(const KeyEventArgs& keyEventArgs);
	void OnMouseMoved(const MouseMotionEventArgs& mouseMotionEventArgs);
	void OnMouseButtonPressed(const MouseButtonEventArgs& mouseButtonEventArgs);
	void OnMouseButtonReleased(const MouseButtonEventArgs& mouseButtonEventArgs);
	void OnMouseWheel(const MouseWheelEventArgs& mouseWheelEventArgs);
	void OnResize(const ResizeEventArgs& resizeEventArgs);

	void SetCamera(Camera* camera) { m_mainCamera = camera; }
private:
	DirectX::XMINT2 m_PreviousMousePosition;
	Camera* m_mainCamera;
};

inline DirectX::XMINT2 operator-(const DirectX::XMINT2& x0, const DirectX::XMINT2& x1)
{
	return DirectX::XMINT2(x0.x - x1.x, x0.y - x1.y);
}

