#pragma once


class Entity {
public:
    Entity() {}
    virtual ~Entity() {}
    virtual void Update(float delta) = 0;
    virtual void Draw(ID3D12GraphicsCommandList* commandList) = 0;
};
