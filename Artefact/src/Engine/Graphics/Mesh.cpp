#include "ArtifactPCH.h"
#include "Mesh.h"
#include <fstream>
#include <sstream>

Mesh::Mesh(ID3D11Device* pD3DDevice, const std::vector<VertexPosColor> pVertex, const std::vector<WORD> pIndicies, const std::vector<InstanceType> pInstances) {
	m_Indicies = pIndicies;
	m_Vertices = pVertex;
	m_Instances = pInstances;

	// Create an initialize the vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * pVertex.size();
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = &pVertex[0];

	HRESULT hr = pD3DDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &m_d3dVertexBuffer);
	if (FAILED(hr)) { FatalAppExit(0, "Failed to create Vertex Buffer Descriptor"); }

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * pIndicies.size();
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	resourceData.pSysMem = &pIndicies[0];

	hr = pD3DDevice->CreateBuffer(&indexBufferDesc, &resourceData, &m_d3dIndexBuffer);
	if (FAILED(hr)) { FatalAppExit(0, "Index buffer"); }

	// Create and initialize the instance buffer
	D3D11_BUFFER_DESC instanceBufferDesc;
	ZeroMemory(&instanceBufferDesc, sizeof(D3D11_BUFFER_DESC));

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * pInstances.size();
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	resourceData.pSysMem = &pInstances[0];
	resourceData.SysMemPitch = 0;
	resourceData.SysMemSlicePitch = 0;

	hr = pD3DDevice->CreateBuffer(&instanceBufferDesc, &resourceData, &m_d3dInstanceBuffer);
	if (FAILED(hr)) { FatalAppExit(0, "Failed to initialize the instance buffer for Object"); }

	CalculateNextMatrix();
}

Mesh::~Mesh() {
	SafeRelease(m_d3dInputLayout);
	SafeRelease(m_d3dVertexBuffer);
	SafeRelease(m_d3dIndexBuffer);
	SafeRelease(m_d3dInstanceBuffer);
}

void Mesh::Render(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext, 
	Shader* shaders, ID3D11RenderTargetView* d3dRenderTargetView, ID3D11DepthStencilState* d3dDepthStencilState,
	ID3D11DepthStencilView* d3dDepthStencilView, ID3D11RasterizerState* d3dRasterizerState, D3D11_VIEWPORT* viewport) {
	shaders->Update(Shader::CB_Object, m_nextMatrix);

	const UINT vertexStride = sizeof(VertexPosColor);
	const UINT offset = 0;

	ID3D11Buffer* bufferPointers[2];
	bufferPointers[0] = m_d3dVertexBuffer;
	bufferPointers[1] = m_d3dInstanceBuffer;

	d3dDeviceContext->IASetVertexBuffers(0, 1, bufferPointers, &vertexStride, &offset);
	d3dDeviceContext->IASetInputLayout(shaders->GetInputLayout());
	d3dDeviceContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shaders->BindShaders(d3dRenderTargetView, d3dDepthStencilState, d3dDepthStencilView, d3dRasterizerState, viewport);
	d3dDeviceContext->DrawIndexedInstanced(m_Indicies.size(), m_Instances.size(), 0, 0, 0);
}

//Mesh* Mesh::LoadFromFile(ID3D11Device* device, std::string path, const DirectX::XMFLOAT3 pPosition, const DirectX::XMFLOAT3 pRotation, const DirectX::XMFLOAT3 pScale, int instanceCount) {
//	std::vector<VertexPosColor> vertices;
//	std::vector<WORD> indices;
//
//	std::ifstream in(path.c_str(), std::ios::in);
//	if (!in) {
//		std::cerr << "Cannot open " << path << std::endl;
//		exit(1);
//	}
//	std::string line;
//	while (std::getline(in, line)) {
//		//check v for vertices
//		if (line.substr(0, 2) == "v ") {
//			std::istringstream v(line.substr(2));
//			double x, y, z;
//			v >> x; v >> y; v >> z;
//			DirectX::XMFLOAT3 vert = DirectX::XMFLOAT3(x, y, z);
//			vertices.push_back({ vert, DirectX::XMFLOAT3(.0f, .0f, .0f) });
//		}
//		//check for faces
//		else if (line.substr(0, 2) == "f ") {
//			int a, b, c; //to store mesh index
//			int A, B, C; //to store texture index
//						 //std::istringstream v;
//						 //v.str(line.substr(2));
//			const char* chh = line.c_str();
//			sscanf_s(chh, "f %i/%i %i/%i %i/%i", &a, &A, &b, &B, &c, &C); //here it read the line start with f and store the corresponding values in the variables
//														//v>>a;v>>b;v>>c;
//			a--; b--; c--;
//			A--; B--; C--;
//
//			//std::cout<<a<<b<<c<<A<<B<<C;
//			indices.push_back(a);
//			indices.push_back(b);
//			indices.push_back(c);
//		}
//	}
//
//	return new Mesh(device, vertices, indices);
//}

void Mesh::SetInstanceCount(UINT count) {
	
}

void Mesh::CalculateNextMatrix() {
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_nextMatrix = XMMatrixMultiply(XMMatrixMultiply(translation, rotation), scale);
}

void Mesh::Move(DirectX::XMFLOAT3 vec) {
	m_position.x += vec.x;
	m_position.y += vec.y;
	m_position.z += vec.z;

	CalculateNextMatrix();
}

void Mesh::Rotate(DirectX::XMFLOAT3 vec) {
	m_rotation.x += vec.x;
	m_rotation.y += vec.y;
	m_rotation.z += vec.z;

	CalculateNextMatrix();
}

void Mesh::Scale(DirectX::XMFLOAT3 vec) {
	m_scale.x += vec.x;
	m_scale.y += vec.y;
	m_scale.z += vec.z;

	CalculateNextMatrix();
}