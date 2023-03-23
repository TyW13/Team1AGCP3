#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
	Bullet();
	virtual ~Bullet();

	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Draw(ID3D12GraphicsCommandList* commandList, ID3D12PipelineState* pipelineState, ID3D12DescriptorHeap* heap);


private:

	float m_speed;
};

