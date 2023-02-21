#pragma once

#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>


#include "AGCP3.h"



using namespace DirectX;

struct Vertex
{
   int m_vertices;
};

class GameObject
{
public:
    GameObject();
    ~GameObject();

    bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap,
        int textureIndex, int numVertices, int numIndices, Vertex* vertices, DWORD* indices);
    void Update(float deltaTime);
    void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap, ID3D12RootSignature* rootSignature,
        ID3D12PipelineState* pipelineState, DirectX::XMMATRIX viewProjectionMatrix);

    void SetPosition(float x, float y);
    void SetPosition(DirectX::XMFLOAT2 position);
    void SetRotation(float angle);
    void SetScale(float scale);

    DirectX::XMFLOAT2 GetPosition() const { return m_position; }
    float GetRotation() const { return m_rotation; }
    float GetScale() const { return m_scale; }

protected:
    struct VertexBufferView
    {
        D3D12_VERTEX_BUFFER_VIEW view;
        UINT stride;
        UINT sizeInBytes;
    };
    VertexBufferView m_vertexBufferView;

    struct IndexBufferView
    {
        D3D12_INDEX_BUFFER_VIEW view;
        UINT sizeInBytes;
    };
    IndexBufferView m_indexBufferView;

    int m_textureIndex;
    ID3D12Resource* m_texture = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE m_textureDescriptorHandle;

    XMFLOAT2 m_position;
    float m_rotation;
    float m_scale;

    XMFLOAT4X4 m_worldMatrix;

    void CreateTexture(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* descriptorHeap);
    void CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, Vertex* vertices, int numVertices);
    void CreateIndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, DWORD* indices, int numIndices);

private:
    ID3D12Resource* m_vertexBuffer = nullptr;
    ID3D12Resource* m_vertexBufferUploadHeap = nullptr;
    ID3D12Resource* m_indexBuffer = nullptr;
    ID3D12Resource* m_indexBufferUploadHeap = nullptr;

    int m_numVertices;
    int m_numIndices;

    void UpdateWorldMatrix();
};



//using namespace Microsoft::WRL;
//using namespace DirectX;
//
//struct Vertex2D
//{
//    XMFLOAT3 position;
//    XMFLOAT2 texCoord;
//};
//
//class Player
//{
//public:
//
//	// Player(MyD3D& d3d) //DX11
//	Player(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState,
//		D3D12_VERTEX_BUFFER_VIEW vertexBufferView, D3D12_INDEX_BUFFER_VIEW indexBufferView, int textureIndex);
//
//
//	//Sprite character;		//jet
//	
//	void Init(ID3D12Device* device);
//    void Update(float deltaTime, bool moveLeft, bool moveRight, bool jump);
//
//private:
//    void ApplyGravity(float deltaTime);
//    void ApplyMovement(float deltaTime, bool moveLeft, bool moveRight);
//    void ApplyJump(float deltaTime, bool jump);
//
//    bool m_isOnGround;
//    bool m_isJumping;
//    float m_jumpVelocity;
//    float m_jumpDuration;
//    float m_elapsedJumpTime;
//    float m_horizontalVelocity;
//    float m_fallVelocity;
//};


//class GameObject
//{
//public:
//    GameObject();
//    virtual ~GameObject();
//
//    // Initialize the GameObject
//    virtual void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
//    // Update the GameObject
//    virtual void Update(float deltaTime);
//    // Render the GameObject
//    virtual void Render(ID3D12GraphicsCommandList* commandList);
//
//    // Set the Position of the GameObject
//    void SetPosition(float x, float y, float z = 0.0f);
//    // Set the Velocity of the GameObject
//    void SetVelocity(float x, float y);
//    // Set the Scale of the GameObject
//    void SetScale(float scale);
//    // Set the Texture of the GameObject
//    void SetTexture(ID3D12Resource* texture);
//
//    // Set Index Buffer
//    void SetIndexBuffer(const unsigned short* indices, int count);
//
//    void SetVertexBuffer(Vertex2D* vertices, int count);
//
//protected:
//    XMFLOAT3 m_position;
//    XMFLOAT3 m_velocity;
//    XMFLOAT3 m_rotation;
//
//    XMFLOAT2 m_size;
//    ID3D12Resource* m_texture;
//    ID3D12Resource* m_vertexBuffer;
//    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
//    ID3D12Resource* m_indexBuffer;
//    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
//    int m_indexCount;
//};
