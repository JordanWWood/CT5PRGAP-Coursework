#include "ArtifactPCH.h"
#include "Context.h"
#include "Mesh.h"
#include "Camera.h"


Context::Context(HINSTANCE* hInstance, int* cmdShow, const LONG windowWidth, const LONG windowHeight,
                 const LPCSTR windowName, const BOOL vSync, WNDPROC wndproc) {
	m_window = new Window(windowWidth, windowHeight, windowName, hInstance, cmdShow);
	m_window->InitWindow(wndproc);

	m_EnableVSync = vSync;
}

Context::~Context() {
	SafeRelease(m_d3dDepthStencilView);
	SafeRelease(m_d3dRenderTargetView);
	SafeRelease(m_d3dDepthStencilBuffer);
	SafeRelease(m_d3dDepthStencilState);
	SafeRelease(m_d3dRasterizerState);
	SafeRelease(m_d3dSwapChain);
	SafeRelease(m_d3dDeviceContext);
	SafeRelease(m_d3dDevice);

	delete(m_camera);	
	m_camera = NULL;
	delete(m_shaders);
	m_shaders = NULL;
	delete(m_camera);
	m_camera = NULL;
}

/**
* Initialize the GameParent device and swap chain.
*/
int Context::InitDirectX() {

	// A Window handle must have been created already.
	assert(m_window->GetWindowHandle() != 0);

	RECT clientRect;
	GetClientRect(m_window->GetWindowHandle(), &clientRect);

	// Compute the exact client dimensions. This will be used
	// to initialize the render targets for our swap chain.
	unsigned int clientWidth = clientRect.right - clientRect.left;
	unsigned int clientHeight = clientRect.bottom - clientRect.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate = QueryRefreshRate(clientWidth, clientHeight, m_EnableVSync);
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_window->GetWindowHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// These are the feature levels that we will accept.
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// This will be the feature level that 
	// is used to create our device and swap chain.
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &m_d3dSwapChain, &m_d3dDevice,
		&featureLevel,
		&m_d3dDeviceContext);
	if (hr == E_INVALIDARG)
		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_d3dSwapChain, &m_d3dDevice, &featureLevel,
			&m_d3dDeviceContext);
	if (FAILED(hr)) return -1;

	// The Direct3D device and swap chain were successfully created..
	ID3D11Texture2D* backBuffer;
	hr = m_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
	if (FAILED(hr)) return -1;
	hr = m_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &m_d3dRenderTargetView);
	if (FAILED(hr)) return -1;

	SafeRelease(backBuffer);

	// Create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = clientWidth;
	depthStencilBufferDesc.Height = clientHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_d3dDepthStencilBuffer);
	if (FAILED(hr)) return -1;

	hr = m_d3dDevice->CreateDepthStencilView(m_d3dDepthStencilBuffer, nullptr, &m_d3dDepthStencilView);
	if (FAILED(hr)) return -1;

	// Setup depth/stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = m_d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &m_d3dDepthStencilState);
	if (FAILED(hr)) return -1;

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	hr = m_d3dDevice->CreateRasterizerState(&rasterizerDesc, &m_d3dRasterizerState);
	if (FAILED(hr)) return -1;

	// Initialize the viewport to occupy the entire client area.
	m_Viewport.Width = static_cast<float>(clientWidth);
	m_Viewport.Height = static_cast<float>(clientHeight);
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
//	= {
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Mesh::VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Mesh::VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "INSTPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1}
//	};
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = offsetof(Mesh::VertexPosColor, Position);
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = offsetof(Mesh::VertexPosColor, Color);
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	
	polygonLayout[2].SemanticName = "POSITION";
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = 0;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	// Load basic shaders, these arent the only shaders that can be loaded. The same class can be used to create other shaders.
	m_shaders = new Shader(m_d3dDevice, m_d3dDeviceContext);
#if _DEBUG
	if (!m_shaders->LoadShaders(polygonLayout, _countof(polygonLayout), L"SimpleVertexShader_d.cso", L"SimplePixelShader_d.cso")) return -1;
