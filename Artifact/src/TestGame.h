#pragma once
#include "Engine/GameParent.h"

class TestGame : public GameParent
{
public:
	TestGame(BOOL enableVSync, HINSTANCE *hInstance, int* cmdShow);
	~TestGame();

	int Run() override;
	void Update(float deltaTime) override;

	ID3D11ShaderResourceView* m_Texture = nullptr;
};

