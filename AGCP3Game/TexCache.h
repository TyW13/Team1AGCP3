#pragma once
#include "pch.h"
#include <unordered_map>


class TexCache
{
public:
	//associate a file name with a texture resource
	struct Data
	{
		Data() {}
		Data(const std::string& fName, Microsoft::WRL::ComPtr<ID3D12Resource> p, const DirectX::SimpleMath::Vector2& _dim)
			: fileName(fName), pTex(p), dim(_dim)
		{
			frames.clear();
		}
		Data(const std::string& fName, Microsoft::WRL::ComPtr<ID3D12Resource> p, const DirectX::SimpleMath::Vector2& _dim, const std::vector<RECT>* _frames)
			:fileName(fName), pTex(p), dim(_dim)

		{
			if (_frames)
				frames = *_frames;
		}
		std::string fileName;
		Microsoft::WRL::ComPtr<ID3D12Resource> pTex = nullptr;
		DirectX::SimpleMath::Vector2 dim;
		std::vector<RECT> frames;
	};

	//tidy up at the end
	void Release();
	//if this texture is new load it in, otherwise find it and return a handle
	Microsoft::WRL::ComPtr<ID3D12Resource> LoadTexture(ID3D12Device* pDevice, const std::string& fileName, const std::string& texName = "", bool appendPath = true, const std::vector<RECT>* _frames = nullptr);
	//usually we just have a texture file name, but they're all in a sub folder
	void SetAssetPath(const std::string& path) {
		mAssetPath = path;
	}
	//pull out a texture by nickname = fast
	Data& Get(const std::string& texName) {
		return mCache.at(texName);
	}
	//slowly find a texture by handle
	const Data& Get(Microsoft::WRL::ComPtr<ID3D12Resource> pTex);

private:
	DirectX::SimpleMath::Vector2 GetDimensions(Microsoft::WRL::ComPtr<ID3D12Resource> pTex);
	//array of texture data
	typedef std::unordered_map<std::string, Data> MyMap;
	MyMap mCache;

	//some data sub folder with all the textures in
	std::string mAssetPath;
};

