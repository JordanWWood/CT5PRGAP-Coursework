#pragma once
#include "../Engine/GameParent.h"

class Game : public GameParent {
public:
	Game(BOOL, HINSTANCE*, int*);
	~Game();

	int Run() override;
	void Update(float) override;
};

