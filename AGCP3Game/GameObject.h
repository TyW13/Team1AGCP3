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
    GameObject(float x, float y, float width, float height);
    virtual ~GameObject();

    // Getters and setters
    float GetX() const { return m_x; }
    void SetX(float x) { m_x = x; }
    float GetY() const { return m_y; }
    void SetY(float y) { m_y = y; }
    float GetWidth() const { return m_width; }
    void SetWidth(float width) { m_width = width; }
    float GetHeight() const { return m_height; }
    void SetHeight(float height) { m_height = height; }

    // Update and render
    virtual void Update(float deltaTime);
    virtual void Render();

    // Check for collision with another game object
    bool IsColliding(GameObject* other);

protected:
    float m_x, m_y;
    float m_width, m_height;
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
