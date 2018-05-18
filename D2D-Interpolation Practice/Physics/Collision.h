#pragma once
class Collision
{
public:
	Collision();
	~Collision();

	static bool IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 mousePos);
	static bool IntersectRayToLine(Ray ray, Vector2 start, Vector2 end, Vector2* outPos = NULL, Vector2* outNormal = NULL);
};

