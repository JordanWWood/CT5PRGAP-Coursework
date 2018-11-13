#include "ArtifactPCH.h"
#include "Mesh.h"
#include <comdef.h>

namespace Artefact {
    Mesh::Mesh(ID3D11Device* pD3DDevice, const std::vector<Shader::VertexPosColor> pVertex, const std::vector<WORD> pIndicies, const std::vector<Shader::InstanceType> pInstances) {
        m_Indicies = pIndicies;
        m_Vertices = pVertex;
        m_Instances = pInstances;

        // Create an initialize the vertex buffer.
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.ByteWidth = sizeof(Shader::VertexPosColor) * pVertex.size();
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

        instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        instanceBufferDesc.ByteWidth = sizeof(Shader::InstanceType) * pInstances.size();
        instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        instanceBufferDesc.MiscFlags = 0;
        instanceBufferDesc.StructureByteStride = 0;

        resourceData.pSysMem = &pInstances[0];

        hr = pD3DDevice->CreateBuffer(&instanceBufferDesc, &resourceData, &m_d3dInstanceBuffer);
        if (FAILED(hr)) { FatalAppExit(0, "Failed to initialize the instance buffer for Object"); }

        // Set the object origin to the world origin using an Identity matrix. This can be changed by the SetOrigin function
        m_origin = DirectX::XMMatrixIdentity();
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
        // If the origin has been moved then move it
        shaders->Update(Shader::CB_Object, m_origin);

        // Set instance data by mapping the data within the buffer to a variable and replacing it. This variable is then unmapped whilst not being used
        D3D11_MAPPED_SUBRESOURCE resource;
        d3dDeviceContext->Map(m_d3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
        memcpy(resource.pData, &m_Instances[0], sizeof(Shader::InstanceType) * m_Instances.size());
        d3dDeviceContext->Unmap(m_d3dInstanceBuffer, 0);

        // Notify the GPU how far apart each piece of data is.
        UINT vertexStride[2];
        vertexStride[0] = sizeof(Shader::VertexPosColor);
        vertexStride[1] = sizeof(Shader::InstanceType);

        // Currently there is no need for an offset so they are set to zero.
        UINT offset[2];
        offset[0] = 0;
        offset[1] = 0;

        // Create an array of pointers to the buffers in memory so the GPU knows where to get its data
        ID3D11Buffer* bufferPointers[2];
        bufferPointers[0] = m_d3dVertexBuffer;
        bufferPointers[1] = m_d3dInstanceBuffer;

        d3dDeviceContext->IASetVertexBuffers(0, 2, bufferPointers, vertexStride, offset);
        d3dDeviceContext->IASetInputLayout(shaders->GetInputLayout());
        d3dDeviceContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        shaders->BindShaders(d3dRenderTargetView, d3dDepthStencilState, d3dDepthStencilView, d3dRasterizerState, viewport);
        d3dDeviceContext->DrawIndexedInstanced(m_Indicies.size(), m_Instances.size(), 0, 0, 0);
    }

    void Mesh::Move(DirectX::XMFLOAT3 vec, UINT index) {
        m_Instances.at(index).Position.x = vec.x;
        m_Instances.at(index).Position.y = vec.y;
        m_Instances.at(index).Position.z = vec.z;
    }

    //TODO Update
    void Mesh::Rotate(DirectX::XMFLOAT3 vec, UINT index) {
        m_Instances.at(index).Rotaion.x = vec.x;
        m_Instances.at(index).Rotaion.y = vec.y;
        m_Instances.at(index).Rotaion.z = vec.z;
    }

    void Mesh::Scale(DirectX::XMFLOAT3 vec, UINT index) {
        m_scale.x += vec.x;
        m_scale.y += vec.y;
        m_scale.z += vec.z;
    }
}