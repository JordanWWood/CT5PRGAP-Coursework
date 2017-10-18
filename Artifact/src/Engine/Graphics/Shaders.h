#pragma once

// Creation and managment of m_shaders
class Shaders
{
public:
	Shaders(ID3D11Device* device, ID3D11DeviceContext* d3dDeviceContext);
	~Shaders();

	void Shaders::BindShaders(ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState, ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState *d3dRaserizerState, D3D11_VIEWPORT* viewport);
	
	// Shader resources
	enum ConstantBuffer {
		CB_Application,
		CB_Frame,
		CB_Object,
		NumConstantBuffers
	};	
	
	// Vertex data for a colored cube.
	struct VertexPosColor {
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;
	};

	void Update(ConstantBuffer cb, DirectX::XMMATRIX matrix);
	HRESULT CreateBuffer(D3D11_BUFFER_DESC* desc, ConstantBuffer buffer);

	template <class ShaderClass>
	HRESULT CreateShader(ID3DBlob* pShaderBlob, ID3D11ClassLinkage* pClassLinkage);

	bool LoadShaders();

	ID3D11InputLayout* GetInputLayout() const { return m_InputLayout; }
private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dDeviceContext;

	// Shader data
	ID3D11VertexShader* m_d3dVertexShader = nullptr;
	ID3D11PixelShader* m_d3dPixelShader = nullptr;

	ID3D11Buffer* m_d3dConstantBuffers[NumConstantBuffers];
	ID3D11InputLayout* m_InputLayout = nullptr;
};