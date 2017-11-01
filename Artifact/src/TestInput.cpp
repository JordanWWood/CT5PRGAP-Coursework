#include "ArtifactPCH.h"
#include "TestInput.h"

TestInput::TestInput(Camera* camera) : base(Input()), m_mainCamera(camera) {}

TestInput::~TestInput() {}

void TestInput::OnKeyPressed(const KeyEventArgs& e) {}
void TestInput::OnKeyReleased(const KeyEventArgs& e) {}
void TestInput::OnMouseMoved(const MouseMotionEventArgs& e) {
	base::OnMouseMoved(e);

	if (m_mainCamera == nullptr) return;
	std::cout << "Mouse Moved: Input Test" << std::endl;

	const float moveSpeed = 0.1f;

	DirectX::XMINT2 currentMousePosition = DirectX::XMINT2(e.X, e.Y);
	DirectX::XMINT2 mouseDelta = m_PreviousMousePosition - currentMousePosition;
	m_PreviousMousePosition = currentMousePosition;

	if (e.LeftButton) {
		//TODO rotate camera
		m_mainCamera->Rotate(mouseDelta.y * moveSpeed, {0, 1, 0});
		//m_mainCamera->Rotate(mouseDelta.x * moveSpeed, {0, 0, 0});
	}
}

void TestInput::OnMouseButtonPressed(const MouseButtonEventArgs& e) {}
void TestInput::OnMouseButtonReleased(const MouseButtonEventArgs& e) {}
void TestInput::OnMouseWheel(const MouseWheelEventArgs& e) {}
void TestInput::OnResize(const ResizeEventArgs& e) {}
