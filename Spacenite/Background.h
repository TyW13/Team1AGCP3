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
	Background(MyD3D& d3d)
		:bGroundTest{d3d,d3d,d3d,d3d,d3d,d3d,d3d,d3d}
	{
	}
	std::vector<Sprite> bGround;
	Sprite bGroundTest[2][4];
	void Init(MyD3D& d3d);
	void Update(float dTime, DirectX::SimpleMath::Vector2& mPos);
	void Render(DirectX::SpriteBatch& batch);
	void Increase();
	void CheckBounds(DirectX::SimpleMath::Vector2& mPos);

private:
	const float SCROLL_SPEED = 10.f;
	static const int BGND_LAYERS = 8;
	int currentx = 0;
	int currenty = 0;
	bool IsTop = false;
	bool IsBottom = false;
	bool IsLeft = false;
	bool IsRight = false;
	bool IsMaxY = false;
	bool IsMinY = false;
	bool IsMinX = false;
	bool IsMaxX = false;
};


#endif
