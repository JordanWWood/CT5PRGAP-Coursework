#pragma once
#include "Engine/GameParent.h"

class LightingTest : public GameParent
{
public:
	LightingTest(BOOL enableVSync, HINSTANCE *hInstance, int* cmdShow);
	~LightingTest();

	int Run() override;
	void Update(float deltaTime) override;

	ID3D11ShaderResourceView* m_Texture = nullptr;

private:
	std::vector<Shaders::MaterialProperties> m_MaterialProperties;
};

