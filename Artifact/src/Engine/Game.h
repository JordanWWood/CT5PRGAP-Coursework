#pragma once
#include "Graphics/Context.h"

// High level interaction with rendering for simple and effective interaction. Creating a DirectX context for the game.
class Game {
public:
	Game(BOOL enableVSync, HINSTANCE *hInstance, int* cmdShow);
	~Game();

	int Init() const;
	int Run();

	void Update(float deltaTime);
private:
	Context* m_Context;
};