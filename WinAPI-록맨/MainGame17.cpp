#include "stdafx.h"
#include "MainGame17.h"


MainGame17::MainGame17()
{
}


MainGame17::~MainGame17()
{
}

HRESULT MainGame17::Init()
{
	GameNode::Init();

	ground[0] = RectMakeCenter(
		WINSIZEX / 2, WINSIZEY - WINSIZEY / 16 - WINSIZEY/ 32, WINSIZEX, WINSIZEY / 8);
	ground[1] = RectMake(600, 350, 265, 50);
	ground[2] = RectMake(875, 165, 135, 50);

	rockman = new Image;
	bg = new Image;

	bg->Init("images/메가맨배경.bmp", WINSIZEX, WINSIZEY);
	rockman->Init("images/rockman.bmp",
		WINSIZEX, WINSIZEY + WINSIZEY/2, 12, 6, true, RGB(255,0,255));

	rockmanInfo.state = STATE_RIGHTIDLE;
	rockmanInfo.isGround = true;

	rockman->SetFrameX(0);
	rockman->SetFrameY(3);
	rockman->SetX(WINSIZEX / 2 + rockman->GetFrameWidth() / 2);
	rockman->SetY(ground[0].top - rockman->GetFrameHeight() / 2);

	gravity = 0;
	checkRect = false;
	checkStatus = false;

	prevTime = GetTickCount();

	return S_OK;
}

void MainGame17::Release()
{
	GameNode::Release();
	SAFE_DELETE(rockman);
	SAFE_DELETE(bg);
}

