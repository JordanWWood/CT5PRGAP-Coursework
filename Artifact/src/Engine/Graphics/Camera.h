#pragma once
#include <DirectXColors.h>
#include "Shaders.h"
#include "Window.h"

class Camera {
public:
	Camera(DirectX::XMVECTOR p_EyePosition, DirectX::XMVECTOR p_FocusPoint, DirectX::XMVECTOR p_UpDirection, DirectX::XMFLOAT2 p_WindowSize);
	~Camera();

	void Move(float x, float y, float z) { Move({ x, y, z }); }
	void Move(DirectX::XMFLOAT3 vec);
	void Rotate(float x, float y, float z) { Rotate({ x, y, z }); }
	void Rotate(DirectX::XMFLOAT3 vec);

	void Update(Shaders* p_Shaders, Window* p_Window) const;
private:
	DirectX::XMFLOAT2 m_WindowSize;

	DirectX::XMVECTOR m_EyePosition;
	DirectX::XMVECTOR m_FocusPoint;
	DirectX::XMVECTOR m_UpDirection;
};
