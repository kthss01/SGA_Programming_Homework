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

	if (INPUT->GetKey(VK_LEFT)) {
		if (marioInfo.state == STATE_RIGHTJUMPUP
			|| marioInfo.state == STATE_LEFTJUMPUP)
			marioInfo.state = STATE_LEFTJUMPUP;
		else if (marioInfo.state == STATE_RIGHTJUMPDOWN
			|| marioInfo.state == STATE_LEFTJUMPDOWN)
			marioInfo.state = STATE_LEFTJUMPDOWN;
		else
			marioInfo.state = STATE_LEFTWALK;
		marioInfo.x -= 5.0f;
		if (marioInfo.x + marioInfo.width< 0)
			marioInfo.x = WINSIZEX;

	}
	if (INPUT->GetKeyUp(VK_LEFT)) { 
		if(marioInfo.state == STATE_LEFTWALK)
			marioInfo.state = STATE_LEFTIDLE; 
	}
	
	if (INPUT->GetKey(VK_RIGHT)) { 
		if (marioInfo.state == STATE_LEFTJUMPUP
			|| marioInfo.state == STATE_RIGHTJUMPUP)
			marioInfo.state = STATE_RIGHTJUMPUP;
		else if (marioInfo.state == STATE_LEFTJUMPDOWN
			|| marioInfo.state == STATE_RIGHTJUMPDOWN)
			marioInfo.state = STATE_RIGHTJUMPDOWN;
		else
			marioInfo.state = STATE_RIGHTWALK;
		marioInfo.x += 5.0f;
		if (marioInfo.x > WINSIZEX)
			marioInfo.x = -marioInfo.width;
	}
	if (INPUT->GetKeyUp(VK_RIGHT)) { 
		if(marioInfo.state == STATE_RIGHTWALK)
			marioInfo.state = STATE_RIGHTIDLE; 
	}

 	if (INPUT->GetKeyDown(VK_SPACE)) {
		if (marioInfo.state == STATE_LEFTWALK ||
			marioInfo.state == STATE_LEFTIDLE) {
			marioInfo.state = STATE_LEFTJUMPUP;

			gravity = 10.0f;
		}

		if (marioInfo.state == STATE_RIGHTWALK ||
			marioInfo.state == STATE_RIGHTIDLE) {
			marioInfo.state = STATE_RIGHTJUMPUP;

			gravity = 10.0f;
		}
	}

	if (INPUT->GetKeyDown('V')) {
		if (marioInfo.state == STATE_LEFTIDLE)
			marioInfo.state = STATE_LEFTVICTORY;
		if (marioInfo.state == STATE_RIGHTIDLE)
			marioInfo.state = STATE_RIGHTVICTORY;
		if (marioInfo.state == STATE_LEFTVICTORY)
			marioInfo.state = STATE_RIGHTVICTORY;
		else if (marioInfo.state == STATE_RIGHTVICTORY)
			marioInfo.state = STATE_LEFTVICTORY;
	}

	switch (marioInfo.state)
	{
	case STATE_LEFTIDLE:
		break;
	case STATE_RIGHTIDLE:
		break;
	case STATE_LEFTWALK:
		break;
	case STATE_RIGHTWALK:
		break;
	case STATE_LEFTRUN:
		break;
	case STATE_RIGHTRUN:
		break;
	case STATE_LEFTJUMPUP:
	case STATE_LEFTJUMPDOWN:
	case STATE_RIGHTJUMPUP:
	case STATE_RIGHTJUMPDOWN:
		marioInfo.y -= gravity;
		break;
	default:
		break;
	}

	if (marioInfo.y > WINSIZEY - WINSIZEY / 4 - ani[0].height) {
		marioInfo.y = WINSIZEY - WINSIZEY / 4 - ani[0].height;

  		if (marioInfo.state == STATE_LEFTJUMPDOWN ||
			marioInfo.state == STATE_LEFTJUMPUP)
			marioInfo.state = STATE_LEFTIDLE;
		if (marioInfo.state == STATE_RIGHTJUMPDOWN ||
			marioInfo.state == STATE_RIGHTJUMPUP)
			marioInfo.state = STATE_RIGHTIDLE;
	}
	
	if (gravity == 0 && marioInfo.state == STATE_LEFTJUMPUP)
		marioInfo.state = STATE_LEFTJUMPDOWN;
	if (gravity == 0 && marioInfo.state == STATE_RIGHTJUMPUP)
		marioInfo.state = STATE_RIGHTJUMPDOWN;

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {


		gravity -= 2;
		//marioInfo.speed -= 0.1f;
		
		CheckState();

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
	sprintf_s(str, "State : %d", marioInfo.state);
	TextOut(memDC, 10, 10, str, strlen(str));

	sprintf_s(str, "x : %f, y : %f", marioInfo.x, marioInfo.y);
	TextOut(memDC, 10, 30, str, strlen(str));

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame16::CheckState()
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
		marioInfo.stateCount = 0;
		break;
	// 두 개의 이미지로 되어 있는 에니메이션
	case STATE_LEFTWALK:
	case STATE_RIGHTWALK:
		marioInfo.stateCount++;
		if (marioInfo.stateCount == 2)
			marioInfo.stateCount = 0;
		break;
	// 세 개의 이미지로 되어 있는 에니메이션
	case STATE_LEFTRUN:
	case STATE_RIGHTRUN:
		marioInfo.stateCount++;
		if (marioInfo.stateCount == 3)
			marioInfo.stateCount = 0;
		break;
	}

	marioInfo.offsetX = ani[marioInfo.state + marioInfo.stateCount].offsetX;
	marioInfo.offsetY = ani[marioInfo.state + marioInfo.stateCount].offsetY;

	
}