void MainGame17::Update()
{
	GameNode::Update();

	if (INPUT->GetKeyDown(VK_CONTROL)) { checkRect = !checkRect; }
	if (INPUT->GetKeyDown(VK_TAB)) { checkStatus = !checkStatus; }

 	if (INPUT->GetKeyDown(VK_LEFT)) {
		if (rockmanInfo.isGround) {
			if (rockmanInfo.state != STATE_LEFTWALK) {
				rockmanInfo.state = STATE_LEFTWALK;
				rockman->SetFrameX(0);
				rockman->SetFrameY(0);
			}
		}
	}

	if (INPUT->GetKey(VK_LEFT)) {
		if (rockmanInfo.isGround) {
			if (rockmanInfo.state != STATE_LEFTWALK) {
				rockmanInfo.state = STATE_LEFTWALK;
				rockman->SetFrameX(0);
				rockman->SetFrameY(0);
			}
		}
		else {
			if (rockmanInfo.state == STATE_LEFTWALLCATCH) {
				rockman->SetY(rockman->GetY() - 75.0f);
				if (rockman->GetFrameX() < 9)
					rockman->SetFrameX(rockman->GetFrameX() + 1);
			}
			else if (rockmanInfo.state != STATE_LEFTJUMP) {
				rockmanInfo.state = STATE_LEFTJUMP;
				rockman->SetFrameX(5);
				rockman->SetFrameY(4);
			}
		}

		rockman->SetX(rockman->GetX() - 5.0f);
	}

	if (INPUT->GetKeyUp(VK_LEFT)) {
		if (rockmanInfo.isGround) {
			if (rockmanInfo.state != STATE_LEFTIDLE) {
				rockmanInfo.state = STATE_LEFTIDLE;
				rockman->SetFrameX(5);
				rockman->SetFrameY(2);
			}
		}
	}

	if (INPUT->GetKeyDown(VK_RIGHT)) {
		if (rockmanInfo.isGround) {
			if (rockmanInfo.state != STATE_RIGHTWALK) {
				rockmanInfo.state = STATE_RIGHTWALK;
				rockman->SetFrameX(0);
				rockman->SetFrameY(1);
			}
		}
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		if (rockmanInfo.isGround) {
			if (rockmanInfo.state != STATE_RIGHTWALK) {
				rockmanInfo.state = STATE_RIGHTWALK;
				rockman->SetFrameX(0);
				rockman->SetFrameY(1);
			}
		}
		else {
			if (rockmanInfo.state == STATE_RIGHTWALLCATCH) {
				rockman->SetY(rockman->GetY() - 75.0f);
				if(rockman->GetFrameX() < 9)
					rockman->SetFrameX(rockman->GetFrameX() + 1);
			}
			else if (rockmanInfo.state != STATE_RIGHTJUMP) {
				rockmanInfo.state = STATE_RIGHTJUMP;
				rockman->SetFrameX(5);
				rockman->SetFrameY(5);
			}
		}

		rockman->SetX(rockman->GetX() + 5.0f);
	}

	if (INPUT->GetKeyUp(VK_RIGHT)) {
		if (rockmanInfo.isGround) {
			if (rockmanInfo.state != STATE_RIGHTIDLE) {
				rockmanInfo.state = STATE_RIGHTIDLE;
				rockman->SetFrameX(0);
				rockman->SetFrameY(3);
			}
		}
	}

	if (INPUT->GetKeyDown(VK_SPACE)) {
 		

		if (rockmanInfo.state == STATE_LEFTIDLE ||
			rockmanInfo.state == STATE_LEFTWALK ||
			rockmanInfo.state == STATE_LEFTWALLCATCH) {
			rockmanInfo.state = STATE_LEFTJUMP;
			rockmanInfo.isGround = false;
			rockman->SetFrameX(0);
			rockman->SetFrameY(4);
			gravity = 10;
		}
		if (rockmanInfo.state == STATE_RIGHTIDLE ||
			rockmanInfo.state == STATE_RIGHTWALK ||
			rockmanInfo.state == STATE_RIGHTWALLCATCH) {
			rockmanInfo.isGround = false;
			rockmanInfo.state = STATE_RIGHTJUMP;
			rockman->SetFrameX(0);
			rockman->SetFrameY(5);
			gravity = 10;
		}
	}

	if (rockmanInfo.isGround == false) {
		if (rockmanInfo.state == STATE_LEFTIDLE ||
			rockmanInfo.state == STATE_LEFTWALK ||
			rockmanInfo.state == STATE_LEFTWALLCATCH) {
			rockmanInfo.state = STATE_LEFTJUMP;
			rockman->SetFrameX(5);
			rockman->SetFrameY(4);
		}
		if (rockmanInfo.state == STATE_RIGHTIDLE ||
			rockmanInfo.state == STATE_RIGHTWALK ||
			rockmanInfo.state == STATE_RIGHTWALLCATCH) {
			rockmanInfo.state = STATE_RIGHTJUMP;
			rockman->SetFrameX(5);
			rockman->SetFrameY(5);
		}

	}

	rockman->SetY(rockman->GetY() - gravity);


	for (int i = 0; i < 3; i++) {
		RECT temp;
		// 지면
		if (i == 0 &&
			IntersectRect(&temp, &rockman->GetFrameBoundingBox(), &ground[i])) {
			gravity = 0;
			rockmanInfo.isGround = true;
			rockman->SetY(ground[i].top - rockman->GetFrameHeight() / 2);
		}
		// 밑이 뚤려있고 위에 서잇을 수 있는 지면
		if (i == 1 &&
			IntersectRect(&temp, &rockman->GetFrameBoundingBox(), &ground[i]) &&
			gravity < 0) {
			if (rockman->GetX() > ground[i].left &&
				rockman->GetX() < ground[i].right &&
				rockman->GetY() < ground[i].top) {
				gravity = 0;
				rockmanInfo.isGround = true;
				rockman->SetY(ground[i].top - rockman->GetFrameHeight() / 2);
			}
		}
		if (i == 2 &&
			IntersectRect(&temp, &rockman->GetFrameBoundingBox(), &ground[i])) {
			// 위
			if (rockman->GetX() > ground[i].left &&
				rockman->GetX() < ground[i].right &&
				rockman->GetY() < ground[i].top) {
				gravity = 0;
				rockmanInfo.isGround = true;
				rockman->SetY(ground[i].top - rockman->GetFrameHeight() / 2);
			}
			//// 아래
			//if (rockman->GetX() > ground[i].left &&
			//	rockman->GetX() < ground[i].right &&
			//	rockman->GetY() > ground[i].bottom) {
			//	gravity = 0;
			//	rockman->SetY(ground[i].bottom + rockman->GetFrameHeight() / 2);
			//}

			// 왼쪽 끝 잡는 경우
			if (rockman->GetX() > ground[i].left - 20 &&
				rockman->GetX() < ground[i].left &&
				rockman->GetY() > ground[i].top + rockman->GetFrameHeight() / 4 - 10 && 
				rockman->GetY() < ground[i].top + rockman->GetFrameHeight() / 4 + 10) {
				gravity = 0;
				rockman->SetY(ground[i].top + rockman->GetFrameHeight() / 4);
				if(rockmanInfo.state != STATE_RIGHTWALLCATCH)
					rockman->SetX(ground[i].left - 15);
				rockmanInfo.state = STATE_RIGHTWALLCATCH;
				rockman->SetFrameX(6);
				rockman->SetFrameY(3);
			}

			// 오른쪽 끝 잡는 경우
			if (rockman->GetX() < ground[i].right + 20 &&
				rockman->GetX() > ground[i].right &&
				rockman->GetY() > ground[i].top + rockman->GetFrameHeight() / 4 - 10 &&
				rockman->GetY() < ground[i].top + rockman->GetFrameHeight() / 4 + 10) {
				gravity = 0;
				rockman->SetY(ground[i].top + rockman->GetFrameHeight() / 4);
				if (rockmanInfo.state != STATE_LEFTWALLCATCH)
					rockman->SetX(ground[i].right + 15);
				rockmanInfo.state = STATE_LEFTWALLCATCH;
				rockman->SetFrameX(6);
				rockman->SetFrameY(2);
			}
		}
	}

	curTime = GetTickCount();
	if (curTime - prevTime > 100) {

		int curFrameX = rockman->GetFrameX();

		gravity -= 2.0f;
		if (gravity < -2)
			rockmanInfo.isGround = false;

 		switch (rockmanInfo.state)
		{
		case STATE_LEFTIDLE:
			curFrameX--;
			if (curFrameX < 0)
				curFrameX = 5;
			break;
		case STATE_RIGHTIDLE:
			curFrameX++;
			if (curFrameX > 5)
				curFrameX = 0;
			break;
		case STATE_LEFTJUMP:
			if (gravity > 0) {
				curFrameX++;
				if (curFrameX > 4)
					curFrameX = 1;
			}
			else if (gravity == 0)
			{
				curFrameX = 5;
			}
			else {
				curFrameX++;

				if (rockmanInfo.isGround) {
					rockmanInfo.state = STATE_LEFTIDLE;
					curFrameX = 0;
					rockman->SetFrameX(0);
					rockman->SetFrameY(2);
					/*if (curFrameX < 10)
						curFrameX = 10;
					if (curFrameX > 11) {
						rockmanInfo.state = STATE_LEFTIDLE;
						curFrameX = 0;
						rockman->SetFrameX(0);
						rockman->SetFrameY(2);
					}*/
				}
				else if (curFrameX > 9)
					curFrameX = 6;
			}
			break;
		case STATE_RIGHTJUMP:
			if (gravity > 0) {
				curFrameX++;
				if (curFrameX > 4)
					curFrameX = 1;
			}
			else if (gravity == 0)
			{
				curFrameX = 5;
			}
			else {
				curFrameX++;
				
				if (rockmanInfo.isGround) {
					rockmanInfo.state = STATE_RIGHTIDLE;
					curFrameX = 0;
					rockman->SetFrameX(0);
					rockman->SetFrameY(3);
					//if (curFrameX < 10)
					//	curFrameX = 10;
 				//	if (curFrameX > 11) {
					//	rockmanInfo.state = STATE_RIGHTIDLE;
					//	curFrameX = 0;
					//	rockman->SetFrameX(0);
					//	rockman->SetFrameY(3);
					//}
				}
				else if (curFrameX > 9)
					curFrameX = 6;
			}
			break;
		case STATE_LEFTWALLCATCH:
			break;
		case STATE_LEFTWALK:
			curFrameX++;
			if (curFrameX > 10)
				curFrameX = 0;
			break;
		case STATE_RIGHTWALLCATCH:
			break;
		case STATE_RIGHTWALK:
			curFrameX++;
			if (curFrameX > 10)
				curFrameX = 0;
			break;
		default:
			break;
		}

		rockman->SetFrameX(curFrameX);

		prevTime = GetTickCount();
	}
}

