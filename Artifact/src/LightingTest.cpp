#include "ArtifactPCH.h"
#include "LightingTest.h"
#include "TestInput.h"
#include "Engine/Graphics/Mesh.h"

// Vertices for a unit plane.
std::vector<Shaders::VertexPosNormTex> g_PlaneVerts = {
	{DirectX::XMFLOAT3(-0.5f, 0.0f,  0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) }, // 0
	{DirectX::XMFLOAT3(0.5f, 0.0f,  0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) }, // 1
	{DirectX::XMFLOAT3(0.5f, 0.0f, -0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }, // 2
	{DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }  // 3
};

// Index buffer for the unit plane.
std::vector<WORD> g_PlaneIndex = {
	0, 1, 3, 1, 2, 3
};

TestInput input = TestInput();
LightingTest::LightingTest(BOOL enableVSync, HINSTANCE* hInstance, int* cmdShow) : GameParent(enableVSync, hInstance, cmdShow, &input) {}

LightingTest::~LightingTest() {}

int LightingTest::Run() {
	input.SetCamera(m_Context.GetCamera());

	try {
		m_EffectFactory->CreateTexture(L"Textures\\grid_texture.png", m_Context.GetDeviceContext(), &m_Texture);
	} catch (std::exception&) {
		MessageBoxA(m_Context.GetWindow()->GetWindowHandle(), "Failed to load texture.", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	Shaders::MaterialProperties defaultMaterial;
	m_MaterialProperties.push_back(defaultMaterial);

	Shaders::MaterialProperties greenMaterial;
	greenMaterial.Material.Ambient = DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	greenMaterial.Material.Diffuse = DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	greenMaterial.Material.Specular = DirectX::XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	greenMaterial.Material.SpecularPower = 76.8f;
	m_MaterialProperties.push_back(greenMaterial);

	Shaders::MaterialProperties redPlasticMaterial;
	redPlasticMaterial.Material.Diffuse = DirectX::XMFLOAT4(0.6f, 0.1f, 0.1f, 1.0f);
	redPlasticMaterial.Material.Specular = DirectX::XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	redPlasticMaterial.Material.SpecularPower = 32.0f;
	m_MaterialProperties.push_back(redPlasticMaterial);

	Shaders::MaterialProperties pearlMaterial;
	pearlMaterial.Material.Ambient = DirectX::XMFLOAT4(0.25f, 0.20725f, 0.20725f, 1.0f);
	pearlMaterial.Material.Diffuse = DirectX::XMFLOAT4(1.0f, 0.829f, 0.829f, 1.0f);
	pearlMaterial.Material.Specular = DirectX::XMFLOAT4(0.296648f, 0.296648f, 0.296648f, 1.0f);
	pearlMaterial.Material.SpecularPower = 11.264f;
	m_MaterialProperties.push_back(pearlMaterial);

	Mesh::InstanceData* planeInstanceData = static_cast<Mesh::InstanceData*>(_aligned_malloc(sizeof(Mesh::InstanceData) * 5, 16));

	float scalePlane = 20.0f;
	float translateOffset = scalePlane / 2.0f;
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scalePlane, 1.0f, scalePlane);
	DirectX::XMMATRIX translateMatrix = DirectX::XMMatrixTranslation(0, 0, 0);
	DirectX::XMMATRIX rotateMatrix = DirectX::XMMatrixRotationX(0.0f);

	// Floor plane.
	DirectX::XMMATRIX worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
	planeInstanceData[0].WorldMatrix = worldMatrix;
	planeInstanceData[0].InverseTransposeWorldMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, worldMatrix));

	// Back wall plane.
	translateMatrix = DirectX::XMMatrixTranslation(0, translateOffset, translateOffset);
	rotateMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-90));
	worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	planeInstanceData[1].WorldMatrix = worldMatrix;
	planeInstanceData[1].InverseTransposeWorldMatrix = XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, worldMatrix));

	// Ceiling plane.
	translateMatrix = DirectX::XMMatrixTranslation(0, translateOffset * 2.0f, 0);
	rotateMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(180));
	worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	planeInstanceData[2].WorldMatrix = worldMatrix;
	planeInstanceData[2].InverseTransposeWorldMatrix = XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, worldMatrix));

	// Front wall plane.
	translateMatrix = DirectX::XMMatrixTranslation(0, translateOffset, -translateOffset);
	rotateMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
	worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	planeInstanceData[3].WorldMatrix = worldMatrix;
	planeInstanceData[3].InverseTransposeWorldMatrix = XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, worldMatrix));

	// Left wall plane.
	translateMatrix = DirectX::XMMatrixTranslation(-translateOffset, translateOffset, 0);
	rotateMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(-90));
	worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	planeInstanceData[4].WorldMatrix = worldMatrix;
	planeInstanceData[4].InverseTransposeWorldMatrix = XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, worldMatrix));

	// Right wall plane.
	translateMatrix = DirectX::XMMatrixTranslation(translateOffset, translateOffset, 0);
	rotateMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(90));
	worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

	planeInstanceData[5].WorldMatrix = worldMatrix;
	planeInstanceData[5].InverseTransposeWorldMatrix = XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, worldMatrix));

	Mesh* plane = m_Context.CreateMesh(g_PlaneVerts, g_PlaneIndex, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	plane->CreateInstanceBuffer(m_Context.GetDevice(), 4, planeInstanceData);

	return GameParent::Run();
}
void LightingTest::Update(float deltaTime) {}
