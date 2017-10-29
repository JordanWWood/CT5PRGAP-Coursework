#pragma once
#include "Window.h"
#include "Shaders.h"
#include "Camera.h"
#include <vector>

class Mesh;

class Context
{
public:
	Context();
	Context(HINSTANCE *p_HInstance, int *p_cmdShow, LONG p_WindowWidth, LONG p_WindowHeight, LPCSTR p_WindowName, BOOL p_vSync, WNDPROC wndproc);
	~Context();

	int InitDirectX();

	Window* GetWindow() const { return m_window; }

	void Clear(const FLOAT p_ClearColor[4], FLOAT p_ClearDepth, UINT8 p_ClearStencil);
	void Present();
	void Frame();

	Mesh* CreateMesh(const std::vector<Shaders::VertexPosColor> vertex, const std::vector<WORD> indices, const DirectX::XMFLOAT3 pPosition, const DirectX::XMFLOAT3 pRotation, const DirectX::XMFLOAT3 pScale);
	void RenderMesh(Mesh* mesh);
	void DeleteMesh(Mesh* mesh);

	Shaders* GetShaders() const { return m_shaders; }
	Camera* GetCamera() const { return m_camera; }
private:
	Window* m_window = nullptr;
	Shaders* m_shaders = nullptr;
	Camera *m_camera = nullptr;

	BOOL m_EnableVSync;

	// Direct3D device and swap chain.
	ID3D11Device* m_d3dDevice = nullptr;
	ID3D11DeviceContext* m_d3dDeviceContext = nullptr;
	IDXGISwapChain* m_d3dSwapChain = nullptr;

	// Render target view for the back buffer of the swap chain.
	ID3D11RenderTargetView* m_d3dRenderTargetView = nullptr;
	// Depth/stencil view for use as a depth buffer.
	ID3D11DepthStencilView* m_d3dDepthStencilView = nullptr;
	// A texture to associate to the depth stencil view.
	ID3D11Texture2D* m_d3dDepthStencilBuffer = nullptr;

	// Define the functionality of the depth/stencil stages.
	ID3D11DepthStencilState* m_d3dDepthStencilState = nullptr;
	// Define the functionality of the rasterizer stage.
	ID3D11RasterizerState* m_d3dRasterizerState = nullptr;
	D3D11_VIEWPORT m_Viewport = { 0 };

	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;

	std::vector<Mesh*> m_AllMesh;

	static DXGI_RATIONAL QueryRefreshRate(UINT p_ScreenWidth, UINT m_ScreenHeight, BOOL p_vsync);
};
