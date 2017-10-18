#include "ArtifactPCH.h"
#include "Graphics/Mesh.h"
#include "Graphics/Context.h"
#include "Game.h"

const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;

LPCSTR g_WindowName = "Game";

Mesh* mesh = nullptr;
Mesh* mesh1 = nullptr;

std::vector<Shaders::VertexPosColor> g_Vertices = {
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 0
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },  // 1
	{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },   // 2
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },  // 3
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },  // 4
	{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },   // 5
	{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },    // 6
	{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) }    // 7
};

std::vector<WORD> g_Indicies = {
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};

Game::Game(BOOL enableVSync, HINSTANCE *hInstance, int* cmdShow) : m_Context(new Context(hInstance, cmdShow, g_WindowWidth, g_WindowHeight, g_WindowName, enableVSync)) {}

Game::~Game() {
	delete(m_Context);
	m_Context = NULL;
}

int Game::Init() const {
	if (!DirectX::XMVerifyCPUSupport()) {
		MessageBox(nullptr, TEXT("Failed to verify Game Math library support."), TEXT("Error"), MB_OK);
		return -1;
	}

	return m_Context->InitDirectX();;
}

/**
* The main application loop.
*/
int Game::Run() {
	MSG msg = { nullptr };

	static DWORD previousTime = timeGetTime();
	static const float targetFramerate = 30.0f;
	static const float maxTimeStep = 1.0f / targetFramerate;
	mesh = m_Context->CreateMesh(g_Vertices, g_Indicies);
	mesh1 = m_Context->CreateMesh(g_Vertices, g_Indicies);

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - previousTime) / 1000.0f;
			previousTime = currentTime;

			// Cap the delta time to the max time step (useful if your 
			// debugging and you don't want the deltaTime value to explode.
			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			Update(deltaTime);

			m_Context->Clear(DirectX::Colors::CornflowerBlue, 1.0f, 0);

			m_Context->RenderMesh(mesh);
			m_Context->RenderMesh(mesh1);
			m_Context->Present();
		}
	}

	return static_cast<int>(msg.wParam);
}

void Game::Update(float deltaTime) {
	const DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
	const DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
	const DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);

	// Camera
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	m_Context->GetShaders()->Update(Shaders::CB_Frame, m_ViewMatrix);

	static float angle = 0.0f;
	if (GetAsyncKeyState(VK_LEFT)) {
		angle += 5.0f * deltaTime;
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		angle -= 5.0f * deltaTime;
	}

	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(.5f, .5f, .5f, .5f);

	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians((angle * 5)));
	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(sin((angle * 5) / 50), sin((angle * 5) / 100), sin((angle * 5) / 10));
	mesh->SetNextMatrix(XMMatrixMultiply(matrix, rot));

	rot = DirectX::XMMatrixRotationAxis(rotationAxis, -DirectX::XMConvertToRadians((angle * 10)));
	matrix = DirectX::XMMatrixTranslation(3 * cos(angle), 3 * sin(angle), 0);
	mesh1->SetNextMatrix(DirectX::XMMatrixMultiply(matrix, rot));
}