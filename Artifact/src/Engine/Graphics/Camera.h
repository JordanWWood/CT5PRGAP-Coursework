#pragma once
#include "Shaders.h"
#include "Window.h"

class Camera {
public:
	Camera(DirectX::XMFLOAT3 p_EyePosition, DirectX::XMFLOAT3 p_FocusPoint, DirectX::XMFLOAT3 p_UpDirection, DirectX::XMFLOAT2 p_WindowSize);
	~Camera();

	void Update(Shaders* p_Shaders, Window* p_Window) const;

	void Move(float x, float y, float z) { Move({ x, y, z }); }
	void Move(DirectX::XMFLOAT3 vec);
	void Rotate(float degrees, DirectX::XMFLOAT3 axis);
private:
	DirectX::XMFLOAT2 m_WindowSize;

	DirectX::XMFLOAT3 m_EyePosition;
	DirectX::XMFLOAT3 m_FocusPoint;
	DirectX::XMFLOAT3 m_UpDirection;
};
