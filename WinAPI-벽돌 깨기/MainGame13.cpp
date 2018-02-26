#include "stdafx.h"
#include "MainGame13.h"


MainGame13::MainGame13()
{
}


MainGame13::~MainGame13()
{
}

HRESULT MainGame13::Init()
{
	GameNode::Init();

	// block
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 12; j++) {
			block[i * 12 + j].x = 100 + 75 * j;
			block[i * 12 + j].y = 100 + 50 * i;
			block[i * 12 + j].rc = RectMakeCenter(
				block[i*12+j].x, block[i*12+j].y, 75, 50);
			block[i * 12 + j].life = 4 - i;
			block[i * 12 + j].isItem = false;
		}
	}
	// bar
	bar.x = WINSIZEX / 2;
	bar.y = WINSIZEY - WINSIZEY / 8;
	bar.width = 200;
	bar.height = 50;
	bar.speed = 5.0f;
	bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
	// ball
	ball.radius = 15;
	ball.speed = 0;
	ball.pt.x = WINSIZEX / 2 +
		RND->GetFromInto(-(bar.rc.right - bar.rc.left)/2, (bar.rc.right-bar.rc.left)/2);
	ball.pt.y = bar.y - (bar.rc.bottom - bar.rc.top) / 2 - ball.radius;
	//ball.angle = 45.0f * PI / 180;
	ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);

	isOver = false;

	// item
	// 블록 인덱스의 순서 섞기 랜덤한 블록 위치를 만들기 위해 
	int num[BLOCKMAX];
	for (int i = 0; i < BLOCKMAX; i++) {
		num[i] = i;
	}
	// 셔플
	for (int i = 0; i < 1000; i++) {
		int dest = RND->GetInt(BLOCKMAX);
		int sour = RND->GetInt(BLOCKMAX);
		int temp = num[dest];
		num[dest] = num[sour];
		num[sour] = temp;
	}

	for (int i = 0; i < ITEMMAX; i++) {
		item[i].isFind = false;
		// 7종류이므로 1~7 랜덤한 아이템 넣기 -> 1~6으로
		// bullet 은 일단 미구현 하기로 결정
		item[i].kind = (ItemKind)(RND->GetInt(6) + 1);
		//item[i].kind = ITEM_CATCH;
		// 랜덤한 블록의 위치에 아이템 넣기
		item[i].x = block[num[i]].x;
		item[i].y = block[num[i]].y;
		item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);
		// 아이템이 들어간 블록에 아이템 표시
		block[num[i]].isItem = true;
	}

	return S_OK;
}

void MainGame13::Release()
{
	GameNode::Release();
}

