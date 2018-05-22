#include "stdafx.h"
#include "RectCollider.h"


RectCollider::RectCollider()
{
}


RectCollider::~RectCollider()
{
}

void RectCollider::GetWorldBox(const Transform * trans, Vector2* outBoxPos)
{
	Vector2 vertices[4];

	// 왼쪽하단
	vertices[0] = Vector2(this->localMinPos.x, this->localMaxPos.y);
	// 왼쪽상단
	vertices[1] = Vector2(this->localMinPos.x, this->localMinPos.y);
	// 오른쪽상단
	vertices[2] = Vector2(this->localMaxPos.x, this->localMinPos.y);
	// 오른쪽하단
	vertices[3] = Vector2(this->localMaxPos.x, this->localMaxPos.y);

	Matrix matFinal = trans->GetFinalMatrix();

	for (int i = 0; i < 4; i++)
		outBoxPos[i] = vertices[i].TransformCoord(matFinal);
}

void RectCollider::GetWorldAABBMinMax(const Transform * trans, Vector2 * min, Vector2 * max)
{
	Vector2 worldPos[4];
	GetWorldBox(trans, worldPos);

	*min = worldPos[0];
	*max = worldPos[0];

	for (int i = 1; i < 4; i++) {
		if (min->x > worldPos[i].x)
			min->x = worldPos[i].x;
		else if (min->y > worldPos[i].y)
			min->y = worldPos[i].y;

		if (max->x < worldPos[i].x)
			max->x = worldPos[i].x;
		else if (max->y < worldPos[i].y)
			max->y = worldPos[i].y;
	}
}

void RectCollider::RenderGizmo(const Transform * trans)
{
}

void RectCollider::SetBound(const Vector2 * pCenter, const Vector2 * pHalfSize)
{
	CircleCollider::SetBound(pCenter, pHalfSize);
	this->localMinPos = this->localCenter - this->halfSize;
	this->localMaxPos = this->localCenter + this->halfSize;
}
