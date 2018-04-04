#pragma once
#include "../Engine/GameParent.h"

class Game : public GameParent {
public:
	Game(BOOL enableVSync, HINSTANCE *hInstance, int* cmdShow);
	~Game();

	int Run() override;
	void Update(float deltaTime) override;
};

