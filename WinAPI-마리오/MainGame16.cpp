#include "stdafx.h"
#include "MainGame16.h"


MainGame16::MainGame16()
{
}


MainGame16::~MainGame16()
{
}

HRESULT MainGame16::Init()
{
	GameNode::Init();

	mario = new Image;
	bg = new Image;
	tile = new Image;

	mario->Init("images/mario_all.bmp", 800, 400, true, RGB(255, 0, 255));
	bg->Init("images/mario_bg.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	tile->Init("images/mario_tile1.bmp", 
		WINSIZEX / 8, WINSIZEY / 4, true, RGB(255, 0, 255));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			// 마지막 3, 7은 빈 이미지
			if (i == 3 && j == 7) continue;
			ani[i * 8 + j].width = 100;
			ani[i * 8 + j].height = 100;
			ani[i * 8 + j].offsetX = ani[i * 8 + j].width * j;
			ani[i * 8 + j].offsetY = ani[i * 8 + j].height * i;
		}
	}

	marioInfo.speed = 0;
	marioInfo.state = STATE_LEFTIDLE;
	marioInfo.x = WINSIZEX / 2;
	marioInfo.y = WINSIZEY - WINSIZEY / 4 - ani[0].height;
	marioInfo.offsetX = ani[STATE_LEFTIDLE].offsetX;
	marioInfo.offsetY = ani[STATE_LEFTIDLE].offsetY;
	marioInfo.width = ani[STATE_LEFTIDLE].width;
	marioInfo.height = ani[STATE_LEFTIDLE].height;
	marioInfo.stateCount = 0;

	prevTime = GetTickCount();

	gravity = 0;

	rc = RectMake(WINSIZEX / 2, WINSIZEY / 2 - WINSIZEY / 16,
		WINSIZEX / 8, WINSIZEY / 8);

	return S_OK;
}

void MainGame16::Release()
{
	GameNode::Release();
	SAFE_DELETE(mario);
	SAFE_DELETE(bg);
	SAFE_DELETE(tile);
}

