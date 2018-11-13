#include "GameInput.h"

GameInput::GameInput(Artefact::Camera* camera) : Input(Input()), m_mainCamera(camera) {}

GameInput::~GameInput() {}

void GameInput::OnKeyPressed(const Artefact::KeyEventArgs& e) {
	const int speed = 100;
	switch(e.Key) {
		case 'S': m_mainCamera->Move(0, 0, -(speed * m_deltatime)); break;
		case 'W': m_mainCamera->Move(0, 0, speed * m_deltatime); break;
		case 'A': m_mainCamera->Move(-(speed * m_deltatime), 0, 0); break;
		case 'D': m_mainCamera->Move(speed * m_deltatime, 0, 0); break;
		case VK_ESCAPE: break;
	}
}
void GameInput::OnKeyReleased(const Artefact::KeyEventArgs& e) {}

void GameInput::OnMouseMoved(const Artefact::MouseMotionEventArgs& e) {
	Input::OnMouseMoved(e);

	if (m_mainCamera == nullptr) return;
	const float moveSpeed = 0.1f;

	DirectX::XMINT2 currentMousePosition = DirectX::XMINT2(e.X, e.Y);
	DirectX::XMINT2 mouseDelta = m_PreviousMousePosition - currentMousePosition;
	m_PreviousMousePosition = currentMousePosition;

	if (e.LeftButton) {
		//TODO rotate camera
		m_mainCamera->Rotate(mouseDelta.y * moveSpeed, {-1, 0, 0});
		m_mainCamera->Rotate(mouseDelta.x * moveSpeed, {0, 1, 0});
	}
}

void GameInput::OnMouseButtonPressed(const Artefact::MouseButtonEventArgs& e) {}
void GameInput::OnMouseButtonReleased(const Artefact::MouseButtonEventArgs& e) {}
void GameInput::OnMouseWheel(const Artefact::MouseWheelEventArgs& e) {}
void GameInput::OnResize(const Artefact::ResizeEventArgs& e) {}
