#include "Background.h"
using namespace std;
void Background::Init(MyD3D& mD3D)
{
	//a sprite for each layer
	assert(bGround.empty());
	bGround.insert(bGround.begin(), BGND_LAYERS, Sprite(mD3D));

	//a neat way to package pairs of things (nicknames and filenames)
	pair<string, string> files[BGND_LAYERS]{
		{ "bgnd0","backgroundlayers/mountains01_007.dds" },
		{ "bgnd1","backgroundlayers/mountains01_005.dds" },
		{ "bgnd2","backgroundlayers/mountains01_004.dds" },
		{ "bgnd3","backgroundlayers/mountains01_003.dds" },
		{ "bgnd4","backgroundlayers/mountains01_002.dds" },
		{ "bgnd5","backgroundlayers/mountains01_001.dds" },
		{ "bgnd6","backgroundlayers/mountains01_000.dds" },
		{ "bgnd7","backgroundlayers/mountains01_006.dds" }
	};
	int i = 0;
	for (auto& f : files)
	{
		//set each texture layer
		ID3D11ShaderResourceView* p = mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), f.second, f.first);
		if (!p)
			assert(false);
		bGround[i++].SetTex(*p);
	}
}

void Background::Update(float dTime)
{

	bGround[current].Scroll(dTime * SCROLL_SPEED, 0);
}

void Background::Render(DirectX::SpriteBatch& batch)
{
	bGround[current].Draw(batch);
}
