#pragma once
#include <Artefact.h>

class Game : public Artefact::GameParent {
public:
	Game(BOOL, HINSTANCE*, int*);
	~Game();

	int Run() override;
	void Update(float) override;
};

