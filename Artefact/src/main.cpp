#include <ArtifactPCH.h>
#include "Game/Game.h"

using namespace DirectX;

const BOOL g_EnableVSync = TRUE;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow) {
#if _DEBUG 
	// Enable console only if we are running the application with the Debug Configuration
	AllocConsole();
	FILE* pCin; //for CIN to work
	freopen_s(&pCin, "CONIN$", "r", stdin);
	FILE* pCout; //for COUT to work
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	FILE* pCerr; //for CERR to work
	freopen_s(&pCerr, "CONOUT$", "w", stderr);
#endif

	// Construct and initialise the game object.
	Game* game = new Game(g_EnableVSync, &hInstance, &cmdShow);
	game->Init();

	// Wait for run to return. This is the games main loop and should return if anything goes wrong or the game is closed.
	return game->Run();
}
