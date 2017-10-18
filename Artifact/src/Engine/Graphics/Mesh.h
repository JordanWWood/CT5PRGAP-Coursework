#pragma once
#include "Shaders.h"
#include <vector>

class Mesh
{
public:
	Mesh(const std::vector<Shaders::VertexPosColor> p_vertex, const std::vector<WORD> p_indicies, ID3D11Device* d3dDevice);
	~Mesh();

	void Render(ID3D11Device* m_d3dDevice, ID3D11DeviceContext* m_d3dDeviceContext,
		Shaders* m_Shaders, ID3D11RenderTargetView* p_d3dRenderTargetView, ID3D11DepthStencilState* p_d3dDepthStencilState,
		ID3D11DepthStencilView* p_d3dDepthStencilView, ID3D11RasterizerState* p_d3dRasterizerState, D3D11_VIEWPORT* p_Viewport);

	static Mesh* LoadFromFile(std::string p_Path, ID3D11Device* p_Device);

	std::vector<Shaders::VertexPosColor> GetVertices() const { return m_Vertices; }
	std::vector<WORD> GetIndicies() const { return m_Indicies; }

	void SetNextMatrix(DirectX::XMMATRIX p_Matrix) { m_nextMatrix = p_Matrix; }
private:
	std::vector<Shaders::VertexPosColor> m_Vertices;
	std::vector<WORD> m_Indicies;

	ID3D11InputLayout* m_d3dInputLayout = nullptr;
	ID3D11Buffer* m_d3dVertexBuffer = nullptr;
	ID3D11Buffer* m_d3dIndexBuffer = nullptr;

	DirectX::XMMATRIX m_nextMatrix;
};

