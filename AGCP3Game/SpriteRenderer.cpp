//#include "SpriteRenderer.h"
//#include "Input.h"
//#include "DDSTextureLoader.h"
//
//SpriteRenderer::SpriteRenderer()
//{
//    // Create an instance of the Render class
//    m_render = std::make_unique<Render>();
//
//    // Create a texture for the sprite
//    m_texture = std::make_unique<Texture>();
//    m_texture->LoadFromFile("sprite.dds");
//
//    // Create a sprite with the texture
//    m_sprite = std::make_unique<Sprite>(m_texture.get());
//    m_sprite->SetPosition(100, 100);
//}
//
//void SpriteRenderer::Update(float deltaTime)
//{
//    // Handle input and update sprite position
//    if (Input::IsKeyDown('W'))
//    {
//        m_sprite->Move(0, -50 * deltaTime);
//    }
//    if (Input::IsKeyDown('S'))
//    {
//        m_sprite->Move(0, 50 * deltaTime);
//    }
//    if (Input::IsKeyDown('A'))
//    {
//        m_sprite->Move(-50 * deltaTime, 0);
//    }
//    if (Input::IsKeyDown('D'))
//    {
//        m_sprite->Move(50 * deltaTime, 0);
//    }
//}
//
//void SpriteRenderer::Render()
//{
//    // Clear the render target
//    m_render->Clear();
//
//    // Draw the sprite
//    m_render->DrawSprite(m_sprite.get());
//
//    // Present the frame
//    m_render->Present();
//}