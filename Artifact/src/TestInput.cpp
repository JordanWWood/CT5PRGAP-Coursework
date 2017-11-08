#include "ArtifactPCH.h"
#include "TestInput.h"

TestInput::TestInput(Camera* camera) : Input(Input()), m_mainCamera(camera) {}

TestInput::~TestInput() {}

void TestInput::OnKeyPressed(const KeyEventArgs& e) {
	std::cout << e.Key << std::endl;

	const int speed = 10;
	switch(e.Key) {
		case 'S': m_mainCamera->Move(0, 0, -(speed * m_deltatime)); break;
		case 'W': m_mainCamera->Move(0, 0, speed * m_deltatime); break;
		case 'A': m_mainCamera->Move(-(speed * m_deltatime), 0, 0); break;
		case 'D': m_mainCamera->Move(speed * m_deltatime, 0, 0); break;
		case VK_ESCAPE: break;
	}
}
void TestInput::OnKeyReleased(const KeyEventArgs& e) {}

void TestInput::OnMouseMoved(const MouseMotionEventArgs& e) {
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

void TestInput::OnMouseButtonPressed(const MouseButtonEventArgs& e) {}
void TestInput::OnMouseButtonReleased(const MouseButtonEventArgs& e) {}
void TestInput::OnMouseWheel(const MouseWheelEventArgs& e) {}
void TestInput::OnResize(const ResizeEventArgs& e) {}
