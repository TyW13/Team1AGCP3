#pragma once

#include <DirectXMath.h>


struct AABB2D
{
	DirectX::XMFLOAT2 min, max;

    bool Intersects(const AABB2D& other) const
    {
        return !(max.x < other.min.x || max.y < other.min.y ||
            min.x > other.max.x || min.y > other.max.y);
    }
};

class Collision2D
{
public:
    static bool AABBvsAABB(const AABB2D& a, const AABB2D& b)
    {
        return a.Intersects(b);
    }

    static bool PointvsAABB(const DirectX::XMFLOAT2& point, const AABB2D& aabb)
    {
        return (point.x >= aabb.min.x && point.x <= aabb.max.x &&
            point.y >= aabb.min.y && point.y <= aabb.max.y);
    }

    static bool AABBvsPoint(const AABB2D& aabb, const DirectX::XMFLOAT2& point)
    {
        return PointvsAABB(point, aabb);
    }

};

