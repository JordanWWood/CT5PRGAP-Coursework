#include "ArtifactPCH.h"
#include "Graphics/Mesh.h"
#include "Game.h"

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;

LPCSTR g_WindowName = "Game";

std::vector<Mesh*> meshList;
Mesh* mesh = nullptr;
Mesh* mesh1 = nullptr;

std::vector<Shaders::VertexPosColor> g_Vertices = {
	{DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)}, // 0
	{DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f)}, // 1
	{DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)}, // 2
	{DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f)}, // 3
	{DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)}, // 4
	{DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f)}, // 5
	{DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)}, // 6
	{DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)} // 7
};

std::vector<WORD> g_Indicies = {
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};

Game::Game(BOOL enableVSync, HINSTANCE* hInstance, int* cmdShow) : m_Context(Context(hInstance, cmdShow, g_WindowWidth,
                                                                                     g_WindowHeight, g_WindowName,
                                                                                     enableVSync)),
                                                                   m_Input(Input()), m_HInstance(hInstance) {}

Game::~Game() {
	m_Input.Shutdown();
}

int Game::Init() {
	if (!DirectX::XMVerifyCPUSupport()) {
		MessageBox(nullptr, TEXT("Failed to verify Game Math library support."), TEXT("Error"), MB_OK);
		return -1;
	}

	if (!m_Context.InitDirectX()) {
		MessageBox(nullptr, TEXT("Could not initialize DirectX."), TEXT("Error"), MB_OK);
		return false;
	}

	if (!m_Input.Initialize(*m_HInstance, m_Context.GetWindow()->GetWindowHandle(), g_WindowWidth, g_WindowHeight)) {
		MessageBox(nullptr, TEXT("Could not initialize the input object."), TEXT("Error"), MB_OK);
		return false;
	}

	return true;
}

/**
* The main application loop.
*/
int Game::Run() {
	MSG msg = {nullptr};

	static DWORD previousTime = timeGetTime();
	static const float targetFramerate = 30.0f;
	static const float maxTimeStep = 1.0f / targetFramerate;
	mesh1 = m_Context.CreateMesh(g_Vertices, g_Indicies, { 0, 0, 0 }, { 0, 0, 0 }, { .5f, .5f, .5f });

	for (int i = 0; i < 100; i++) {
		float x = rand() % 100 - 50;
		float y = rand() % 100 - 50;
		float z = rand() % 100;

		mesh = m_Context.CreateMesh(g_Vertices, g_Indicies, { x, y, z }, { 0, 0, 0 }, { 1, 1, 1 });
		meshList.push_back(mesh);
	}

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - previousTime) / 1000.0f;
			previousTime = currentTime;

			// Cap the delta time to the max time step
			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			Update(deltaTime);
			m_Context.Frame();
		}
	}

	return static_cast<int>(msg.wParam);
}

void Game::Update(float deltaTime) {
	static float angle = 0.0f;
	if (GetAsyncKeyState(VK_LEFT)) {
		angle += 5.0f * deltaTime;
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		angle -= 5.0f * deltaTime;
	}

	angle += 5.0f * deltaTime;

	if (angle > 360) {
		angle = 0.0f;
	}

	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0.1f, 0.1f, 0.1f, 0.1f);

//	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians((angle * 5)));
//	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(sin((angle * 5) / 50), sin((angle * 5) / 100),
//	                                                        sin((angle * 5) / 10));
//	mesh->SetNextMatrix(XMMatrixMultiply(matrix, rot));
//	mesh->Rotate({ sin(angle / 10), cos(angle / 10), sin(angle / 10) });

	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationAxis(rotationAxis, -DirectX::XMConvertToRadians((angle * 10)));
	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(10 * cos(angle), 10 * sin(angle), 0);
	mesh1->SetNextMatrix(DirectX::XMMatrixMultiply(matrix, rot));

//	for (Mesh* me : meshList) {
//		rot = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians((angle) * 250));
//		matrix = DirectX::XMMatrixTranslation((me->GetPosition().x * cos(angle)) + me->GetPosition().x, (me->GetPosition().y * sin(angle)) + me->GetPosition().y, me->GetPosition().z);
//
//		me->SetNextMatrix(DirectX::XMMatrixMultiply(matrix, rot));
//	}

	for (int i = 0; i < 50; i++) {
		rot = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians((angle) * 100));
		matrix = DirectX::XMMatrixTranslation((meshList.at(i)->GetPosition().x * cos(angle)) + meshList.at(i)->GetPosition().x, (meshList.at(i)->GetPosition().y * sin(angle)) + meshList.at(i)->GetPosition().y, meshList.at(i)->GetPosition().z);

		meshList.at(i)->SetNextMatrix(DirectX::XMMatrixMultiply(matrix, rot));
	}

	for (int i = 50; i < 100; i++) {
		rot = DirectX::XMMatrixRotationAxis(rotationAxis, -DirectX::XMConvertToRadians((angle) * 100));
		matrix = DirectX::XMMatrixTranslation(-((meshList.at(i)->GetPosition().x * cos(angle)) + meshList.at(i)->GetPosition().x), -((meshList.at(i)->GetPosition().y * sin(angle)) + meshList.at(i)->GetPosition().y), meshList.at(i)->GetPosition().z);

		meshList.at(i)->SetNextMatrix(DirectX::XMMatrixMultiply(matrix, rot));
	}
}