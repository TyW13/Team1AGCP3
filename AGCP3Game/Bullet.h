#pragma once
#include "GameObject.h"
#include "AITurret.h"

class Bullet : public GameObject
{
	Bullet();
	virtual ~Bullet();

	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Draw(ID3D12GraphicsCommandList* commandList, ID3D12PipelineState* pipelineState, ID3D12DescriptorHeap* heap);


	

private:

	float m_speed;
	/*int m_bulletVelocity;*/

	DirectX::SimpleMath::Vector2 m_velocity;
	DirectX::SimpleMath::Vector2 m_bulletVelocity;
};

