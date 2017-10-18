#pragma once
#include "Shaders.h"
#include <vector>

class Mesh
{
public:
	Mesh(const std::vector<Shaders::VertexPosColor> p_vertex, const std::vector<WORD> p_indicies, ID3D11Device* d3dDevice);
	~Mesh();

	void Render(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext,
		Shaders* shaders, ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState,
		ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState* d3dRasterizerState, D3D11_VIEWPORT* viewport);

	static Mesh* LoadFromFile(std::string path, ID3D11Device* device);

	std::vector<Shaders::VertexPosColor> GetVertices() const { return m_Vertices; }
	std::vector<WORD> GetIndicies() const { return m_Indicies; }

	void SetNextMatrix(DirectX::XMMATRIX matrix) { m_nextMatrix = matrix; }
private:
	std::vector<Shaders::VertexPosColor> m_Vertices;
	std::vector<WORD> m_Indicies;

	ID3D11InputLayout* m_d3dInputLayout = nullptr;
	ID3D11Buffer* m_d3dVertexBuffer = nullptr;
	ID3D11Buffer* m_d3dIndexBuffer = nullptr;

	DirectX::XMMATRIX m_nextMatrix;
};