void MainGame13::Update()
{
	GameNode::Update();


	// isOver true 면 게임 종료
	if (isOver)
		KillTimer(g_hWnd, 1);

	if (INPUT->GetKey(VK_LEFT)) { 
		bar.x -= bar.speed; 
		// 왼쪽 벽 경계
		int width = bar.rc.right - bar.rc.left;
		if (bar.x - width / 2 < 0) {
			bar.x = width / 2;
		}

		// 공이 발사되지 않았을 때 (스피드가 0일때)
		if (ball.speed == 0) {
			// 볼도 바에 따라 움직임
			ball.pt.x -= bar.speed+2;
			// 발사 하진 않았으니 바에서 떨어지지는 않게
			if (ball.pt.x < bar.rc.left) {
				ball.pt.x = bar.rc.left;
			}
			// 움직임에 따라 발사 각도 조절
			ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);
		}
	}
	if (INPUT->GetKey(VK_RIGHT)) { 
		bar.x += 5.0f;
		// 오른쪽 벽 경계
		int width = bar.rc.right - bar.rc.left;
		if (bar.x + width / 2 > WINSIZEX) {
			bar.x = WINSIZEX - width / 2;
		}
		// 공이 발사되지 않았을 때 (스피드가 0일때)
		if (ball.speed == 0) {
			// 볼도 바에 따라 움직임
			ball.pt.x += bar.speed+2;
			// 발사 하진 않았으니 바에서 떨어지지는 않게
			if (ball.pt.x > bar.rc.right) {
				ball.pt.x = bar.rc.right;
			}
			// 움직임에 따라 발사 각도 조절
			ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);
		}
	}
	bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);

	if (INPUT->GetKeyDown(VK_SPACE)) {
		ball.speed = 5.0f;
	}

	// item move
	for (int i = 0; i < ITEMMAX; i++) {
		// 발견되지 않앗음 무시
		if (item[i].isFind == false) continue;
		item[i].y += 3.0f;
		item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);

		// 바닥으로 끝까지 떨어지면 
		if (item[i].y + (item[i].rc.bottom - item[i].rc.top) / 2 > WINSIZEY)
			item[i].isFind = false;

		// 바와 충돌시 아이템 효과 적용
		RECT temp;
		if (IntersectRect(&temp, &item[i].rc, &bar.rc)) {
			switch (item[i].kind)
			{
			case ITEM_MULTY:
				break;
			case ITEM_FAST:
				ball.speed += 3.0f;
				break;
			case ITEM_SLOW:
				ball.speed -= 3.0f;
				if (ball.speed <= 1)
					ball.speed = 1;
				break;
			case ITEM_LONG:
				bar.width += 30;
				bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
				break;
			case ITEM_SHORT:
				bar.width -= 30;
				bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
				break;
			case ITEM_CATCH:
				ball.speed = 0;
				ball.pt.x = bar.x +
					RND->GetFromInto(-(bar.rc.right - bar.rc.left) / 2, (bar.rc.right - bar.rc.left) / 2);
				ball.pt.y = bar.y - (bar.rc.bottom - bar.rc.top) / 2 - ball.radius;
				break;
			case ITEM_BULLET:
				break;
			}

			// 효과 적용시 다시 발견되지 않음으로해서 렌더 및 검사에서 제외
			item[i].isFind = false;
		}
	}

	// ball move
	ball.pt.x += cosf(ball.angle) * ball.speed;
	// 0.05f 는 중력값 적용 결국 떨어질 수 있도록 -> 필요 없을듯
	ball.pt.y += -sinf(ball.angle) * ball.speed;
	
	// ball 벽 충돌
	// left
	if (ball.pt.x - ball.radius < 0) {
		ball.pt.x = ball.radius;
		ball.angle = PI - ball.angle;
	}
	// right
	if (ball.pt.x + ball.radius > WINSIZEX) {
		ball.pt.x = WINSIZEX - ball.radius;
		ball.angle = PI - ball.angle;
	}
	// top
	if (ball.pt.y - ball.radius < 0) {
		ball.pt.y = ball.radius;
		ball.angle = -ball.angle;
	}
	// bottom
	if (ball.pt.y + ball.radius > WINSIZEY) {
		//ball.pt.y = WINSIZEY - ball.radius;
		//ball.angle = -ball.angle;
		isOver = true;
	}

	// ball bar 충돌
	if (RectInCircle(&bar.rc, ball.pt, ball.radius)) {
		// 이 방법은 아닌듯 이상하게 튕겨나감 벽충돌처럼 구현해야할 듯
		// bal bar 충돌은 이 방법이 맞는거 같기도함 그래야 컨트롤 가능하니
		ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);

		//// 이 방법이 그나마 유력함 -> 맞은듯 -> 틀린듯
		//// bar 왼쪽
		//if (ball.pt.x < bar.rc.left) {
		//	ball.pt.x = bar.rc.left - ball.radius;
		//	ball.angle = PI - ball.angle;
		//}
		//// bar 오른쪽
		//if (ball.pt.x > bar.rc.right) {
		//	ball.pt.x = bar.rc.right + ball.radius;
		//	ball.angle = PI - ball.angle;
		//}
		//// bar 위쪽
		//if (ball.pt.y < bar.rc.top) {
		//	ball.pt.y = bar.rc.top - ball.radius;
		//	ball.angle = -ball.angle;
		//}
		//// bar 아래쪽
		//if (ball.pt.y > bar.rc.bottom) {
		//	ball.pt.y = bar.rc.bottom + ball.radius;
		//	ball.angle = -ball.angle;
		//}
	}

	// ball block 충돌
	for (int i = 0; i < BLOCKMAX; i++) {
		// life 0인 경우 무시
		if (block[i].life == 0) continue;

		// 충돌시
		if (RectInCircle(&block[i].rc, ball.pt, ball.radius)) {
			// 블락 라이프 1감소
			block[i].life -= 1;
			// 블락이 아이템을 가지고 있으며 라이프가 0이 되었을 때
			if (block[i].isItem && block[i].life == 0) {
				// 아이템을 찾음
				for (int j = 0; j < ITEMMAX; j++) {
					// 발견된 아이템 무시
					if (item[j].isFind == true) continue;

					// 블락의 위치와 아이템의 위치가 같을 때 아이템 발견
					if (block[i].x == item[j].x && block[i].y == item[j].y) {
						item[j].isFind = true;
						break;
					}
				}
			}
			// 볼 각도 변경
			// 일단은 이 방법으로
			//ball.angle = GetAngle(block[i].x, block[i].y, ball.pt.x, ball.pt.y);
			// block 왼쪽
			if (ball.pt.x < block[i].rc.left) {
				ball.pt.x = block[i].rc.left - ball.radius;
				ball.angle = PI - ball.angle;
			}
			// bar 오른쪽
			if (ball.pt.x > block[i].rc.right) {
				ball.pt.x = block[i].rc.right + ball.radius;
				ball.angle = PI - ball.angle;
			}
			// bar 위쪽
			if (ball.pt.y < block[i].rc.top) {
				ball.pt.y = block[i].rc.top - ball.radius;
				ball.angle = -ball.angle;
			}
			// bar 아래쪽
			if (ball.pt.y > block[i].rc.bottom) {
				ball.pt.y = block[i].rc.bottom + ball.radius;
				ball.angle = -ball.angle;
			}
		}
	}
}

