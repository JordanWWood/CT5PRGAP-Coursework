#pragma once
#include <DirectXColors.h>
#include "Shaders.h"

class Camera {
public:
	Camera(DirectX::XMVECTOR p_EyePosition, DirectX::XMVECTOR p_FocusPoint, DirectX::XMVECTOR p_UpDirection);
	~Camera();

	void Update(Shaders* p_Shaders) const;
private:
	DirectX::XMVECTOR m_EyePosition;
	DirectX::XMVECTOR m_FocusPoint;
	DirectX::XMVECTOR m_UpDirection;
};
