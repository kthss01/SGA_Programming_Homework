#include "stdafx.h"
#include "Gizmo.h"

Gizmo* Gizmo::instance = NULL;

Gizmo::Gizmo()
{
	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMO_SEGMENTS;

	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++) {
		float angle = i * intervalAngle;
		circlePos[i] = Vector2(cosf(angle), sinf(angle));
	}
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

// centerPos 값은 회전값 받지 않게 position값만 받음됨 (worldPosition으로 받지 말고)
void Gizmo::Circle(Vector2 & centerPos, float radius, DWORD color) const
{
	D3DXMATRIX matPrev;
	// Transform 이라 TS RenderState RS
	D2D::GetDevice()->GetTransform(D3DTS_WORLD, &matPrev);

	DWORD prevLight;
	D2D::GetDevice()->GetRenderState(D3DRS_LIGHTING, &prevLight);

	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	GIZMOVERTEX vertices[CIRCLEGIZMO_SEGMENTS + 1];

	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++) {
		vertices[i].position = circlePos[i];
		vertices[i].color = color;
	}

	// scale 값 적용
	// 3d에서 바뀌게 되면 이쪽이 바뀌게됨 회전이 들어가서
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 right = D3DXVECTOR3(1, 0, 0);

	// 1,0,0	0,0,1	0,1,0
	// 밀어내는 식으로 하면 된다고 함 x축, y축, z축 세번 해줘야됨
	forward *= radius;
	up *= radius;
	right *= radius;

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// 11, 12, 13 값만 복사
	memcpy(&matWorld._11, right, sizeof(D3DXVECTOR3));
	// 21, 22, 23 값만 복사
	memcpy(&matWorld._21, up, sizeof(D3DXVECTOR3));
	// 31, 32, 33 값만 복사
	memcpy(&matWorld._31, forward, sizeof(D3DXVECTOR3));

	matWorld._41 = centerPos.x;
	matWorld._42 = centerPos.y;

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	// [0][1][2][3]
	// strip = [0][1], [1][2], [2][3], [3][4] 한칸 씩 밀려가면서 그리는거
	// list = [0][1], [2][3], [4][5],
	D2D::GetDevice()->DrawPrimitiveUP(
		D3DPT_LINESTRIP, CIRCLEGIZMO_SEGMENTS,
		vertices, sizeof(GIZMOVERTEX)
	);

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matPrev);
	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void Gizmo::AABBBox(const Vector2 & minPos, const Vector2 & maxPos, DWORD color) const
{
	this->Line(Vector2(minPos.x, minPos.y), Vector2(maxPos.x, minPos.y), color);
	this->Line(Vector2(maxPos.x, minPos.y), Vector2(maxPos.x, maxPos.y), color);
	this->Line(Vector2(maxPos.x, maxPos.y), Vector2(minPos.x, maxPos.y), color);
	this->Line(Vector2(minPos.x, maxPos.y), Vector2(minPos.x, minPos.y), color);
}
