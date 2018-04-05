#include "ArtifactPCH.h"
#include "Shader.h"
#include "comdef.h"

Shader::Shader(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext) {
	m_d3dDeviceContext = d3dDeviceContext;
	m_d3dDevice = d3dDevice;
}

Shader::~Shader()
{
	delete(m_d3dDevice);
	delete(m_d3dDeviceContext);
	delete(m_d3dVertexShader);
	delete(m_d3dPixelShader);
	delete(m_d3dConstantBuffers);
	delete(m_InputLayout);

	m_d3dDevice = nullptr;
	m_d3dDeviceContext = nullptr;
	m_d3dVertexShader = nullptr;
	m_d3dPixelShader = nullptr;
	m_InputLayout = nullptr;
}

void Shader::BindShaders(ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState, ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState *d3dRaserizerState, D3D11_VIEWPORT* viewport) {
	m_d3dDeviceContext->VSSetShader(m_d3dVertexShader, nullptr, 0);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 3, m_d3dConstantBuffers);

	m_d3dDeviceContext->RSSetState(d3dRaserizerState);
	m_d3dDeviceContext->RSSetViewports(1, viewport);

	m_d3dDeviceContext->PSSetShader(m_d3dPixelShader, nullptr, 0);

	m_d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);
	m_d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilState, 1);
}

void Shader::Update(ConstantBuffer cb, DirectX::XMMATRIX matrix) {
	m_d3dDeviceContext->UpdateSubresource(m_d3dConstantBuffers[cb], 0, nullptr, &matrix, 0, 0);
}

HRESULT Shader::CreateBuffer(D3D11_BUFFER_DESC* desc, ConstantBuffer buffer) {
	return m_d3dDevice->CreateBuffer(desc, nullptr, &m_d3dConstantBuffers[buffer]);
}

template <>
HRESULT Shader::CreateShader<ID3D11VertexShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage) {
	assert(m_d3dDevice);
	assert(pShaderBlob);

	return m_d3dDevice->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &m_d3dVertexShader);
}

template <>
HRESULT Shader::CreateShader<ID3D11PixelShader>(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage) {
	assert(m_d3dDevice);
	assert(pShaderBlob);

	return m_d3dDevice->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &m_d3dPixelShader);
}

bool Shader::LoadShaders(D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc, int size, LPCWSTR compiledVertexShaderObject, LPCWSTR compiledPixelShaderObject) {
	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = CreateBuffer(&constantBufferDesc, CB_Application);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed to create constant buffer CB_Application");
#endif
	}
	hr = CreateBuffer(&constantBufferDesc, CB_Frame);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed to create constant buffer CB_Frame");
#endif
	}
	hr = CreateBuffer(&constantBufferDesc, CB_Object);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed to create constant buffer CB_Object");
#endif
	}

	// Load the compiled vertex shader.
	ID3DBlob* vertexShaderBlob;

	hr = D3DReadFileToBlob(compiledVertexShaderObject, &vertexShaderBlob);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed to read vertex shader to blob");
#endif
	}

	hr = CreateShader<ID3D11VertexShader>(vertexShaderBlob, nullptr);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed create vertex shader from blob");
#endif
	}

	hr = m_d3dDevice->CreateInputLayout(vertexLayoutDesc, size, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_InputLayout);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed to create input layout");
#endif
	}

	SafeRelease(vertexShaderBlob);

	// Load the compiled pixel shader.
	ID3DBlob* pixelShaderBlob;

	hr = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed to read pixel shader to blob");
#endif
	}

	CreateShader<ID3D11PixelShader>(pixelShaderBlob, nullptr);
	if (FAILED(hr)) {
#if _DEBUG
		_com_error err(hr);
		FatalAppExit(0, err.ErrorMessage());
#else
		FatalAppExit(0, "Failed create pixel shader from blob");
#endif
	}

	SafeRelease(pixelShaderBlob);

	return true;
}
