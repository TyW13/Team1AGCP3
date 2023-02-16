#pragma once

#include <DirectXMath.h>


// Represents an Axis-Aligned Bounding Box in 2D Space.
// Minimum and Maximum Point.
struct AABB2D
{
	DirectX::XMFLOAT2 min, max;

    // Checks whether two AABB's intersect.
    // SAT method = Seperating Axis Theorem 
    bool Intersects(const AABB2D& other) const
    {
        return !(max.x < other.min.x || max.y < other.min.y ||
            min.x > other.max.x || min.y > other.max.y);
    }
};


// Provides static functions for testing collisions.
class Collision2D
{
public:

    // Check for AABB vs AABB
    static bool AABBvsAABB(const AABB2D& a, const AABB2D& b)
    {
        return a.Intersects(b);
    }

    // Check between a Point and a AABB
    static bool PointvsAABB(const DirectX::XMFLOAT2& point, const AABB2D& aabb)
    {
        return (point.x >= aabb.min.x && point.x <= aabb.max.x &&
            point.y >= aabb.min.y && point.y <= aabb.max.y);
    }

    // Check between an AABB and a point
    static bool AABBvsPoint(const AABB2D& aabb, const DirectX::XMFLOAT2& point)
    {
        return PointvsAABB(point, aabb);
    }

};

