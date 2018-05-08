#include "stdafx.h"
#include "Program.h"


Program::Program()
{
	vEye = Vector2(0, 0, 0);
	vLookAt = Vector2(0, 0, 1);
	vUp = Vector2(0, 1);

	hdc = GetDC(g_hWnd);
	memDC = CreateCompatibleDC(hdc);

	hBitMap = CreateCompatibleBitmap(hdc, WINSIZE_X, WINSIZE_Y);
	hOldBitmap = (HBITMAP)SelectObject(memDC, hBitMap);

	/////////////////////////////////////////////////
	// 로컬 처리 - 물체에 대한 크기만 지정

	// 삼각형 두개로 사각형 만듬

	// 큰 삼각형
	vecVertex.push_back(Vector2(-135,  0));
	vecVertex.push_back(Vector2(0, 135));
	vecVertex.push_back(Vector2(135,  0));
	
	// 양옆에 작은 삼각형 두 개
	vecVertex.push_back(Vector2(-35, 0));
	vecVertex.push_back(Vector2(-135, -75));
	vecVertex.push_back(Vector2(-135, 0));

	vecVertex.push_back(Vector2(135, 0));
	vecVertex.push_back(Vector2(135, -75));
	vecVertex.push_back(Vector2(35, 0));

	// 가운데 작은 삼각형
	vecVertex.push_back(Vector2(35, 0));
	vecVertex.push_back(Vector2(0, -55));
	vecVertex.push_back(Vector2(-35, 0));

	// 아래 꼬리 삼각형 두 개
	vecVertex.push_back(Vector2(0, -55));
	vecVertex.push_back(Vector2(-85, -150));
	vecVertex.push_back(Vector2(-35, 0));

	vecVertex.push_back(Vector2(35, 0));
	vecVertex.push_back(Vector2(85, -150));
	vecVertex.push_back(Vector2(0, -55));

	matWorld = Matrix::Identity(4);
	matView = Matrix::View(vEye, vLookAt, vUp);
	// z축이 0.0 ~ 1.0 까지만 렌더하기 위한거
	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0.0f, 1.0f);
	matViewport = Matrix::Viewport(0, 0, WINSIZE_X, WINSIZE_Y, 0, 1);

	vBoxPos = Vector2(WINSIZE_X / 2, WINSIZE_Y / 2);
	//Vector2 move = Vector2(200, 0);
	//matChildTrans = Matrix::Translation(move);
	fAngle = 0.0f;

	isDebug = true;
}


Program::~Program()
{
	ReleaseDC(g_hWnd, hdc);

	SelectObject(memDC, hOldBitmap);
	DeleteObject(hBitMap);
	DeleteDC(memDC);
}

void Program::Update()
{
	Matrix matTrans;
	Matrix matRotate;
	float fSpeed = 1.0f;

	// 회전이 들어가지 않은 box의 y축
	Vector2 dirTo = Vector2(0, 1);

	matTrans = Matrix::Translation(vBoxPos);
	matRotate = Matrix::Rotate(fAngle);

	// coord는 이동 시켜주는 녀석
	// 회전을 시켜주는 녀석
	dirTo = dirTo.TransformNormal(matRotate);

	if (GetKeyState('W') & 0x8000) { 
		//vBoxPos.y += fSpeed; 
		vBoxPos = vBoxPos + dirTo * fSpeed;
	}
	if (GetKeyState('S') & 0x8000) { 
		//vBoxPos.y -= fSpeed; 
		vBoxPos = vBoxPos - dirTo * fSpeed;
	}
	if (GetKeyState('A') & 0x8000) { fAngle += 0.01f; }
	if (GetKeyState('D') & 0x8000) { fAngle -= 0.01f; }
	
	if (GetKeyState(VK_TAB) & 0x8000) { isDebug = true; }
	if (GetKeyState(VK_SHIFT) & 0x8000) { isDebug = false; }

	Matrix matScale = Matrix::Identity(4);

	// scale인 함수 안만들어서 수동으로 조작 필요
	// scale은 기본값이 1임
	matScale[0][0] = 1.5f;	// 물체에 대한 가로 길이
	matScale[1][1] = 1.5f;  // 물체에 대한 세로 길이

	// SRT 순서로 넣어줘야함 -> Scale(크기) * Rotate(회전) * Translate(이동)
	matWorld = matScale * matRotate * matTrans;
	//matChild = matChildTrans * matWorld;
}

void Program::Render()
{
	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
	
	// 랜더링 파이프라인
	// 렌더링 해야하는 순서 지정, 이 순서대로여야만 함
	// 이번에 직접 구현해본거고 다이렉트에는 구현되어있음
	// 다이렉트에서는 넣어주는 거
	Matrix matWVPV = matWorld * matView * matProjection * matViewport;

	// size_t 부호가 없는 정수
	// 계산된 매트릭스 이동해서 정점들 변환 시켜주는 거
	for (size_t i = 0; i < vecVertex.size(); i += 3) {
		Vector2 v1 = vecVertex[i].TransformCoord(matWVPV);
		Vector2 v2 = vecVertex[i + 1].TransformCoord(matWVPV);
		Vector2 v3 = vecVertex[i + 2].TransformCoord(matWVPV);

		// 내부선 안그릴때 작은 삼각형 무시
		if (i == 9 && !isDebug) continue;

		// 현재 그릴 수 있는 방법 없어서 라인으로 그릴꺼
		MoveToEx(memDC, v1.x, v1.y, NULL);
		LineTo(memDC, v2.x, v2.y);
		LineTo(memDC, v3.x, v3.y);
		if(isDebug)
			LineTo(memDC, v1.x, v1.y);
	}

	// 하나의 도형 더 띄우고 싶으면 (로컬값은 같은데)
	//Matrix matWorld2 = Matrix::Identity(4);
	////matWVPV = matWorld2 * matView * matProjection * matViewport;
	//matWVPV = matChild * matView * matProjection * matViewport;

	//// size_t 부호가 없는 정수
	//// 계산된 매트릭스 이동해서 정점들 변환 시켜주는 거
	//for (size_t i = 0; i < vecVertex.size(); i += 3) {
	//	Vector2 v1 = vecVertex[i].TransformCoord(matWVPV);
	//	Vector2 v2 = vecVertex[i + 1].TransformCoord(matWVPV);
	//	Vector2 v3 = vecVertex[i + 2].TransformCoord(matWVPV);

	//	// 현재 그릴 수 있는 방법 없어서 라인으로 그릴꺼
	//	MoveToEx(memDC, v1.x, v1.y, NULL);
	//	LineTo(memDC, v2.x, v2.y);
	//	LineTo(memDC, v3.x, v3.y);
	//	LineTo(memDC, v1.x, v1.y);
	//}

	BitBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, memDC, 0, 0, SRCCOPY);
}
