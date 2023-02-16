#include "stdafx.h"
#include "EntityManager.h"


#include "EntityManager.h"

void EntityManager::AddEntity(std::unique_ptr<Entity>&& entity) {
    entities_.push_back(std::move(entity));
}

void EntityManager::UpdateEntities(float delta) {
    for (auto& entity : entities_) {
        entity->Update(delta);
    }
}

void EntityManager::DrawEntities(ID3D12GraphicsCommandList* commandList) {
    for (auto& entity : entities_) {
        entity->Draw(commandList);
    }
}