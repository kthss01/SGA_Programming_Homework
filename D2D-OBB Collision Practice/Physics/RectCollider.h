#pragma once

#include "CircleCollider.h"

class RectCollider : public CircleCollider
{
protected:
	Vector2 localMinPos;
	Vector2 localMaxPos;
public:
	RectCollider();
	~RectCollider();

	// OBB 박스를 가지고 오는거
	void GetWorldBox(const Transform* trans, Vector2* outBoxPos);
	// 회전값이 들어가지 않는 Rect
	// Rect에서 min x,y 값 제일 작은 녀석 max x,y 값 제일 큰 녀석 
	void GetWorldAABBMinMax(const Transform* trans,
		Vector2 * min, Vector2 * max);

	virtual void RenderGizmo(const Transform* trans) override;
	virtual void SetBound(
		const Vector2* pCenter, const Vector2* pHalfSize) override;
};
