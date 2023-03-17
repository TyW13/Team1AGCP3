//#pragma once
//
//#include <DirectXMath.h>
//#include <d3d12.h>
//#include <wrl.h>
//
//using Microsoft::WRL::ComPtr;
//
//
//class CompanionAI
//{
//public :
//
//	CompanionAI(ID3D12Device* device);
//
//
//	void Update(float deltatime, const DirectX::XMFLOAT3& playerPosition);
//	void Render(ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap, UINT cbvOffset);
//
//private:
//
//	struct ConstantBuffer
//	{
//		DirectX::XMFLOAT4X4 worldMatrix;
//
//	};
//
//
//
//    ComPtr<ID3D12Resource> m_constantBuffer;
//    ConstantBuffer* m_mappedConstantBuffer;
//    ComPtr<ID3D12PipelineState> m_pipelineState;
//    ComPtr<ID3D12RootSignature> m_rootSignature;
//    ComPtr<ID3D12Resource> m_vertexBuffer;
//    ComPtr<ID3D12Resource> m_indexBuffer;
//    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
//    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
//    UINT m_indexCount;
//    DirectX::XMFLOAT3 m_position;
//    float m_rotation;
//    float m_scale;
//};

