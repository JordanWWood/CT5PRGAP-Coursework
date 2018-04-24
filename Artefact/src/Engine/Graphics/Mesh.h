#pragma once
#include "Shader.h"
#include <vector>

class Mesh
{
public:
	Mesh(ID3D11Device*, const std::vector<Shader::VertexPosColor>, const std::vector<WORD>, const std::vector<Shader::InstanceType>);
	~Mesh();

	void Render(ID3D11Device*, ID3D11DeviceContext*,
		Shader*, ID3D11RenderTargetView*, ID3D11DepthStencilState*,
		ID3D11DepthStencilView*, ID3D11RasterizerState*, D3D11_VIEWPORT*);

	static Mesh* LoadFromFile(ID3D11Device*, std::string, const DirectX::XMFLOAT3, const DirectX::XMFLOAT3, const DirectX::XMFLOAT3);

	void Move(float x, float y, float z, UINT index) { Move({ x, y, z }, index); }
	void Move(DirectX::XMFLOAT3, UINT index);
	void Rotate(float x, float y, float z, UINT index) { Rotate({ x, y, z }, index); }
	void Rotate(DirectX::XMFLOAT3, UINT index);
	void Scale(float x, float y, float z, UINT index) { Scale({ x, y, z }, index); }
	void Scale(DirectX::XMFLOAT3, UINT index);

	std::vector<Shader::VertexPosColor> GetVertices() const { return m_Vertices; }
	std::vector<WORD> GetIndicies() const { return m_Indicies; }

	DirectX::XMFLOAT3 GetPosition(UINT index) const { return m_Instances.at(index).Position; }
	DirectX::XMFLOAT3 GetRotation(UINT index) const { return m_rotation; }
	DirectX::XMFLOAT3 GetScale(UINT index) const { return m_scale; }

	void SetOrigin(DirectX::XMMATRIX pMatrix) { m_origin = pMatrix; }

	int GetVertexCount() const { return m_Vertices.size(); }
	int GetInstanceCount() const { return m_Instances.size(); }

	void SetInstanceCount(UINT count);
private:
	void CalculateNextMatrix();

	std::vector<Shader::VertexPosColor> m_Vertices;
	std::vector<WORD> m_Indicies;
	std::vector<Shader::InstanceType> m_Instances;

	ID3D11InputLayout* m_d3dInputLayout = nullptr;
	ID3D11Buffer* m_d3dVertexBuffer = nullptr;
	ID3D11Buffer* m_d3dIndexBuffer = nullptr;
	ID3D11Buffer* m_d3dInstanceBuffer = nullptr;

	DirectX::XMMATRIX m_origin;

	DirectX::XMFLOAT3 m_position = { 0, 0, 0 };
	DirectX::XMFLOAT3 m_rotation = { 0, 0, 0 };
	DirectX::XMFLOAT3 m_scale = { 1, 1, 1 };
};
