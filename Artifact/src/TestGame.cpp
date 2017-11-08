#include "ArtifactPCH.h"
#include "TestGame.h"
#include "Engine/Graphics/Mesh.h"
#include "TestInput.h"

std::vector<Mesh*> meshList;
Mesh* mesh = nullptr;

using namespace DirectX;
// Swap with loaded mesh
std::vector<Shaders::VertexPosColor> g_Vertices = {
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },  // 0
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(1.0f, 1.0f, 0.0f) },  // 1
	{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f) },  // 2
	{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, 1.0f, 1.0f) },  // 3
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f) },  // 4
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT3(1.0f, 0.0f, 1.0f) },  // 5
	{ XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 0.0f, 1.0f) },  // 6
	{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 0.0f) }   // 7
};

std::vector<WORD> g_Indicies = {
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};

TestInput input = TestInput();
TestGame::TestGame(BOOL enableVSync, HINSTANCE* hInstance, int* cmdShow): GameParent(enableVSync, hInstance, cmdShow, &input) {}

TestGame::~TestGame() {}

/**
* The main application loop.
*/
int TestGame::Run() {
	input.SetCamera(m_Context.GetCamera());

	try {
		GameParent::GetEffectFactory()->CreateTexture(L"Textures\\grid_texture.png", m_Context.GetDeviceContext.Get(), &m_DirectXTexture);
	} catch (std::exception&) {
		MessageBoxA(m_Context.GetWindow.GetWindow(), "Failed to load texture.", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	for (int i = 0; i < 1000; i++) {
		const float x = rand() % 100 - 50;
		const float y = rand() % 100 - 50;
		const float z = rand() % 100;

		mesh = m_Context.CreateMesh(g_Vertices, g_Indicies, {x, y, z}, {0, 0, 0}, {1, 1, 1});
		meshList.push_back(mesh);
	}

	return GameParent::Run();
}

void TestGame::Update(const float deltaTime) {
	static float angle = 0.0f;
	angle += 5.0f * deltaTime;
	if (angle > 360) { angle = 0.0f; }

	const XMVECTOR rotationAxis = XMVectorSet(0.1f, 0.1f, 0.1f, 0.1f);

	XMMATRIX rot;
	XMMATRIX translation;

	for (int i = 0; i < 499; i++) {
		rot = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle * deltaTime));
		translation = XMMatrixTranslation(
			(meshList.at(i)->GetPosition().x * cos(angle)) + meshList.at(i)->GetPosition().x,
			(meshList.at(i)->GetPosition().y * sin(angle)) + meshList.at(i)->GetPosition().y, meshList.at(i)->GetPosition().z);

		meshList.at(i)->SetNextMatrix(XMMatrixMultiply(translation, rot));
	}

	for (int i = 500; i < 1000; i++) {
		rot = XMMatrixRotationAxis(rotationAxis, -XMConvertToRadians(angle * deltaTime));
		translation = XMMatrixTranslation(
			-((meshList.at(i)->GetPosition().x * cos(angle)) + meshList.at(i)->GetPosition().x),
			-((meshList.at(i)->GetPosition().y * sin(angle)) + meshList.at(i)->GetPosition().y),
			meshList.at(i)->GetPosition().z);

		meshList.at(i)->SetNextMatrix(XMMatrixMultiply(translation, rot));
	}
}
