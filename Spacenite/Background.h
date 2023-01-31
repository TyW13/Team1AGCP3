#if !defined(BackgroundH)
#define BackgroundH

#include <vector>
#include "Sprite.h"
/// <summary>
/// The class for the Paralax scrolling background with all that needs to be public is public
/// The rest is private. The Background is a composition member of Playmode and is exclusively used by it.
/// </summary>
class Background
{
public:
	std::vector<Sprite> bGround;
	void Init(MyD3D& d3d);
	void Update(float dTime);
	void Render(DirectX::DX11::SpriteBatch& batch);

private:
	const float SCROLL_SPEED = 10.f;
	static const int BGND_LAYERS = 8;
};


#endif
