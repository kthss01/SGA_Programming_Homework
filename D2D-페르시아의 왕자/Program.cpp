#include "stdafx.h"
#include "Program.h"

#include "GameObject\Rect.h"
#include "GameObject\BackGround.h"

Program::Program()
{
	srand(time(NULL));

	bg = new BackGround;
	bg->Init();

	rect = new Rect;
	rect->Init();

	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	// 알파값 제거
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	vEye = Vector2(0, 0);	// 카메라의 위치
}

Program::~Program()
{
	rect->Release();
	SAFE_DELETE(bg);
	SAFE_DELETE(rect);
}

void Program::Update()
{
	bg->Update();
	rect->Update();

	// 카메라 만드는거
	Vector2 vLookAt(0, 0, 1);	// 바라보는 좌표값
	vLookAt = vEye + Vector2(0, 0, 1);
	Vector2 vUp(0, 1, 0);
	matView = Matrix::View(vEye, vLookAt, vUp);
	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0.0, 1.0f);
}

void Program::Render()
{
	// 실질적으로 계산하는건 Device로 넘겨주게됨
	D2D::GetDevice()->SetTransform(D3DTS_VIEW, &matView.ToDXMatrix());
	D2D::GetDevice()->SetTransform(D3DTS_PROJECTION, &matProjection.ToDXMatrix());

	bg->Render();
	rect->Render();
}
