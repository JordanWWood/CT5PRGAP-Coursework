#pragma once

// Creation and managment of m_shaders
class Shaders
{
public:
	Shaders(ID3D11Device* p_Device, ID3D11DeviceContext* p_d3dDeviceContext);
	~Shaders();

	void Shaders::BindShaders(ID3D11RenderTargetView* p_d3dRenderTargetView, ID3D11DepthStencilState* p_d3dDepthStencilState, ID3D11DepthStencilView* p_d3dDepthStencilView, ID3D11RasterizerState *p_d3dRaserizerState, D3D11_VIEWPORT* p_Viewport);
	
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

	void Update(ConstantBuffer p_CB, DirectX::XMMATRIX p_Matrix);
	HRESULT CreateBuffer(D3D11_BUFFER_DESC* p_Desc, ConstantBuffer p_Buffer);

	template <class ShaderClass>
	HRESULT CreateShader(ID3DBlob* p_ShaderBlob, ID3D11ClassLinkage* p_ClassLinkage);

	bool LoadShaders();

	ID3D11InputLayout* GetInputLayout() const { return m_InputLayout; }
private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dDeviceContext;

	ID3D11VertexShader* m_d3dVertexShader = nullptr;
	ID3D11PixelShader* m_d3dPixelShader = nullptr;

	ID3D11Buffer* m_d3dConstantBuffers[NumConstantBuffers];
	ID3D11InputLayout* m_InputLayout = nullptr;
};