#else
	if (!m_shaders->LoadShaders(vertexLayoutDesc, _countof(vertexLayoutDesc), L"SimpleVertexShader.cso", L"SimplePixelShader.cso")) return -1;
#endif

	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), static_cast<float>(clientWidth) / static_cast<float>(clientHeight), 0.1f, 1000.0f);
	m_shaders->Update(Shader::CB_Application, m_ProjectionMatrix);

	m_camera = new Camera({ 0, 0, -10 }, { 0, 0, 0 }, { 0, 1, 0 }, DirectX::XMFLOAT2{ static_cast<float>(clientWidth / 2), static_cast<float>(clientHeight / 2) });

	return 1;
}

// Clear the color and depth buffers.
void Context::Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil) {
	m_d3dDeviceContext->ClearRenderTargetView(m_d3dRenderTargetView, clearColor);
	m_d3dDeviceContext->ClearDepthStencilView(m_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth,
	                                          clearStencil);
}

void Context::Present() {
	if (m_EnableVSync) m_d3dSwapChain->Present(1, 0);
	else m_d3dSwapChain->Present(0, 0);
}

void Context::Frame() {
	m_camera->Update(GetShaders(), GetWindow());
	Clear(DirectX::Colors::CornflowerBlue, 1.0f, 0);

	for (Mesh* mesh : m_AllMesh) {
		RenderMesh(mesh);
	}

	Present();
}

void Context::RenderMesh(Mesh* mesh) {
	mesh->Render(m_d3dDevice, m_d3dDeviceContext, m_shaders, m_d3dRenderTargetView,
	             m_d3dDepthStencilState, m_d3dDepthStencilView, m_d3dRasterizerState, &m_Viewport);
}

Mesh* Context::CreateMesh(const std::vector<Mesh::VertexPosColor> vertex, const std::vector<WORD> indices, const std::vector<Mesh::InstanceType> instances) {
	Mesh* mesh = new Mesh(m_d3dDevice, vertex, indices, instances);
	m_AllMesh.push_back(mesh);

	return mesh;
}

// This function was inspired by :
// http://www.rastertek.com/dx11tut03.html
DXGI_RATIONAL Context::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync) {
	DXGI_RATIONAL refreshRate = {0, 1};
	if (vsync) {
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		DXGI_MODE_DESC* displayModeList;

		// Create a GameParent graphics interface factory.
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(hr)) {
			MessageBox(nullptr,
			           TEXT("Could not create DXGIFactory instance."),
			           TEXT("Query Refresh Rate"),
			           MB_OK);

			throw new std::exception("Failed to create DXGIFactory.");
		}

		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr)) {
			MessageBox(nullptr,
			           TEXT("Failed to enumerate adapters."),
			           TEXT("Query Refresh Rate"),
			           MB_OK);

			throw new std::exception("Failed to enumerate adapters.");
		}

		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr)) {
			MessageBox(0,
			           TEXT("Failed to enumerate adapter outputs."),
			           TEXT("Query Refresh Rate"),
			           MB_OK);

			throw new std::exception("Failed to enumerate adapter outputs.");
		}

		UINT numDisplayModes;
		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes,
		                                       nullptr);
		if (FAILED(hr)) {
			MessageBox(nullptr,
			           TEXT("Failed to query display mode list."),
			           TEXT("Query Refresh Rate"),
			           MB_OK);

			throw new std::exception("Failed to query display mode list.");
		}

		displayModeList = new DXGI_MODE_DESC[numDisplayModes];
		assert(displayModeList);

		hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes,
		                                       displayModeList);
		if (FAILED(hr)) {
			MessageBox(nullptr,
			           TEXT("Failed to query display mode list."),
			           TEXT("Query Refresh Rate"),
			           MB_OK);

			throw new std::exception("Failed to query display mode list.");
		}

		// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
		for (UINT i = 0; i < numDisplayModes; ++i) {
			if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight) {
				refreshRate = displayModeList[i].RefreshRate;
			}
		}

		delete[] displayModeList;
		SafeRelease(adapterOutput);
		SafeRelease(adapter);
		SafeRelease(factory);
	}

	return refreshRate;
}
