#pragma once

// Creation and managment of shaders
class Shaders
{
public:
	// Shader resources
	enum ConstantBuffer {
		CB_Frame,
		CB_Object,
		CB_MaterialProperties,
		CB_LightProperties,
		NumConstantBuffers
	};	

	// A structure to hold the data for a per-object constant buffer
	// defined in the vertex shader.
	struct PerFrameConstantBufferData {
		DirectX::XMMATRIX ViewProjectionMatrix;
	};

	// This structure is used in the simple vertex shader.
	struct PerObjectConstantBufferData {
		DirectX::XMMATRIX WorldMatrix;
		DirectX::XMMATRIX InverseTransposeWorldMatrix;
		DirectX::XMMATRIX WorldViewProjectionMatrix;
	};

	// Per-vertex data.
	struct VertexPosNormTex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex0;
	};

	struct _Material
	{
		_Material()
			: Emissive(0.0f, 0.0f, 0.0f, 1.0f)
			, Ambient(0.1f, 0.1f, 0.1f, 1.0f)
			, Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
			, Specular(1.0f, 1.0f, 1.0f, 1.0f)
			, SpecularPower(128.0f)
			, UseTexture(false)
		{}

		DirectX::XMFLOAT4   Emissive;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Ambient;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Diffuse;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Specular;
		//----------------------------------- (16 byte boundary)
		float               SpecularPower;
		// Add some padding complete the 16 byte boundary.
		int                 UseTexture;
		// Add some padding to complete the 16 byte boundary.
		float                 Padding[2];
		//----------------------------------- (16 byte boundary)
	}; // Total:                                80 bytes (5 * 16)

	struct MaterialProperties
	{
		_Material   Material;
	};

#define MAX_LIGHTS 8
	
	enum LightType
	{
		DirectionalLight = 0,
		PointLight = 1,
		SpotLight = 2
	};

	struct Light
	{
		Light()
			: Position(0.0f, 0.0f, 0.0f, 1.0f)
			, Direction(0.0f, 0.0f, 1.0f, 0.0f)
			, Color(1.0f, 1.0f, 1.0f, 1.0f)
			, SpotAngle(DirectX::XM_PIDIV2)
			, ConstantAttenuation(1.0f)
			, LinearAttenuation(0.0f)
			, QuadraticAttenuation(0.0f)
			, LightType(DirectionalLight)
			, Enabled(0)
		{}

		DirectX::XMFLOAT4    Position;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4    Direction;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4    Color;
		//----------------------------------- (16 byte boundary)
		float       SpotAngle;
		float       ConstantAttenuation;
		float       LinearAttenuation;
		float       QuadraticAttenuation;
		//----------------------------------- (16 byte boundary)
		int         LightType;
		int         Enabled;
		// Add some padding to make this struct size a multiple of 16 bytes.
		int         Padding[2];
		//----------------------------------- (16 byte boundary)
	};  // Total:                              80 bytes ( 5 * 16 )

	struct LightProperties
	{
		LightProperties()
			: EyePosition(0.0f, 0.0f, 0.0f, 1.0f)
			, GlobalAmbient(0.2f, 0.2f, 0.8f, 1.0f)
		{}

		DirectX::XMFLOAT4   EyePosition;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   GlobalAmbient;
		//----------------------------------- (16 byte boundary)
		Light               Lights[MAX_LIGHTS]; // 80 * 8 bytes
	};  // Total:                                  672 bytes (42 * 16)

	Shaders(ID3D11Device* p_Device, ID3D11DeviceContext* p_d3dDeviceContext);
	~Shaders();

	void Shaders::BindShaders(ID3D11RenderTargetView* p_d3dRenderTargetView, ID3D11DepthStencilState* p_d3dDepthStencilState, ID3D11DepthStencilView* p_d3dDepthStencilView, ID3D11RasterizerState *p_d3dRaserizerState, D3D11_VIEWPORT* p_Viewport);
	
	
	// Per-instance data (must be 16 byte aligned)
	__declspec(align(16)) struct PlaneInstanceData
	{
		DirectX::XMMATRIX WorldMatrix;
		DirectX::XMMATRIX InverseTransposeWorldMatrix;
	};

	void Update(ConstantBuffer cb, DirectX::XMMATRIX matrix);
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