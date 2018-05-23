#include "stdafx.h"
#include "Gizmo.h"

Gizmo* Gizmo::instance = NULL;

Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

Gizmo * Gizmo::Get()
{
	if (instance == NULL)
		instance = new Gizmo;
	return instance;
}

void Gizmo::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

// 모델이 이동하거나 회전하더라도 중앙(모델의)에 붙어있어야 함
void Gizmo::Line(const Vector2 & startPos, const Vector2 & endPos, DWORD color) const
{
	D3DXMATRIX matPrevWorld;
	D2D::GetDevice()->GetTransform(D3DTS_WORLD, &matPrevWorld);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matIdentity);

	// light에 영향도 받으면 안됨
	// 기즈모가 가지고 있는 색상도 다 의미가 있음
	// x(red), y(green), z(blue)로 사용하기로 암묵적으로 되어있음 
	DWORD prevLight = 0;


	D2D::GetDevice()->GetRenderState(D3DRS_LIGHTING, &prevLight);

	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	GIZMOVERTEX vertices[2];
	vertices[0].position = startPos;
	vertices[1].position = endPos;
	vertices[0].color = color;
	vertices[1].color = color;

	// 그리기
	// 실질적으론 이렇게 그리면 프레임 저하가 심해짐
	// 초기화 업데이트 렌더 3개로 분할해서 할 수 있게
	// 지금은 세개 다 같이 하고 있음 (매 프레임마다 다 해버려서)
	D2D::GetDevice()->SetFVF(GIZMOVERTEX::FVF);
	D2D::GetDevice()->DrawPrimitiveUP(
		D3DPT_LINELIST, 1, vertices, sizeof(GIZMOVERTEX));

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matPrevWorld);
	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void Gizmo::Circle(Vector2 & centerPos, float radius, DWORD color) const
{
	// +1 하는 이유는 for문 돌릴 때 마지막 하나가 연결이 안되서 한번 더 하는거
	Vector2 circlePos[CIRCLEGIZMO_SEGMENTS + 1];

	D3DXMATRIX matPrevWorld;
	D2D::GetDevice()->GetTransform(D3DTS_WORLD, &matPrevWorld);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matIdentity);

	// light에 영향도 받으면 안됨
	// 기즈모가 가지고 있는 색상도 다 의미가 있음
	// x(red), y(green), z(blue)로 사용하기로 암묵적으로 되어있음 
	DWORD prevLight = 0;

	D2D::GetDevice()->GetRenderState(D3DRS_LIGHTING, &prevLight);

	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	GIZMOVERTEX vertices[CIRCLEGIZMO_SEGMENTS + 1];

	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++) {
		float angle = 2.0f * D3DX_PI / CIRCLEGIZMO_SEGMENTS * i;
		circlePos[i].x = cosf(angle) * radius;
		circlePos[i].y = -sinf(angle) * radius;
		vertices[i].position = centerPos + circlePos[i];
		vertices[i].color = color;
	}

	// 그리기
	// 실질적으론 이렇게 그리면 프레임 저하가 심해짐
	// 초기화 업데이트 렌더 3개로 분할해서 할 수 있게
	// 지금은 세개 다 같이 하고 있음 (매 프레임마다 다 해버려서)
	D2D::GetDevice()->SetFVF(GIZMOVERTEX::FVF);


	D2D::GetDevice()->DrawPrimitiveUP(
		D3DPT_LINESTRIP, CIRCLEGIZMO_SEGMENTS, vertices, sizeof(GIZMOVERTEX));

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matPrevWorld);
	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void Gizmo::AABBBox(const Vector2 & minPos, const Vector2 & maxPos, DWORD color) const
{
	this->Line(Vector2(minPos.x, minPos.y), Vector2(maxPos.x, minPos.y), color);
	this->Line(Vector2(maxPos.x, minPos.y), Vector2(maxPos.x, maxPos.y), color);
	this->Line(Vector2(maxPos.x, maxPos.y), Vector2(minPos.x, maxPos.y), color);
	this->Line(Vector2(minPos.x, maxPos.y), Vector2(minPos.x, minPos.y), color);
}
