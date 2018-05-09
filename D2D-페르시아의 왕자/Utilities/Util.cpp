#include "stdafx.h"
#include "Util.h"

Util::Util()
{
}


Util::~Util()
{
}

void Util::GetMousePos(Vector2 * out)
{
	// API 함수를 통해서 받아올 꺼
	POINT cursorPos;

	GetCursorPos(&cursorPos);	// 현재 윈도우 창을 기준으로 생성됨

	ScreenToClient(g_hWnd, &cursorPos);

	// 마우스 포지션을 화면에 출력한 비율값으로 계산할꺼
	Vector2 temp;

	// 0 ~ 1 사이 비율로 변경 해줌
	temp.x = (float)cursorPos.x / (float)WINSIZE_X;
	// y값은 D3D를 하고 있어서 위가 +이고 아래가 -이어서 뒤집도록 빼준 거
	temp.y = 1.0f - (cursorPos.y / (float)WINSIZE_Y);

	// x값도 같은 이유로 왼쪽이 -1 오른쪽이 1이라 곱하고 빼준거
	temp.x = temp.x * 2.0f - 1.0f;
	temp.y = temp.y * 2.0f - 1.0f;

	// 이렇게 하면 화면 중심이 0,0 왼쪽 -1 오른쪽 1 위 1 아래 -1임

	*out = temp;
}

float Util::GetRandomSeed()
{
	int seed = rand() % 1001;
	
	float result = (float)seed / 1000.0f;
	
	return result;
}
