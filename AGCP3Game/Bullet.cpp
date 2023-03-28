#include "Bullet.h"




//Bullet::Bullet()
//{
//
//	// Speed
//
//	m_speed = 200.f;
//
//	m_velocity = DirectX::XMFLOAT2(0.0f, 0.0f);
//}
//Bullet::~Bullet()
//{
//
//}
//
//
//
//
//void Bullet::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
//{
//	//// Move the bullet.
//
//	//DirectX::SimpleMath::Vector2 position = GetPosition();
//	//position += m_speed * GetVelocity() * dTime;
//	//SetPosition(position);
//
//	// Update the position of the bullet based on its velocity
//	DirectX::XMFLOAT3 position = GetPosition();
//	position.x += m_velocity.x * dTime;
//	position.y += m_velocity.y * dTime;
//
//	SetPosition(position);
//}
//
//void Bullet::Draw(ID3D12GraphicsCommandList* commandList, ID3D12PipelineState* pipelineState, ID3D12DescriptorHeap* heap)
//{
//	// Draw the bullet.
//
//
//}
//
//void Bullet::SetVelocity(float x, float y)
//{
//	m_velocity = DirectX::XMFLOAT2(x, y);
//}
//
//DirectX::XMFLOAT2 Bullet::GetVelocity() const
//{
//	return m_velocity;
//}
