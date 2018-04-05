#pragma once
#include "../Engine/Input/Input.h"
#include "../Engine/Graphics/Camera.h"

class GameInput : public Input {
public:
	// Findout about passable methods or identify a better way to implement input
	GameInput(Camera* = nullptr);
	~GameInput();

	void OnKeyPressed(const KeyEventArgs&) override;
	void OnKeyReleased(const KeyEventArgs&) override;
	void OnMouseMoved(const MouseMotionEventArgs&) override;
	void OnMouseButtonPressed(const MouseButtonEventArgs&) override;
	void OnMouseButtonReleased(const MouseButtonEventArgs&) override;
	void OnMouseWheel(const MouseWheelEventArgs&) override;
	void OnResize(const ResizeEventArgs&) override;

	void SetCamera(Camera* camera) { m_mainCamera = camera; }
private:
	DirectX::XMINT2 m_PreviousMousePosition;
	Camera* m_mainCamera;
};

inline DirectX::XMINT2 operator-(const DirectX::XMINT2& x0, const DirectX::XMINT2& x1) {
	return DirectX::XMINT2(x0.x - x1.x, x0.y - x1.y);
}

