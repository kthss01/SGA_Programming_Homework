#pragma once

#include "./Common/Transform.h"

// collider 충돌 계산 해줄 영역
// 원은 사각형 바깥쪽에 있는 거

class CircleCollider
{
protected:
	Transform * transform;
	Vector2 localCenter;
	float radius;
	Vector2 halfSize;
public:
	CircleCollider();
	~CircleCollider();

	// 외부에 반환해주는 용도
	void GetWorldCenterRadius(
		const Transform* trans, Vector2* center, float* radius);
	virtual void RenderGizmo(const Transform* trans);
	// 외부에서 세팅하는 용도
	virtual void SetBound(
		const Vector2* pCenter, const Vector2* pHalfSize);
};

