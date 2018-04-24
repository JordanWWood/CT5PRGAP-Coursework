#include "ArtifactPCH.h"
#include "Game.h"
#include "../Engine/Graphics/Mesh.h"

#include "GameInput.h"
#include <random>

Mesh* mesh;

using namespace DirectX;
// Swap with loaded mesh
std::vector<XMFLOAT3> starts;

std::vector<Shader::VertexPosColor> g_Vertices = {
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
#define M_PI           3.14159265358979323846  /* pi */

// Initilise objects that will be needed from the get go of the game.
int Game::Run() {
	std::vector<Shader::InstanceType> instances;
	input.SetCamera(m_Context.GetCamera());

	for (int i = 0; i < 20000; i++) {
		const float x = rand() % 100 - 50;
		const float y = rand() % 100 - 50;
		const float z = rand() % 100;

		std::mt19937 rng(std::random_device{}()); //Unless you are using MinGW
		std::uniform_real_distribution<> dist(0, 2 * M_PI);

		float rotX = dist(rng);
		float rotY = dist(rng);
		float rotZ = dist(rng);

		auto* inst = new Shader::InstanceType;
		inst->Position = { x, y, z };
		inst->Rotaion = { rotX, rotY, rotZ };

		starts.push_back({ x, y, z });
		instances.push_back(*inst);
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

//	for (int i = 0; i < 50; i++) {
//		mesh->Rotate({sin(angle), .0f, .0f }, i);
//
//		mesh->Move((starts.at(i).x * cos(angle)),
//			(starts.at(i).y * sin(angle)),
//			starts.at(i).z, i);
//	}

	for (int i = 0; i < 20000; i++) {
		mesh->Rotate({ sin(angle), cos(angle), .0f }, i);

		mesh->Move(((starts.at(i).x * cos(angle)) + mesh->GetPosition(i).x),
			((starts.at(i).y * sin(angle)) + mesh->GetPosition(i).y),
			starts.at(i).z, i);
	}


	std::cout << "Angle: " << angle << std::endl;
}
