#pragma once
#include <Artefact.h>

class GameInput : public Artefact::Input {
public:
	GameInput(Artefact::Camera* = nullptr);
	~GameInput();

	void OnKeyPressed(const Artefact::KeyEventArgs&) override;
	void OnKeyReleased(const Artefact::KeyEventArgs&) override;
	void OnMouseMoved(const Artefact::MouseMotionEventArgs&) override;
	void OnMouseButtonPressed(const Artefact::MouseButtonEventArgs&) override;
	void OnMouseButtonReleased(const Artefact::MouseButtonEventArgs&) override;
	void OnMouseWheel(const Artefact::MouseWheelEventArgs&) override;
	void OnResize(const Artefact::ResizeEventArgs&) override;

	void SetCamera(Artefact::Camera* camera) { m_mainCamera = camera; }
private:
	DirectX::XMINT2 m_PreviousMousePosition;
    Artefact::Camera* m_mainCamera;
};

inline DirectX::XMINT2 operator-(const DirectX::XMINT2& x0, const DirectX::XMINT2& x1) {
	return DirectX::XMINT2(x0.x - x1.x, x0.y - x1.y);
}

