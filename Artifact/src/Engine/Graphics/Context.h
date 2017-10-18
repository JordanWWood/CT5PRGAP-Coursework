#pragma once
#include "Window.h"
#include "Shaders.h"
#include <vector>

class Mesh;

class Context
{
public:
	Context(HINSTANCE *hInstance, int *cmdShow, LONG windowWidth, LONG windowHeight, LPCSTR windowName, BOOL vSync);
	~Context();

	int InitDirectX();

	Window* GetWindow() const { return m_window; }

	void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
	void Present();

	void RenderMesh(Mesh* mesh);
	Mesh* CreateMesh(std::vector<Shaders::VertexPosColor> vertex, std::vector<WORD> indices) const;

	Shaders* GetShaders() const { return m_shaders; }
private:
	Window* m_window = nullptr;
	Shaders* m_shaders = nullptr;

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

	static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
};
