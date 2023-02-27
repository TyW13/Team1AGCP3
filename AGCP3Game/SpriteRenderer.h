#pragma once

#include <memory>
#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"

class SpriteRenderer
{
public:
    SpriteRenderer();
    void Update(float deltaTime);
    void Render();

private:
    std::unique_ptr<Render> m_render;
    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Sprite> m_sprite;

};
