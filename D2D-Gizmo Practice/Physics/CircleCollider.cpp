#include "stdafx.h"
#include "CircleCollider.h"


CircleCollider::CircleCollider()
{
	// 충돌 구체를 어디에 붙일지 
	transform = new Transform;
}


CircleCollider::~CircleCollider()
{
	SAFE_DELETE(transform);
}

// 외부에 반환해주는 용도
void CircleCollider::GetWorldCenterRadius(const Transform * trans, Vector2 * center, float * radius)
{
	Matrix matFinal = trans->GetFinalMatrix();

	*center = this->localCenter.TransformCoord(matFinal);
	Vector2 scale = trans->GetScale();
	Vector2 halfSize;
	halfSize.x = this->halfSize.x * scale.x;
	halfSize.y = this->halfSize.y * scale.y;

	*radius = halfSize.Length();
}

void CircleCollider::RenderGizmo(const Transform * trans)
{
	Vector2 center;
	float radius;

	this->GetWorldCenterRadius(trans, &center, &radius);

	GIZMO->Circle(center, radius, 0xffffff00);
}

// 외부에서 세팅하는 용도
void CircleCollider::SetBound(const Vector2 * pCenter, const Vector2 * pHalfSize)
{
	this->localCenter = *pCenter;
	this->halfSize = *pHalfSize;
	this->radius = pHalfSize->Length();
}
