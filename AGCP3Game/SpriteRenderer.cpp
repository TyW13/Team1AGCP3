//#include "SpriteRenderer.h"
//#include "Game.h"
//#include "Sprite.h"
//#include <DescriptorHeap.h>
//#include "pch.h"
//#include "DirectXTex.h"
//#include "DDSTextureLoader.h"
//#include "SpriteRenderer.h"
//
//#include "GraphicsMemory.h"
//
//
//using namespace DirectX;
//using namespace DirectX::SimpleMath;
//
//
//
//using Microsoft::WRL::ComPtr;
//
//
//
//void SpriteRenderer::Initialize(ID3D12Device* m_device)
//{
//
//}
//
//void SpriteRenderer::Render()
//{
//
//	// Don't try to render anything before the first Update.
//	if (m_timer.GetFrameCount() == 0)
//	{
//		return;
//	}
//
//
//	// Prepare the command list to render a new frame.
//	m_deviceResources->Prepare();
//
//	auto commandList = m_deviceResources->GetCommandList();
//
//
//
//	float time = float(m_timer.GetTotalSeconds());
//
//	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
//	commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);
//	m_spriteBatch->Begin(commandList);
//
//	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Background),
//		GetTextureSize(m_background.Get()),
//		m_fullscreenRect);
//
//	m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
//		GetTextureSize(m_texture.Get()),
//		m_screenPos, nullptr, Colors::White, 0.f, m_origin);
//
//	m_spriteBatch->End();
//
//
//	// Show the new frame.
//	m_deviceResources->Present();
//	m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
//}
//
//
//void SpriteRenderer::LoadFromFile()
//{
//	
//	auto device = m_deviceResources->GetD3DDevice();
//
//
//	ResourceUploadBatch resourceUpload(device);
//
//	resourceUpload.Begin();
//
//
//	DX::ThrowIfFailed(
//		DirectX::CreateDDSTextureFromFile(device, resourceUpload, L"cat.dds",
//			m_texture.ReleaseAndGetAddressOf()));
//
//	CreateShaderResourceView(device, m_texture.Get(),
//		m_resourceDescriptors->GetCpuHandle(Descriptors::Cat));
//
//	DX::ThrowIfFailed(
//		CreateWICTextureFromFile(device, resourceUpload, L"sunset.jpg",
//			m_background.ReleaseAndGetAddressOf()));
//
//	// BACKGROUND
//
//	CreateShaderResourceView(device, m_background.Get(),
//		m_resourceDescriptors->GetCpuHandle(Descriptors::Background));
//
//
//	RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
//		m_deviceResources->GetDepthBufferFormat());
//
//
//
//	auto sampler = m_states->LinearWrap();
//	SpriteBatchPipelineStateDescription pd(rtState);
//	m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);
//
//	XMUINT2 catSize = GetTextureSize(m_texture.Get());
//
//	m_origin.x = float(catSize.x / 2);
//	m_origin.y = float(catSize.y / 2);
//
//
//	//m_origin.x = float(catSize.x * 2);
//	//m_origin.y = float(catSize.y * 2);
//
//	m_tileRect.left = catSize.x * 2;
//	m_tileRect.right = catSize.x * 6;
//	m_tileRect.top = catSize.y * 2;
//	m_tileRect.bottom = catSize.y * 6;
//
//	auto uploadResourcesFinished = resourceUpload.End(
//	m_deviceResources->GetCommandQueue());
//
//	uploadResourcesFinished.wait();
//
//}
//
//
//void SpriteRenderer::Update()
//{
//
//}
//
//
//
//void SpriteRenderer::OnDeviceLost()
//{
//	m_texture.Reset();
//	m_resourceDescriptors.reset();
//	m_spriteBatch.reset();
//	m_background.Reset();
//}
//
//
//void SpriteRenderer::Draw()
//{
//
//}
//
//
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

