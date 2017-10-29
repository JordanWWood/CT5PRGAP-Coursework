#include "ArtifactPCH.h"
#include "TestGame.h"
#include "Engine/Graphics/Mesh.h"

std::vector<Mesh*> meshList;
Mesh* mesh = nullptr;

// Swap with loaded mesh
std::vector<Shaders::VertexPosColor> g_Vertices = {
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 0
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 1
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 2
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 3
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 4
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) }, // 5
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 6
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) } // 7
};

std::vector<WORD> g_Indicies = {
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};

TestGame::TestGame(BOOL enableVSync, HINSTANCE* hInstance, int* cmdShow): GameParent(enableVSync, hInstance, cmdShow) {}

TestGame::~TestGame() {}

/**
* The main application loop.
*/
int TestGame::Run() {
	for (int i = 0; i < 100; i++) {
		float x = rand() % 100 - 50;
		float y = rand() % 100 - 50;
		float z = rand() % 100;

		mesh = m_Context.CreateMesh(g_Vertices, g_Indicies, {x, y, z}, {0, 0, 0}, {1, 1, 1});
		meshList.push_back(mesh);
	}

	return GameParent::Run();
}

void TestGame::Update(float deltaTime) {
	static float angle = 0.0f;
	angle += 5.0f * deltaTime;
	if (angle > 360) { angle = 0.0f; }

	const int speed = 10;
	if (GetAsyncKeyState('W')) m_Context.GetCamera()->Move(0, 0, speed * deltaTime);
	if (GetAsyncKeyState('A')) m_Context.GetCamera()->Move(-(speed * deltaTime), 0, 0);
	if (GetAsyncKeyState('S')) m_Context.GetCamera()->Move(0, 0, -(speed * deltaTime));
	if (GetAsyncKeyState('D')) m_Context.GetCamera()->Move(speed * deltaTime, 0, 0);

	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 0.1f);

	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationAxis(rotationAxis, -DirectX::XMConvertToRadians((angle * 10)));
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(10 * cos(angle), 10 * sin(angle), 0);

	for (int i = 0; i < 50; i++) {
		rot = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians((angle) * deltaTime));
		translation = DirectX::XMMatrixTranslation(
			(meshList.at(i)->GetPosition().x * cos(angle)) + meshList.at(i)->GetPosition().x,
			(meshList.at(i)->GetPosition().y * sin(angle)) + meshList.at(i)->GetPosition().y, meshList.at(i)->GetPosition().z);

		meshList.at(i)->SetNextMatrix(DirectX::XMMatrixMultiply(translation, rot));
	}

	for (int i = 50; i < 100; i++) {
		rot = DirectX::XMMatrixRotationAxis(rotationAxis, -DirectX::XMConvertToRadians((angle) * deltaTime));
		translation = DirectX::XMMatrixTranslation(
			-((meshList.at(i)->GetPosition().x * cos(angle)) + meshList.at(i)->GetPosition().x),
			-((meshList.at(i)->GetPosition().y * sin(angle)) + meshList.at(i)->GetPosition().y),
			meshList.at(i)->GetPosition().z);

		meshList.at(i)->SetNextMatrix(DirectX::XMMatrixMultiply(translation, rot));
	}
}
