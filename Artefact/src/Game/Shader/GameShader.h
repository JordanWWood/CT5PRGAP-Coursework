#pragma once
#include "../../Engine/Graphics/Shader.h"

#define MAX_LIGHTS 8

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

class GameShader : public Shader {
public:
	GameShader(ID3D11Device* p_Device, ID3D11DeviceContext* p_d3dDeviceContext);
	~GameShader();

	bool LoadShaders(ID3D11Device *device);
	void CreateMaterial(DirectX::XMFLOAT4 emissive = { 0.0f, 0.0f, 0.0f, 1.0f }, DirectX::XMFLOAT4 ambient = { 0.1f, 0.1f, 0.1f, 1.0f },
		DirectX::XMFLOAT4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f }, DirectX::XMFLOAT4 specular = { 1.0f, 1.0f, 1.0f, 1.0f }, float specularPower = 128.0f);
	void BindShaders(ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState,
		ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState *d3dRaserizerState, D3D11_VIEWPORT* viewport);
public:
	// Per-vertex data.
	struct VertexPosNormTex {
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex0;
	};
	// Per-instance data (must be 16 byte aligned)
	__declspec(align(16)) struct PlaneInstanceData {
		DirectX::XMMATRIX WorldMatrix;
		DirectX::XMMATRIX InverseTransposeWorldMatrix;
	};

	// A structure to hold the data for a per - object constant buffer
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
private:
	ID3D11SamplerState* m_d3dSamplerState;

	// Per-Frame constant buffer defined in the instanced vertex shader.
	ID3D11Buffer* m_d3dPerFrameConstantBuffer;
	// Per-Object constant buffer defined in the simple vertex shader.
	ID3D11Buffer* m_d3dPerObjectConstantBuffer;

	// Material properties defined in the pixel shader
	ID3D11Buffer* m_d3dMaterialPropertiesConstantBuffer;
	std::vector<MaterialProperties> m_MaterialProperties;

	// Light properties defined in the pixel shader
	ID3D11Buffer* m_d3dLightPropertiesConstantBuffer;
	LightProperties m_LightProperties;
};

