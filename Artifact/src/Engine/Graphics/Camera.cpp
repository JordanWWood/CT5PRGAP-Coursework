#include "ArtifactPCH.h"
#include "Camera.h"

Camera::Camera(DirectX::XMVECTOR pEyePosition, DirectX::XMVECTOR pFocusPoint, DirectX::XMVECTOR pUpDirection, DirectX::XMFLOAT2 pWindowSize) 
	: m_WindowSize(pWindowSize), m_EyePosition(pEyePosition), m_FocusPoint(pFocusPoint), m_UpDirection(pUpDirection) {
}
Camera::~Camera() {}

void Camera::Move(DirectX::XMFLOAT3 vec) {
	m_EyePosition = DirectX::XMVectorAdd(m_EyePosition, {vec.x, vec.y, vec.z});
}

void Camera::Rotate(DirectX::XMFLOAT3 vec) {
	m_FocusPoint = DirectX::XMVectorAdd(m_FocusPoint, { vec.x, vec.y, vec.z });
}

void Camera::Update(Shaders* p_Shaders, Window* p_Window) const {
//	RECT rect;
//	GetWindowRect(p_Window->GetWindowHandle(), &rect);
//	ClipCursor(&rect);

	p_Shaders->Update(Shaders::CB_Frame, DirectX::XMMatrixLookAtLH(m_EyePosition, m_FocusPoint, m_UpDirection));
}