void MainGame16::Update()
{
	GameNode::Update();


	// 왼쪽 방향키
	if (INPUT->GetKey(VK_LEFT)) {
		// 점프 중인 경우 점프 방향만 변경 (상승 중)
		if (marioInfo.state == STATE_RIGHTJUMPUP
			|| marioInfo.state == STATE_LEFTJUMPUP) {
			if (marioInfo.speed > -5.0f)
				marioInfo.speed -= 1;
			marioInfo.state = STATE_LEFTJUMPUP;
		}
		// 점프 중인 경우 점프 방향만 변경 (하강 중)
		else if (marioInfo.state == STATE_RIGHTJUMPDOWN
			|| marioInfo.state == STATE_LEFTJUMPDOWN)
		{
			if(marioInfo.speed > -5.0f)
				marioInfo.speed -= 1;
			marioInfo.state = STATE_LEFTJUMPDOWN;
		}
		// 달리기 중인 경우
		else if (marioInfo.isRun) {
			// 속도값 변경
			marioInfo.speed -= 0.3f;
			// 일정 속도 이상 달리기 상태로 변경
			if (marioInfo.speed <= -10.0f)
				marioInfo.state = STATE_LEFTRUN;
			// 달리기 상태 중 방향 전환 시
			else if (marioInfo.speed > 0)
				marioInfo.state = STATE_RIGHTRUNCHANGE;
			// 그 외 일정 속도 이하인 경우
			else
				marioInfo.state = STATE_LEFTWALK;
		}
		// 왼쪽으로 달리는 중 그냥 걷는 경우
		else if (marioInfo.state == STATE_LEFTRUN
			|| marioInfo.state == STATE_LEFTRUNCHANGE)
		{
			// 속도 감소
			marioInfo.speed += 0.3f;
			// 걷기 속도 도달 시 걷기로 변경 및 걷기 모션
			if (marioInfo.speed > -5.0f) {
				marioInfo.speed = -5.0f;
				marioInfo.state = STATE_LEFTWALK;
			}
		}
		// 오른쪽으로 달리는 중 그냥 걷는 경우
		else if (marioInfo.state == STATE_RIGHTRUN
			|| marioInfo.state == STATE_RIGHTRUNCHANGE)
		{
			// 방향 전환 모션으로 변경
			marioInfo.state = STATE_RIGHTRUNCHANGE;
			// 속도 증가
			marioInfo.speed -= 0.3f;

			// 걷기 속도 도달 시 걷기로 변경 및 걷기 모션
			if (marioInfo.speed < -5.0f) {
				marioInfo.speed = 5.0f;
				marioInfo.state = STATE_RIGHTWALK;
			}
		}
		// 그냥 걷는 경우
		else {
			marioInfo.state = STATE_LEFTWALK;
			marioInfo.speed = -5.0f;
		}
	}

	// 오른쪽 방향키를 때는 경우
	if (INPUT->GetKeyUp(VK_LEFT)) {
		// 걷고 있는 중이면 왼쪽 정지 상태로 변경
		if (marioInfo.state == STATE_LEFTWALK) {
			marioInfo.state = STATE_LEFTIDLE;
			marioInfo.speed = 0;
		}
	}
	
	if (INPUT->GetKey(VK_RIGHT)) {
		// 점프 중인 경우 점프 방향만 변경 (상승 중)
		if (marioInfo.state == STATE_LEFTJUMPUP
			|| marioInfo.state == STATE_RIGHTJUMPUP) {
			if (marioInfo.speed < 5.0f)
				marioInfo.speed += 1;
			marioInfo.state = STATE_RIGHTJUMPUP;
		}
		// 점프 중인 경우 점프 방향만 변경 (하강 중)
		else if (marioInfo.state == STATE_LEFTJUMPDOWN
			|| marioInfo.state == STATE_RIGHTJUMPDOWN) {
			if (marioInfo.speed < 5.0f)
				marioInfo.speed += 1;
			marioInfo.state = STATE_RIGHTJUMPDOWN;
		}
		// 달리기 중인 경우
		else if (marioInfo.isRun) {
			marioInfo.speed += 0.3f;
			// 일정 속도 이상 달리기 상태로 변경
			if (marioInfo.speed >= 10.0f)
				marioInfo.state = STATE_RIGHTRUN;
			// 달리기 상태 중 방향 전환 시
			else if (marioInfo.speed < 0)
				marioInfo.state = STATE_LEFTRUNCHANGE;
			// 그 외 일정 속도 이하인 경우
			else
				marioInfo.state = STATE_RIGHTWALK;
		}
		// 오른쪽으로 달리는 중 그냥 걷는 경우
		else if (marioInfo.state == STATE_RIGHTRUN
			|| marioInfo.state == STATE_RIGHTRUNCHANGE)
		{
			// 속도 감소
			marioInfo.speed -= 0.3f;
			// 걷기 속도 도달 시 걷기로 변경 및 걷기 모션
			if (marioInfo.speed < 5.0f) {
				marioInfo.speed = 5.0f;
				marioInfo.state = STATE_RIGHTWALK;
			}
		}
		// 왼쪽으로 달리는 중 그냥 걷는 경우
		else if (marioInfo.state == STATE_LEFTRUN
			|| marioInfo.state == STATE_LEFTRUNCHANGE)
		{
			// 방향 전환 모션으로 변경
			marioInfo.state = STATE_LEFTRUNCHANGE;
			// 속도 증가
			marioInfo.speed += 0.3f;

			// 걷기 속도 도달 시 걷기로 변경 및 걷기 모션
			if (marioInfo.speed > 5.0f) {
				marioInfo.speed = 5.0f;
				marioInfo.state = STATE_RIGHTWALK;
			}
		}
		// 그냥 걷는 경우
		else {
			marioInfo.state = STATE_RIGHTWALK;
			marioInfo.speed = 5.0f;
		}
	}
	// 오른쪽 방향키를 때는 경우
	if (INPUT->GetKeyUp(VK_RIGHT)) {
		// 걷고 있는 중이면 오른쪽 정지 상태로 변경
		if (marioInfo.state == STATE_RIGHTWALK) {
			marioInfo.state = STATE_RIGHTIDLE;
			marioInfo.speed = 0;
		}
	}
	
	// 방향키를 때서 속도가 유지된 상태일 때
	if (marioInfo.state == STATE_LEFTIDLE
		|| marioInfo.state == STATE_RIGHTIDLE
		|| marioInfo.state == STATE_LEFTRUNCHANGE
		|| marioInfo.state == STATE_RIGHTRUNCHANGE
		|| marioInfo.state == STATE_RIGHTRUN
		|| marioInfo.state == STATE_LEFTRUN) {
		// 속도를 0에 값게 줄이거나 늘림 (정지 할 수 있도록)
		// 속도 변경이 달릴 때 상승 속도랑 같으면 더 속도 안올라감
		if (marioInfo.speed < 0)
			marioInfo.speed += 0.1f;
		else if (marioInfo.speed > 0)
			marioInfo.speed -= 0.1f;

		// 0에 비슷한 값에 도달시 0으로 변경 (소수값이므로 0이 안될 수 있어서)
		if (-0.001f <= marioInfo.speed &&
			marioInfo.speed <= 0.001)
			marioInfo.speed = 0;
	}

	// 속도에 따른 x 값 변경
	marioInfo.x += marioInfo.speed;

	// 화면 밖 벗어날 시
	// 좌측
	if (marioInfo.x + marioInfo.width< 0)
		marioInfo.x = WINSIZEX;
	// 우측
	if (marioInfo.x > WINSIZEX)
		marioInfo.x = -marioInfo.width;

	// 점프
 	if (INPUT->GetKeyDown(VK_SPACE)) {
		// 왼쪽 방향시 왼쪽 점프
		if (marioInfo.state == STATE_LEFTWALK ||
			marioInfo.state == STATE_LEFTIDLE ||
			marioInfo.state == STATE_LEFTRUN) {
			marioInfo.state = STATE_LEFTJUMPUP;

			gravity = 12.0f;
		}

		// 오른쪽 방향시 오른쪽 점프
		if (marioInfo.state == STATE_RIGHTWALK ||
			marioInfo.state == STATE_RIGHTIDLE ||
			marioInfo.state == STATE_RIGHTRUN) {
			marioInfo.state = STATE_RIGHTJUMPUP;

			gravity = 12.0f;
		}
	}

	// 빅토리 포즈
	if (INPUT->GetKeyDown('V')) {
		// 왼쪽 정지 상태시 왼쪽 빅토리
		if (marioInfo.state == STATE_LEFTIDLE)
			marioInfo.state = STATE_LEFTVICTORY;
		// 오른쪽 정지 상태시 오른쪽 빅토리
		if (marioInfo.state == STATE_RIGHTIDLE)
			marioInfo.state = STATE_RIGHTVICTORY;
		// 왼쪽 빅토리시 오른쪽 빅토리
		if (marioInfo.state == STATE_LEFTVICTORY)
			marioInfo.state = STATE_RIGHTVICTORY;
		// 오른쪽 빅토리시 왼쪽 빅토리
		else if (marioInfo.state == STATE_RIGHTVICTORY)
			marioInfo.state = STATE_LEFTVICTORY;
	}

	// 가속도 버튼 클릭시 달리기
	if (INPUT->GetKeyDown('A')) { marioInfo.isRun = true; }
	if (INPUT->GetKeyUp('A')) { marioInfo.isRun = false; }

	// 모든 경우 중력 적용
	marioInfo.y -= gravity;

	// State 에 따른 추가적 변화
	switch (marioInfo.state)
	{
		// 달리기 중 방향 변환시 속도가 정지되면 idle 상태로 변경
	case STATE_LEFTRUN:
	case STATE_LEFTRUNCHANGE:
		if (marioInfo.speed == 0)
			marioInfo.state = STATE_LEFTIDLE;
		break;
	case STATE_RIGHTRUN:
	case STATE_RIGHTRUNCHANGE:
		if (marioInfo.speed == 0)
			marioInfo.state = STATE_RIGHTIDLE;
		break;
	}

	RECT rcMario = RectMake(marioInfo.x, marioInfo.y,
		marioInfo.width, marioInfo.height);
	RECT temp;
	if (IntersectRect(&temp, &rc, &rcMario)) {
		// left
		if (marioInfo.x + marioInfo.width / 2 < rc.left
			&& marioInfo.y + marioInfo.height / 2 > rc.top) {
			marioInfo.x = rc.left - marioInfo.width;
		}
		// right
		if (marioInfo.x + marioInfo.width / 2 > rc.right
			&& marioInfo.y + marioInfo.height / 2 > rc.top) {
			marioInfo.x = rc.right;
		}
		// up
		if (marioInfo.y + marioInfo.height / 2 < rc.top) {
			gravity = 0;
			marioInfo.y = rc.top - marioInfo.height;
			if (marioInfo.state == STATE_LEFTJUMPUP
				|| marioInfo.state == STATE_LEFTJUMPDOWN)
				marioInfo.state = STATE_LEFTIDLE;
			if (marioInfo.state == STATE_RIGHTJUMPUP
				|| marioInfo.state == STATE_RIGHTJUMPDOWN)
				marioInfo.state = STATE_RIGHTIDLE;
		}
		// bottom
		if (marioInfo.y + marioInfo.height / 2 > rc.bottom) {
			gravity = 0;
			marioInfo.y = rc.bottom;
		}
	}

	// 바닥 이상으론 떨어지지 않도록 바닥보다 아래인 경우
	if (marioInfo.y > WINSIZEY - WINSIZEY / 4 - ani[0].height) {
		marioInfo.y = WINSIZEY - WINSIZEY / 4 - ani[0].height;

		gravity = 0;

		// 점프 상태에서 정지 상태로 변경
  		if (marioInfo.state == STATE_LEFTJUMPDOWN ||
			marioInfo.state == STATE_LEFTJUMPUP)
			marioInfo.state = STATE_LEFTIDLE;
		if (marioInfo.state == STATE_RIGHTJUMPDOWN ||
			marioInfo.state == STATE_RIGHTJUMPUP)
			marioInfo.state = STATE_RIGHTIDLE;
	}
	
	// 일정 높이 이상시 상승 모션에서 하강 모션으로 변경 (중력이 0일 때)
	if (gravity == 0 && marioInfo.state == STATE_LEFTJUMPUP)
		marioInfo.state = STATE_LEFTJUMPDOWN;
	if (gravity == 0 && marioInfo.state == STATE_RIGHTJUMPUP)
		marioInfo.state = STATE_RIGHTJUMPDOWN;

	

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {
	
		gravity -= 2;

		// 상태에 따른 애니메이션 변경
		CheckAniForState();

		prevTime = GetTickCount();
	}
}

