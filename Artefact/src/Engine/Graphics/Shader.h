#pragma once

// Creation and managment of shaders
class Shader {
public:
	Shader(ID3D11Device*, ID3D11DeviceContext*);
	~Shader();

	void Shader::BindShaders(ID3D11RenderTargetView*, ID3D11DepthStencilState*, ID3D11DepthStencilView*, ID3D11RasterizerState*, D3D11_VIEWPORT*);
	
	// Shader resources
	enum ConstantBuffer {
		CB_Application,
		CB_Frame,
		CB_Object,
		NumConstantBuffers
	};	

	void Update(ConstantBuffer, DirectX::XMMATRIX);
	HRESULT CreateBuffer(D3D11_BUFFER_DESC*, ConstantBuffer);

	template <class ShaderClass>
	HRESULT CreateShader(ID3DBlob*, ID3D11ClassLinkage*);

	bool LoadShaders(D3D11_INPUT_ELEMENT_DESC*, int, LPCWSTR, LPCWSTR);

	ID3D11InputLayout* GetInputLayout() const { return m_InputLayout; }
protected:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dDeviceContext;

	ID3D11VertexShader* m_d3dVertexShader = nullptr;
	ID3D11PixelShader* m_d3dPixelShader = nullptr;

	ID3D11Buffer* m_d3dConstantBuffers[NumConstantBuffers];
	ID3D11InputLayout* m_InputLayout = nullptr;
};