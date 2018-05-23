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
		if (min->y > worldPos[i].y)
			min->y = worldPos[i].y;

		if (max->x < worldPos[i].x)
			max->x = worldPos[i].x;
		if (max->y < worldPos[i].y)
			max->y = worldPos[i].y;
	}
}

void RectCollider::RenderGizmo(const Transform * trans)
{
	Vector2 worldPos[4];
	this->GetWorldBox(trans, worldPos);

	// OBB 박스
	// 원래 기즈모 물체에 씌워주는 색은 초록색임
	GIZMO->Line(worldPos[0], worldPos[1], 0xffffff00);
	GIZMO->Line(worldPos[1], worldPos[2], 0xffffff00);
	GIZMO->Line(worldPos[2], worldPos[3], 0xffffff00);
	GIZMO->Line(worldPos[3], worldPos[0], 0xffffff00);

	// AABB 박스
	Vector2 minPos;
	Vector2 maxPos;

	this->GetWorldAABBMinMax(trans, &minPos, &maxPos);

	// 0xff808000 - 노란색인데 약간 어두운 노란색임
	GIZMO->AABBBox(minPos, maxPos, 0xff808000);
}

void RectCollider::SetBound(const Vector2 * pCenter, const Vector2 * pHalfSize)
{
	CircleCollider::SetBound(pCenter, pHalfSize);
	this->localMinPos = this->localCenter - this->halfSize;
	this->localMaxPos = this->localCenter + this->halfSize;
}

void RectCollider::RenderGizmoSpecialRed(Transform * trans)
{
	Vector2 min, max;
	this->GetWorldAABBMinMax(trans, &min, &max);

	Vector2 pos = trans->GetWorldPosition();
	Vector2 vertices[6];
	float radius = max.y - min.y;

	for (int i = 0; i < 6; i++) {
		vertices[i].x = pos.x + cosf(2.0f * D3DX_PI / 6 * i - 30.0f / 180.0f * D3DX_PI) * radius;
		vertices[i].y = pos.y - sinf(2.0f * D3DX_PI / 6 * i - 30.0f / 180.0f * D3DX_PI) * radius;
	}

	for (int i = 0; i <= 4; i += 2) {
		GIZMO->Line(vertices[i], vertices[(i + 2) % 6], 0xffff0000);
		GIZMO->Line(vertices[i + 1], vertices[(i + 2) % 6 + 1], 0xfff0000);
	}
	GIZMO->Circle(pos, radius, 0xffff0000);
}

void RectCollider::RenderGizmoSpecialGreen(Transform * trans)
{
	Vector2 min, max;
	this->GetWorldAABBMinMax(trans, &min, &max);

	Vector2 pos = trans->GetWorldPosition();
	Vector2 vertices[10];
	float radius = max.y - min.y;
	float subRadius = radius / 12 * 7;

	vertices[0].x = pos.x + cosf(2.0f * D3DX_PI / 6 - 2.0f * D3DX_PI / 12) * radius;
	vertices[0].y = pos.y - sinf(2.0f * D3DX_PI / 6 - 2.0f * D3DX_PI / 12) * radius;
	
	vertices[1].x = pos.x + cosf(2.0f * D3DX_PI / 2 - 2.0f * D3DX_PI / 12) * radius;
	vertices[1].y = pos.y - sinf(2.0f * D3DX_PI / 2 - 2.0f * D3DX_PI / 12) * radius;
	
	vertices[2].x = pos.x + cosf(2.0f * D3DX_PI / 3 * 2 - 2.0f * D3DX_PI / 12) * radius;
	vertices[2].y = pos.y - sinf(2.0f * D3DX_PI / 3 * 2 - 2.0f * D3DX_PI / 12) * radius;

	vertices[3].x = pos.x + cosf(-2.0f * D3DX_PI / 12) * radius;
	vertices[3].y = pos.y - sinf(-2.0f * D3DX_PI / 12) * radius;

	for (int i = 4; i < 10; i++) {
		vertices[i].x = pos.x + cosf(2.0f * D3DX_PI / 6 * (i-4)) * subRadius;
		vertices[i].y = pos.y - sinf(2.0f * D3DX_PI / 6 * (i-4)) * subRadius;
	}

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			GIZMO->Line(vertices[i], vertices[i + 4], 0xff00ff00);
			GIZMO->Line(vertices[i], vertices[i + 5], 0xff00ff00);
		}
		else if (i == 3) {
			GIZMO->Line(vertices[i], vertices[i + 1], 0xff00ff00);
			GIZMO->Line(vertices[i], vertices[i + 6], 0xff00ff00);
		}
		else {
			GIZMO->Line(vertices[i], vertices[i + 5], 0xff00ff00);
			GIZMO->Line(vertices[i], vertices[i + 6], 0xff00ff00);
		}
	}
	for (int i = 4; i < 7; i++) {
		GIZMO->Line(vertices[i], vertices[i + 3], 0xff00ff00);
	}
	GIZMO->Circle(pos, radius, 0xff00ff00);
}