void MainGame17::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	bg->Render(memDC);
	//rockman->Render(memDC);

	//rockman->FrameRender(memDC, WINSIZEX / 2, WINSIZEY / 2, x, 0);
	rockman->FrameRender(memDC, 
		rockman->GetX() - rockman->GetFrameWidth() / 2, 
		rockman->GetY() - rockman->GetFrameHeight() / 2, 
		rockman->GetFrameX(), rockman->GetFrameY());
	
	// test
	if (checkRect) {
		for (int i = 0; i < 3; i++) {
			RectangleMake(memDC, ground[i]);
		}
		//RectangleMake(memDC, rockman->GetFrameBoundingBox());
	}

	if (checkStatus) {
		char str[128];
		sprintf_s(str, "x : %f, y : %f", rockman->GetX(), rockman->GetY());
		TextOut(memDC, 10, 10, str, strlen(str));

		sprintf_s(str, "gravity : %f", gravity);
		TextOut(memDC, 10, 30, str, strlen(str));

		switch (rockmanInfo.state)
		{
		case STATE_LEFTIDLE:
			sprintf_s(str, "상태 : 왼쪽 IDLE");
			break;
		case STATE_RIGHTIDLE:
			sprintf_s(str, "상태 : 오른쪽 IDLE");
			break;
		case STATE_LEFTJUMP:
			sprintf_s(str, "상태 : 왼쪽 점프");
			break;
		case STATE_RIGHTJUMP:
			sprintf_s(str, "상태 : 오른쪽 점프");
			break;
		case STATE_LEFTWALLCATCH:
			sprintf_s(str, "상태 : 왼쪽 벽 잡기");
			break;
		case STATE_LEFTWALK:
			sprintf_s(str, "상태 : 왼쪽 걷기");
			break;
		case STATE_RIGHTWALLCATCH:
			sprintf_s(str, "상태 : 오른쪽 벽 잡기");
			break;
		case STATE_RIGHTWALK:
			sprintf_s(str, "상태 : 오른쪽 걷기");
			break;
		default:
			break;
		}

		TextOut(memDC, 10, 50, str, strlen(str));
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
