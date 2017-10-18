#include "ArtifactPCH.h"
#include "Shaders.h"

Shaders::Shaders(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) {
	m_d3dDeviceContext = d3dDeviceContext;
	m_d3dDevice = d3dDevice;
}

Shaders::~Shaders() {}

void Shaders::BindShaders(ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState, ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState *d3dRaserizerState, D3D11_VIEWPORT* viewport) {
	m_d3dDeviceContext->VSSetShader(m_d3dVertexShader, nullptr, 0);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 3, m_d3dConstantBuffers);

	m_d3dDeviceContext->RSSetState(d3dRaserizerState);
	m_d3dDeviceContext->RSSetViewports(1, viewport);

	m_d3dDeviceContext->PSSetShader(m_d3dPixelShader, nullptr, 0);

	m_d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);
	m_d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilState, 1);
}

void Shaders::Update(ConstantBuffer cb, DirectX::XMMATRIX matrix) {
	m_d3dDeviceContext->UpdateSubresource(m_d3dConstantBuffers[cb], 0, nullptr, &matrix, 0, 0);
}

HRESULT Shaders::CreateBuffer(D3D11_BUFFER_DESC* desc, ConstantBuffer buffer) {
	return m_d3dDevice->CreateBuffer(desc, nullptr, &m_d3dConstantBuffers[buffer]);
}

template <>
HRESULT Shaders::CreateShader<ID3D11VertexShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage) {
	assert(m_d3dDevice);
	assert(pShaderBlob);

	return m_d3dDevice->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &m_d3dVertexShader);
}

template <>
HRESULT Shaders::CreateShader<ID3D11PixelShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage) {
	assert(m_d3dDevice);
	assert(pShaderBlob);

	return m_d3dDevice->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &m_d3dPixelShader);
}

bool Shaders::LoadShaders() {
	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = CreateBuffer(&constantBufferDesc, CB_Application);
	if (FAILED(hr)) { return false; }
	hr = CreateBuffer(&constantBufferDesc, CB_Frame);
	if (FAILED(hr)) { return false; }
	hr = CreateBuffer(&constantBufferDesc, CB_Object);
	if (FAILED(hr)) { return false; }


	// Load the compiled vertex shader.
	ID3DBlob* vertexShaderBlob;
#if _DEBUG
	LPCWSTR compiledVertexShaderObject = L"SimpleVertexShader_d.cso";
#else
	LPCWSTR compiledVertexShaderObject = L"SimpleVertexShader.cso";
#endif

	hr = D3DReadFileToBlob(compiledVertexShaderObject, &vertexShaderBlob);
	if (FAILED(hr)) { return false; }

	hr = CreateShader<ID3D11VertexShader>(vertexShaderBlob, nullptr);
	if (FAILED(hr)) { return false; }

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Shaders::VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Shaders::VertexPosColor,Color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_InputLayout);
	if (FAILED(hr)) { return false; }

	SafeRelease(vertexShaderBlob);

	// Load the compiled pixel shader.
	ID3DBlob* pixelShaderBlob;
#if _DEBUG
	LPCWSTR compiledPixelShaderObject = L"SimplePixelShader_d.cso";
#else
	LPCWSTR compiledPixelShaderObject = L"SimplePixelShader.cso";
#endif

	hr = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
	if (FAILED(hr)) { return false; }

	CreateShader<ID3D11PixelShader>(pixelShaderBlob, nullptr);
	if (FAILED(hr)) { return false; }

	SafeRelease(pixelShaderBlob);

	return true;
}