void RectCollider::RenderGizmoSpecialBlue(Transform * trans)
{
	Vector2 min, max;
	this->GetWorldAABBMinMax(trans, &min, &max);

	Vector2 pos = trans->GetWorldPosition();
	Vector2 vertices[8];
	float radius = max.y - min.y;

	//for (int i = 0; i < 4; i++) {
	//	vertices[i].x = pos.x + cosf(2.0f * D3DX_PI / 4 * i + 45.0f / 180.0f * D3DX_PI) * radius;
	//	vertices[i].y = pos.y - sinf(2.0f * D3DX_PI / 4 * i + 45.0f / 180.0f * D3DX_PI) * radius;
	//}

	float angle = 30.0f;

	vertices[0].x = pos.x + cosf(angle / 180.0f * D3DX_PI) * radius;
	vertices[0].y = pos.y - sinf(angle / 180.0f * D3DX_PI) * radius;

	vertices[1].x = pos.x + cosf(D3DX_PI - angle / 180.0f * D3DX_PI) * radius;
	vertices[1].y = pos.y - sinf(D3DX_PI - angle / 180.0f * D3DX_PI) * radius;

	vertices[2].x = pos.x + cosf(D3DX_PI + angle / 180.0f * D3DX_PI) * radius;
	vertices[2].y = pos.y - sinf(D3DX_PI + angle / 180.0f * D3DX_PI) * radius;

	vertices[3].x = pos.x + cosf(-angle / 180.0f * D3DX_PI) * radius;
	vertices[3].y = pos.y - sinf(-angle / 180.0f * D3DX_PI) * radius;


	float temp = fabs(vertices[0].x - vertices[1].x) / 3;

	vertices[4].x = vertices[0].x - temp;
	vertices[4].y = vertices[0].y;

	vertices[5].x = vertices[1].x + temp;
	vertices[5].y = vertices[1].y;

	vertices[6].x = vertices[2].x + temp;
	vertices[6].y = vertices[2].y;

	vertices[7].x = vertices[3].x - temp;
	vertices[7].y = vertices[3].y;

	GIZMO->Line(vertices[0], vertices[1], 0xff0000ff);
	GIZMO->Line(vertices[2], vertices[3], 0xff0000ff);

	GIZMO->Line(vertices[0], vertices[7], 0xff0000ff);
	GIZMO->Line(vertices[1], vertices[6], 0xff0000ff);
	GIZMO->Line(vertices[2], vertices[5], 0xff0000ff);
	GIZMO->Line(vertices[3], vertices[4], 0xff0000ff);
	
	GIZMO->Line(vertices[4], vertices[6], 0xff0000ff);
	GIZMO->Line(vertices[5], vertices[7], 0xff0000ff);

	GIZMO->Circle(pos, radius, 0xff0000ff);
}
