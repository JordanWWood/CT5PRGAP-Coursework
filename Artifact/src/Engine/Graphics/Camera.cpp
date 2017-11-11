#include "ArtifactPCH.h"
#include "Camera.h"
#include "Util/DirectXMathConverter.h"

Camera::Camera(DirectX::XMFLOAT3 pEyePosition, DirectX::XMFLOAT3 pFocusPoint, DirectX::XMFLOAT3 pUpDirection,
               DirectX::XMFLOAT2 pWindowSize)
	: m_WindowSize(pWindowSize), m_EyePosition(pEyePosition), m_FocusPoint(pFocusPoint), m_UpDirection(GMathVF(GMathFV(pEyePosition) + GMathFV(pUpDirection))) {}

Camera::~Camera() {}

void Camera::Move(DirectX::XMFLOAT3 direction) {
	m_EyePosition = GMathVF(XMVector3Transform(GMathFV(m_EyePosition),
		XMMatrixTranslation(direction.x, direction.y, direction.z)));
	m_FocusPoint = GMathVF(XMVector3Transform(GMathFV(m_FocusPoint),
		XMMatrixTranslation(direction.x, direction.y, direction.z)));
//	m_UpDirection = GMathVF(XMVector3Transform(GMathFV(m_UpDirection),
//		XMMatrixTranslation(direction.x, direction.y, direction.z)));
}

void Camera::Rotate(float degrees, XMFLOAT3 axis) {	
	XMFLOAT3 look_at_target = GMathVF(GMathFV(m_FocusPoint) - GMathFV(m_EyePosition));
	XMFLOAT3 look_at_up = GMathVF(GMathFV(m_UpDirection) - GMathFV(m_EyePosition));
	look_at_target = GMathVF(XMVector3Transform(GMathFV(look_at_target),
		XMMatrixRotationAxis(GMathFV(axis), XMConvertToRadians(degrees))));
	look_at_up = GMathVF(XMVector3Transform(GMathFV(look_at_up),
		XMMatrixRotationAxis(GMathFV(axis), XMConvertToRadians(degrees))));
	m_FocusPoint = GMathVF(GMathFV(m_EyePosition) + GMathFV(look_at_target));
	m_UpDirection = GMathVF(GMathFV(m_EyePosition) + GMathFV(look_at_up));
}

void Camera::Update(Shaders* p_Shaders, Window* p_Window) const {
	p_Shaders->Update(Shaders::CB_Frame, DirectX::XMMatrixLookAtLH(XMLoadFloat3(&m_EyePosition), XMLoadFloat3(&m_FocusPoint), XMLoadFloat3(&m_UpDirection)),);
}
