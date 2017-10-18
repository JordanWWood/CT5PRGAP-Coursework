#include "ArtifactPCH.h"
#include "Camera.h"

Camera::Camera(DirectX::XMVECTOR p_EyePosition, DirectX::XMVECTOR p_FocusPoint, DirectX::XMVECTOR p_UpDirection) 
	: m_EyePosition(p_EyePosition), m_FocusPoint(p_FocusPoint), m_UpDirection(p_UpDirection) {}
Camera::~Camera() {}

void Camera::Update(Shaders* p_Shaders) const {
	p_Shaders->Update(Shaders::CB_Frame, DirectX::XMMatrixLookAtLH(m_EyePosition, m_FocusPoint, m_UpDirection));
}
