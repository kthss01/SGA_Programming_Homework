#include "stdafx.h"
#include "MainGame7.h"


MainGame7::MainGame7()
{
}


MainGame7::~MainGame7()
{
}

HRESULT MainGame7::Init()
{
	GameNode6::Init();

	clock = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 500, 500);

	for (int i = 0; i < TIME_END; i++) {
		timeLineInfo[i].x = WINSIZEX / 2;
		timeLineInfo[i].y = WINSIZEY / 2 - 100;
	}

	for (int i = 0; i < 12; i++) {
		timeNameInfo[i].time = i;
		timeNameInfo[i].x = WINSIZEX / 2 + cos((90 - 30 * i + 1) * PI / 180) * 210.0f - 20;
		timeNameInfo[i].y = WINSIZEY / 2 + -sin((90 - 30 * i + 1) * PI / 180) * 210.0f;
	}

	timeNameInfo[0].time = 12;

	return S_OK;
}

void MainGame7::Release()
{
	GameNode6::Release();
}

void MainGame7::Update()
{
	GameNode6::Update();

	// 시 분 초 선 업데이트
	// sec
	timeLineInfo[TIME_SECOND].x =
		WINSIZEX / 2 + cos((90 - 6 * st.wSecond) * PI / 180) * 200.0f;
	timeLineInfo[TIME_SECOND].y =
		WINSIZEY / 2 + -sin((90 - 6 * st.wSecond) * PI / 180) * 200.0f;
	// min
	timeLineInfo[TIME_MINUTE].x =
		WINSIZEX / 2 + cos((90 - 6 * st.wMinute) * PI / 180) * 200.0f;
	timeLineInfo[TIME_MINUTE].y =
		WINSIZEY / 2 + -sin((90 - 6 * st.wMinute) * PI / 180) * 200.0f;
	// hour
	int hour = st.wHour;
	// 1~ 12시로 바꿔줌
	if (hour > 12)
		hour -= 12;

	// 시가 분에 따라서도 조금씩 움직이는데
	// 조사 결과 1분당 0.5씩 움직이게됨 따라서 0.5f * 분을 빼줘야함
	timeLineInfo[TIME_HOUR].x =
		WINSIZEX / 2 + cos((90.0f - 30 * hour - 0.5f * st.wMinute) * PI / 180) * 150.0f;
	timeLineInfo[TIME_HOUR].y =
		WINSIZEY / 2 + -sin((90.0f - 30 * hour - 0.5f * st.wMinute) * PI / 180) * 150.0f;

	GetLocalTime(&st);
}

void MainGame7::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	char str[128];
	sprintf_s(str, "%d시 %d분 %d 초", st.wHour, st.wMinute, st.wSecond);
	TextOut(memDC, 10, 10, str, strlen(str));

	sprintf_s(str, "mouseX : %d, mouseY : %d", g_ptMouse.x, g_ptMouse.y);
	TextOut(memDC, 10, 30, str, strlen(str));

	for (int i = 0; i < TIME_END; i++) {
		sprintf_s(str, "time %d X : %d, time %d Y : %d", 
			i, i, timeLineInfo[i].x, timeLineInfo[i].y);
		TextOut(memDC, 10, 50 + i*20, str, strlen(str));
	}
	
	Ellipse(memDC, clock.left, clock.top, clock.right, clock.bottom);

	for (int i = 0; i < 12; i++) {
		HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

		sprintf_s(str, "%d 시", timeNameInfo[i].time);
		TextOut(memDC, timeNameInfo[i].x, timeNameInfo[i].y, str, strlen(str));

		SelectObject(memDC, OldFont);
		DeleteObject(hFont);
	}

	for (int i = 0; i < TIME_END; i++) {
		HPEN pen = CreatePen(PS_SOLID, 1+i*2, RGB(0, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(memDC, pen);

		LineMake(memDC, WINSIZEX / 2, WINSIZEY / 2, timeLineInfo[i].x, timeLineInfo[i].y);

		SelectObject(memDC, oldPen);
		DeleteObject(pen);
	}

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

