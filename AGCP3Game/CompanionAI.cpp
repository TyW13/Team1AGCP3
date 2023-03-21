#include "CompanionAI.h"
#include "DXSampleHelper.h"
#include "GameRenderer.h"



using namespace DirectX;



CompanionAI::CompanionAI(ID3D12Device* device)
{
    m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rotation = 0.0f;
    m_scale = 1.0f;
    //m_speechBubbleOffset = XMFLOAT3(0.0f, 40.0f, 0.0f);

  
}

void CompanionAI::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
    m_resourceDescriptors = std::make_unique<DescriptorHeap>(device,
    AIDescriptors::Count);

    ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();

    // Load Debug Dave from file
    std::wstring companionTexturePath = L"Data/DebugDave.dds";
    ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, companionTexturePath.c_str(), m_companionAITexture.ReleaseAndGetAddressOf()));

    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
        m_deviceResources->GetDepthBufferFormat());

    SpriteBatchPipelineStateDescription pd(rtState);
    m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

    XMUINT2 companionAISize = GetTextureSize(m_companiontexture.Get());

    m_origin.x = float(companionAISize.x / 2);
    m_origin.y = float(companionAISize.y / 2);
    
    auto uploadResourcesFinished = resourceUpload.End(
    m_deviceResources->GetCommandQueue());


}

void CompanionAI::Update(float deltaTime, const XMFLOAT3& playerPosition)
{
    // Update the position, velocity, rotation, etc. of the companion based on the player position and other game logic

    // Calculate the direction to the player
    XMVECTOR directionToPlayer = XMVectorSubtract(XMLoadFloat3(&playerPosition), XMLoadFloat3(&m_position));
    XMVECTOR normalizedDirection = XMVector3Normalize(directionToPlayer);

    // Update the velocity
    XMStoreFloat3(&m_velocity, XMVectorMultiply(normalizedDirection, XMVectorReplicate(m_moveSpeed)));

    // Update the position based on the velocity
    XMStoreFloat3(&m_position, XMVectorAdd(XMLoadFloat3(&m_position), XMVectorMultiply(XMLoadFloat3(&m_velocity), XMVectorReplicate(deltaTime))));

    // Update the rotation based on the velocity
    float angle = atan2f(m_velocity.x, m_velocity.z);
    m_rotation = angle - XM_PIDIV2;
}


void CompanionAI::Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap, UINT cbvOffset)
{
    // Set root signature
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());

    // Set constant buffer
    commandList->SetGraphicsRootConstantBufferView(0, m_constantBuffer->GetGPUVirtualAddress());

    // Set vertex buffer
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

    // Set index buffer
    commandList->IASetIndexBuffer(&m_indexBufferView);

    // Set pipeline state
    commandList->SetPipelineState(m_pipelineState.Get());

    // Set descriptor heap
    ID3D12DescriptorHeap* heaps[] = { cbvHeap };
    commandList->SetDescriptorHeaps(_countof(heaps), heaps);

    // Set descriptor table
    commandList->SetGraphicsRootDescriptorTable(1, cbvHeap->GetGPUDescriptorHandleForHeapStart());

    // Draw the companion
    commandList->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

    m_spriteBatch->Begin(commandList);


    m_spriteBatch->Draw(m_resourceDescriptors->GetGPUHandle(AIDescriptors::companionAI),


    m_spriteBatch->End();

}


//
//void CompanionAI::Speak(const wchar_t* message)
//{
//    // Create the speech bubble object
//    SpeechBubble bubble;
//    bubble.Initialize(m_device, message);
//
//    // Set the speech bubble's position relative to the companion's position
//    bubble.SetPosition(m_position + m_speechBubbleOffset);
//
//    // Add the speech bubble to the list of active speech bubbles
//    m_activeSpeechBubbles.push_back(bubble);
//}