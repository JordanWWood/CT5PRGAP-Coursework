#pragma once
#include "Shaders.h"
#include <vector>

class Mesh
{
public:
	Mesh(ID3D11Device* pD3DDevice, const std::vector<Shaders::VertexPosColor> pVertex, const std::vector<WORD> pIndicies, const DirectX::XMFLOAT3 pPosition, const DirectX::XMFLOAT3 pRotation, const DirectX::XMFLOAT3 pScale);
	~Mesh();

	void Render(ID3D11Device* m_d3dDevice, ID3D11DeviceContext* m_d3dDeviceContext,
		Shaders* m_Shaders, ID3D11RenderTargetView* pD3DRenderTargetView, ID3D11DepthStencilState* pD3DDepthStencilState,
		ID3D11DepthStencilView* pD3DDepthStencilView, ID3D11RasterizerState* pD3DRasterizerState, D3D11_VIEWPORT* pViewport);

	static Mesh* LoadFromFile(ID3D11Device* device, std::string path, const DirectX::XMFLOAT3 pPosition, const DirectX::XMFLOAT3 pRotation, const DirectX::XMFLOAT3 pScale);

	void Move(float x, float y, float z) { Move({ x, y, z }); }
	void Move(DirectX::XMFLOAT3 vec);
	void Rotate(float x, float y, float z) { Rotate({ x, y, z }); }
	void Rotate(DirectX::XMFLOAT3 vec);
	void Scale(float x, float y, float z) { Scale({ x, y, z }); }
	void Scale(DirectX::XMFLOAT3 vec);

	std::vector<Shaders::VertexPosColor> GetVertices() const { return m_Vertices; }
	std::vector<WORD> GetIndicies() const { return m_Indicies; }

	DirectX::XMFLOAT3 GetPosition() const { return m_position; }
	DirectX::XMFLOAT3 GetRotation() const { return m_rotation; }
	DirectX::XMFLOAT3 GetScale() const { return m_scale; }

	void SetNextMatrix(DirectX::XMMATRIX pMatrix) { m_nextMatrix = pMatrix; }
private:
	void CalculateNextMatrix();

	std::vector<Shaders::VertexPosColor> m_Vertices;
	std::vector<WORD> m_Indicies;

	ID3D11InputLayout* m_d3dInputLayout = nullptr;
	ID3D11Buffer* m_d3dVertexBuffer = nullptr;
	ID3D11Buffer* m_d3dIndexBuffer = nullptr;

	DirectX::XMMATRIX m_nextMatrix;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
};

