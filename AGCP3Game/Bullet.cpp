#include "Bullet.h"

Bullet::~Bullet()
{

}

void Bullet::Update(DeviceManager* dManager, ResourceManager* rManager, float dTime)
{
	// Move the bullet.

	DirectX::SimpleMath::Vector2 position = GetPosition();
	position += m_speed * GetVelocity() * dTime;
	SetPosition(position);
}

void Bullet::Draw(ID3D12GraphicsCommandList* commandList, ID3D12PipelineState* pipelineState, ID3D12DescriptorHeap* heap)
{
	// Draw the bullet.


}
