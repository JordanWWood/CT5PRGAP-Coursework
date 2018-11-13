#pragma once
#include "Shader.h"
#include "Window.h"

namespace Artefact {
    class Camera {
    public:
        Camera(DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT2);
        ~Camera();

        void Update(Shader*, Window*) const;

        void Move(float x, float y, float z) { Move({ x, y, z }); }
        void Move(DirectX::XMFLOAT3 vec);

        void Rotate(float, DirectX::XMFLOAT3);
    private:
        DirectX::XMFLOAT2 m_WindowSize;

        DirectX::XMFLOAT3 m_EyePosition;
        DirectX::XMFLOAT3 m_FocusPoint;
        DirectX::XMFLOAT3 m_UpDirection;
    };
}