void MainGame16::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	bg->Render(memDC);
	
	for (int i = 0; i < 8; i++) {
		tile->Render(memDC, WINSIZEX / 8 * i, WINSIZEY - WINSIZEY / 4);
	}

	tile->Render(memDC, WINSIZEX / 2, WINSIZEY / 2 - WINSIZEY/16,
		0, 0, WINSIZEX / 8, WINSIZEY / 8);

	// test
	//mario->Render(memDC);
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		// 빈 이미지 위치
	//		if (i == 3 & j == 7) continue;
	//		mario->Render(memDC, 
	//			ani[i * 8 + j].offsetX,
	//			ani[i * 8 + j].offsetY,
	//			ani[i * 8 + j].offsetX,
	//			ani[i * 8 + j].offsetY,
	//			ani[i * 8 + j].width,
	//			ani[i * 8 + j].height);
	//	}
	//}
	//
	//int i = 0;
	//int j = 1;
	//
	//marioImage->Render(memDC,
	//				mario.x, mario.y,
	//				ani[i * 8 + j].offsetX,
	//				ani[i * 8 + j].offsetY,
	//				ani[i * 8 + j].width,
	//				ani[i * 8 + j].height);

	mario->Render(memDC, marioInfo.x, marioInfo.y,
		marioInfo.offsetX, marioInfo.offsetY, marioInfo.width, marioInfo.height);
	
	char str[64];
	switch (marioInfo.state)
	{
	case STATE_LEFTIDLE:
		sprintf_s(str, "State : 왼 대기");
		break;
	case STATE_RIGHTIDLE:
		sprintf_s(str, "State : 오른 대기");
		break;
	case STATE_LEFTWALK:
		sprintf_s(str, "State : 왼 걷기");
		break;
	case STATE_RIGHTWALK:
		sprintf_s(str, "State : 오른 걷기");
		break;
	case STATE_LEFTRUN:
		sprintf_s(str, "State : 왼 뛰기");
		break; 
	case STATE_RIGHTRUN:
		sprintf_s(str, "State : 오른 뛰기");
		break;
	case STATE_LEFTJUMPUP:
		sprintf_s(str, "State : 왼 점프 업");
		break;
	case STATE_LEFTJUMPDOWN:
		sprintf_s(str, "State : 왼 점프 다운");
		break;
	case STATE_RIGHTJUMPUP:
		sprintf_s(str, "State : 오른 점프 업");
		break;
	case STATE_RIGHTJUMPDOWN:
		sprintf_s(str, "State : 오른 점프 다운");
		break;
	case STATE_LEFTVICTORY:
		sprintf_s(str, "State : 왼 빅토리");
		break;
	case STATE_RIGHTVICTORY:
		sprintf_s(str, "State : 오른 빅토리");
		break;
	case STATE_LEFTRUNCHANGE:
		sprintf_s(str, "State : 왼 달리기 중 오른");
		break;	
	case STATE_RIGHTRUNCHANGE:
		sprintf_s(str, "State : 오른 달리기 중 왼");
		break;
	default:
		break;
	}
	
	TextOut(memDC, 10, 10, str, strlen(str));

	sprintf_s(str, "x : %f, y : %f", marioInfo.x, marioInfo.y);
	TextOut(memDC, 10, 30, str, strlen(str));

	sprintf_s(str, "speed : %f", marioInfo.speed);
	TextOut(memDC, 10, 50, str, strlen(str));

	sprintf_s(str, "gravity : %f", gravity);
	TextOut(memDC, 10, 90, str, strlen(str));

	if (marioInfo.isRun) {
		sprintf_s(str, "가속 중");
		TextOut(memDC, 10, 70, str, strlen(str));
	}

	//RectangleMake(memDC, rc);

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame16::CheckAniForState()
{
	switch (marioInfo.state)
	{
	// 하나의 이미지로만 되어 있는 에니메이션
	case STATE_LEFTIDLE:
	case STATE_RIGHTIDLE:
	case STATE_LEFTJUMPUP:
	case STATE_LEFTJUMPDOWN:
	case STATE_RIGHTJUMPUP:
	case STATE_RIGHTJUMPDOWN:
	case STATE_LEFTVICTORY:
	case STATE_RIGHTVICTORY:
	case STATE_LEFTRUNCHANGE:
	case STATE_RIGHTRUNCHANGE:
		marioInfo.stateCount = 0;
		break;
	// 두 개의 이미지로 되어 있는 에니메이션
	case STATE_LEFTWALK:
	case STATE_RIGHTWALK:
		marioInfo.stateCount++;
		if (marioInfo.stateCount >= 2)
			marioInfo.stateCount = 0;
		break;
	// 세 개의 이미지로 되어 있는 에니메이션
	case STATE_LEFTRUN:
	case STATE_RIGHTRUN:
		marioInfo.stateCount++;
		if (marioInfo.stateCount >= 3)
			marioInfo.stateCount = 0;
		break;
	}

	marioInfo.offsetX = ani[marioInfo.state + marioInfo.stateCount].offsetX;
	marioInfo.offsetY = ani[marioInfo.state + marioInfo.stateCount].offsetY;

	
}
