#pragma once
#include "Input/Input.h"
#include "Graphics/Context.h"

// High level interaction with rendering for simple and effective interaction. Creating a DirectX context for the game.
class Game {
public:
	Game(BOOL enableVSync, HINSTANCE *hInstance, int* cmdShow);
	~Game();

	int Init();
	int Run();

	void Update(float deltaTime);
private:
	class Context m_Context;
	Input m_Input;

	HINSTANCE* m_HInstance = nullptr;
};