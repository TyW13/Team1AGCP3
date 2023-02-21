#include "stdafx.h"

#include "GameObject.h"
#include "DDSTextureLoader.h"

GameObject::GameObject(float x, float y, float width, float height)
{
}

GameObject::~GameObject()
{
}

bool GameObject::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, int textureIndex, int numVertices, int numIndices, Vertex* vertices, DWORD* indices)
{
	return false;
}

void GameObject::Update(float deltaTime)
{

}

void GameObject::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState, DirectX::XMMATRIX viewProjectionMatrix)
{

}

void GameObject::SetPosition(float x, float y)
{

}

void GameObject::SetPosition(DirectX::XMFLOAT2 position)
{

}

void GameObject::SetRotation(float angle)
{

}

void GameObject::SetScale(float scale)
{

}

bool GameObject::isColliding(GameObject* other)
{
    float left1 = m_x - m_width / 2.0f;
    float right1 = m_x + m_width / 2.0f;
    float top1 = m_y - m_height / 2.0f;
    float bottom1 = m_y + m_height / 2.0f;

    float left2 = other->GetX() - other->GetWidth() / 2.0f;
    float right2 = other->GetX() + other->GetWidth() / 2.0f;
    float top2 = other->GetY() - other->GetHeight() / 2.0f;
    float bottom2 = other->GetY() + other->GetHeight() / 2.0f;

    return !(left1 > right2 || left2 > right1 || top1 > bottom2 || top2 > bottom1);
}

void GameObject::CreateTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap)
{

}

void GameObject::CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, Vertex* vertices, int numVertices)
{

}

void GameObject::CreateIndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, DWORD* indices, int numIndices)
{
}

void GameObject::UpdateWorldMatrix()
{

}
