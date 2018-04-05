#include "ArtifactPCH.h"
#include "Game.h"
#include "../Engine/Graphics/Mesh.h"

#include "GameInput.h"

Mesh* mesh;

using namespace DirectX;
// Swap with loaded mesh
std::vector<Mesh::VertexPosColor> g_Vertices = {
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

GameInput input = GameInput();
Game::Game(BOOL enableVSync, HINSTANCE* hInstance, int* cmdShow): GameParent(enableVSync, hInstance, cmdShow, &input) {}

Game::~Game() {}

#define SIZE = 10

// Initilise objects that will be needed from the get go of the game.
int Game::Run() {
	input.SetCamera(m_Context.GetCamera());
	std::vector<Mesh::InstanceType> instances;

	for (int i = 0; i < 50; i++) {
		const float x = rand() % 100 - 50;
		const float y = rand() % 100 - 50;
		const float z = rand() % 100;
		
		Mesh::InstanceType inst{};
		inst.Position = { x, y, z };

		instances.push_back(inst);
	}

	mesh = m_Context.CreateMesh(g_Vertices, g_Indicies, instances);
	// Call the parent function to enter the main loop of the game. We also wait for its return
	return GameParent::Run();
}

// The main update function. This is executed once every frame. DeltaTime is passed through so calculations can be made if the execution time is irregular
void Game::Update(const float deltaTime) {
	static float angle = 0.0f;
	angle += 5.0f * deltaTime;
	if (angle > 360) { angle = 0.0f; }

	const XMVECTOR rotationAxis = XMVectorSet(0.1f, 0.1f, 0.1f, 0.1f);

	XMMATRIX rot{};
	XMMATRIX translation{};
//
//	for (int i = 0; i < 50; i++) {
//		rot = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle * deltaTime));
//		translation = XMMatrixTranslation(
//			(mesh->GetPosition(i).x * cos(angle)) + mesh->GetPosition(i).x,
//			(mesh->GetPosition(i).y * sin(angle)) + mesh->GetPosition(i).y, mesh->GetPosition(i).z);
//
//		mesh->SetNextMatrix(XMMatrixMultiply(translation, rot));
//	}
//
//	for (int i = 50; i < 1000; i++) {
//		rot = XMMatrixRotationAxis(rotationAxis, -XMConvertToRadians(angle * deltaTime));
//		translation = XMMatrixTranslation(
//			-((mesh->GetPosition(i).x * cos(angle)) + mesh->GetPosition(i).x),
//			-((mesh->GetPosition(i).y * sin(angle)) + mesh->GetPosition(i).y),
//			mesh->GetPosition(i).z);
//
//		mesh->SetNextMatrix(XMMatrixMultiply(translation, rot));
//	}
}
