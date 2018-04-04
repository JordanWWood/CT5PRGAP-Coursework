#include "ArtifactPCH.h"
#include "GameShader.h"

GameShader::GameShader(ID3D11Device* p_Device, ID3D11DeviceContext* p_d3dDeviceContext) : Shader(
	p_Device, p_d3dDeviceContext) {}

GameShader::~GameShader() {}

bool GameShader::LoadShaders(ID3D11Device* device) {
	// Create a sampler state for texture sampling in the pixel shader
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &m_d3dSamplerState);
	if (FAILED(hr)) { return false; }

	// Create the input layout for rendering instanced vertex data.
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
		// Per-vertex data.
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		// Per-instance data.
		{"WORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"WORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"WORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"WORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INVERSETRANSPOSEWORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INVERSETRANSPOSEWORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INVERSETRANSPOSEWORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INVERSETRANSPOSEWORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};

	// Create a constant buffer for the per-frame data required by the instanced vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(PerFrameConstantBufferData);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create some materials
	CreateMaterial();
	CreateMaterial(DirectX::XMFLOAT4(0, 0, 0, 1), DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f),
		DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f), DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f), 76.8f);
	CreateMaterial(DirectX::XMFLOAT4(0, 0, 0, 1), DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1), DirectX::XMFLOAT4(0.6f, 0.1f, 0.1f, 1.0f),
		DirectX::XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f), 32.0f);
	CreateMaterial(DirectX::XMFLOAT4(0, 0, 0, 1), DirectX::XMFLOAT4(0.25f, 0.20725f, 0.20725f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.829f, 0.829f, 1.0f),
		DirectX::XMFLOAT4(0.296648f, 0.296648f, 0.296648f, 1.0f), 11.264f);

	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dPerFrameConstantBuffer);
	if (FAILED(hr)) { return false; }

	// Create a constant buffer for the per-object data required by the simple vertex shader.
	constantBufferDesc.ByteWidth = sizeof(PerObjectConstantBufferData);

	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dPerObjectConstantBuffer);
	if (FAILED(hr)) { return false; }

	// Create a constant buffer for the material properties required by the pixel shader.
	constantBufferDesc.ByteWidth = sizeof(MaterialProperties);

	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dMaterialPropertiesConstantBuffer);
	if (FAILED(hr)) { return false; }

	// Create a constant buffer for the light properties required by the pixel shader.
	constantBufferDesc.ByteWidth = sizeof(LightProperties);
	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &m_d3dLightPropertiesConstantBuffer);
	if (FAILED(hr)) { return false; }
}

void GameShader::BindShaders(ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState,
	ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState *d3dRaserizerState, D3D11_VIEWPORT* viewport) {
	m_d3dDeviceContext->VSSetShader(m_d3dVertexShader, nullptr, 0);
	m_d3dDeviceContext->VSSetConstantBuffers(0, 3, m_d3dConstantBuffers);

	m_d3dDeviceContext->RSSetState(d3dRaserizerState);
	m_d3dDeviceContext->RSSetViewports(1, viewport);

	m_d3dDeviceContext->PSSetShader(m_d3dPixelShader, nullptr, 0);

	m_d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);
	m_d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilState, 1);
}

void GameShader::CreateMaterial(DirectX::XMFLOAT4 emissive, DirectX::XMFLOAT4 ambient,
	DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, float specularPower) {
	MaterialProperties mat;
	mat.Material.Emissive = emissive;
	mat.Material.Ambient = ambient;
	mat.Material.Diffuse = diffuse;
	mat.Material.Specular = specular;
	mat.Material.SpecularPower = specularPower;
	m_MaterialProperties.push_back(mat);
}