void MainGame13::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	for (int i = 0; i < BLOCKMAX; i++) {
		if (block[i].life == 0) continue;
		HBRUSH brush = NULL;
		HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, brush);
		switch (block[i].life) {
		case 1:
			if (block[i].isItem)
				// 무늬까지 추가로 넣어주는 함수
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0));
			else
				brush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 2:
			if (block[i].isItem)
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 0));
			else
				brush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 3:
			if(block[i].isItem)
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 255));
			else
				brush = CreateSolidBrush(RGB(0, 0, 255));
			break;
		case 4:
			if (block[i].isItem)
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(127, 127, 127));
			else
				brush = CreateSolidBrush(RGB(127, 127, 127));
			break;
		}
		RectangleMake(memDC,
			block[i].rc.left - 1,
			block[i].rc.top - 1,
			block[i].rc.right + 1,
			block[i].rc.bottom + 1);
		FillRect(memDC, &block[i].rc, brush);
		SelectObject(memDC, oldBrush);
		DeleteObject(brush);
	}

	char str[128];

	for (int i = 0; i < ITEMMAX; i++) {
		// 테스트 위해서 잠시 보이게 둔거
		//if (item[i].isFind == false) continue;
		RectangleMake(memDC, item[i].rc);
		switch (item[i].kind)
		{
		case ITEM_MULTY:
			sprintf_s(str, "Multi");
			break;
		case ITEM_FAST:
			sprintf_s(str, "Speed");
			break;
		case ITEM_SLOW:
			sprintf_s(str, "Slow");
			break;
		case ITEM_LONG:
			sprintf_s(str, "Long");
			break;
		case ITEM_SHORT:
			sprintf_s(str, "Short");
			break;
		case ITEM_CATCH:
			sprintf_s(str, "Catch");
			break;
		case ITEM_BULLET:
			sprintf_s(str, "Bullet");
			break;
		}
		TextOut(memDC, item[i].x - (item[i].rc.right - item[i].rc.left)/2 + 5, 
			item[i].y - 5, str, strlen(str));
	}

	RectangleMake(memDC, bar.rc);

	EllipseMakeCenter(memDC, ball.pt.x, ball.pt.y, ball.radius);

	if (isOver) {
		HFONT font = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		HFONT OldFont = (HFONT)SelectObject(memDC, font);
		
		TextOut(memDC, WINSIZEX / 2 - 150,  WINSIZEY / 2, "Game Over", strlen("Game Over"));

		SelectObject(memDC, OldFont);
		DeleteObject(font);
	}

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

bool MainGame13::RectInCircle(RECT * rect, POINT circleCenter, float radius)
{
	if (rect->left <= circleCenter.x && circleCenter.x <= rect->right
		|| rect->top <= circleCenter.y && circleCenter.y <= rect->bottom) {
		RECT rcEx = {
			rect->left - radius,
			rect->top - radius,
			rect->right + radius,
			rect->bottom + radius
		};

		return PtInRect(&rcEx, circleCenter);
	}
	// 모서리
	else {
		float distance[4];
		// left top
		distance[0] = GetDistance(circleCenter.x, circleCenter.y,
			rect->left, rect->top);
		// right top
		distance[1] = GetDistance(circleCenter.x, circleCenter.y,
			rect->right, rect->top);
		// left bottom
		distance[2] = GetDistance(circleCenter.x, circleCenter.y,
			rect->left, rect->bottom);
		distance[3] = GetDistance(circleCenter.x, circleCenter.y,
			rect->right, rect->bottom);
		for (int i = 0; i < 4; i++) {
			if (distance[i] < radius)
				return true;
		}
	}

	return false;
}
