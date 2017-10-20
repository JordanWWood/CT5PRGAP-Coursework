#include <ArtifactPCH.h>
#include "Engine/Game.h"

using namespace DirectX;

const BOOL g_EnableVSync = TRUE;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) {
#if _DEBUG 
	AllocConsole();
	FILE* pCin; //for CIN to work
	freopen_s(&pCin, "CONIN$", "r", stdin);
	FILE* pCout; //for COUT to work
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	FILE* pCerr; //for CERR to work
	freopen_s(&pCerr, "CONOUT$", "w", stderr);
#endif

	// Check for DirectXMath library support.
	if (!XMVerifyCPUSupport()) {
		MessageBox(nullptr, TEXT("Failed to verify DirectX Math library support."), TEXT("Error"), MB_OK);
		return -1;
	}

	Game* game = new Game(g_EnableVSync, &hInstance, &cmdShow);
	game->Init();

	return game->Run();
}
