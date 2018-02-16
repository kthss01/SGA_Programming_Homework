#include "stdafx.h"
#include "MainGame2.h"


MainGame2::MainGame2()
{
	Init();
}


MainGame2::~MainGame2()
{
}

void MainGame2::SetupLevelInfo()
{
	lvInfo[0] = { 20, 50, 10.0f };
	lvInfo[1] = { 50, 20, 10.0f };
	lvInfo[2] = { 50, 20, 20.0f };
	lvInfo[3] = { 100, 10, 30.0f };
}

void MainGame2::SetupRectInfo()
{
	if (currentLevel == LEVEL_END) {
		for (int i = 0; i < lvInfo[currentLevel].count; i++) {
			int size = RND->GetFromInto(
				lvInfo[currentLevel].size,
				lvInfo[currentLevel].size + 10);
			rcInfo[i].rect = RectMakeCenter
			(
				RND->GetInt(WINSIZEX),
				RND->GetFromInto(-300, -50),
				size,
				size
			);
			rcInfo[i].speed =
				RND->GetFromInto(
					lvInfo[currentLevel].speed - 10.0f,
					lvInfo[currentLevel].speed + 20.0f);
		}
	}
	else {
		for (int i = 0; i < lvInfo[currentLevel].count; i++) {
			rcInfo[i].rect = RectMakeCenter
			(
				RND->GetInt(WINSIZEX),
				RND->GetFromInto(-500, -50),
				lvInfo[currentLevel].size,
				lvInfo[currentLevel].size
			);
			rcInfo[i].speed =
				RND->GetFromInto(
					lvInfo[currentLevel].speed - 3.0f,
					lvInfo[currentLevel].speed + 3.0f);
		}
	}
}

HRESULT MainGame2::Init()
{
	GameNode::Init();

	SetupLevelInfo();

	currentLevel = LEVEL_ZERO;

	SetupRectInfo();

	player = RectMake(WINSIZEX / 2 - 50, WINSIZEY - 100, 30, 30);

	isLevelUp = true;
	isGameOver = false;

	timeCount = 0;
	prevTime = GetTickCount();

	score = 0;

	return S_OK;
}

void MainGame2::Release()
{
	GameNode::Release();
}

void MainGame2::Update()
{
	GameNode::Update();

	if (isLevelUp) {
		if (INPUT->GetKeyDown(VK_SPACE))
			isLevelUp = false;
		return;
	}

	if (INPUT->GetKey(VK_LEFT)) {
		player.left -= 3.0f;
		player.right -= 3.0f;
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		player.left += 3.0f;
		player.right += 3.0f;
	}

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {

		for (int i = 0; i < lvInfo[currentLevel].count; i++) {
			// rect가 바닥에 닿으면 랜덤한 위치에 다시 만들어서 떨어지게함
			if (rcInfo[i].rect.bottom >= WINSIZEY) {
				rcInfo[i].rect = rcInfo[i].rect = RectMakeCenter
				(
					RND->GetInt(WINSIZEX),
					0,
					lvInfo[currentLevel].size,
					lvInfo[currentLevel].size
				);
			}

			rcInfo[i].rect.top += rcInfo[i].speed;
			rcInfo[i].rect.bottom += rcInfo[i].speed;
		}

		timeCount++;

		// 0.1초마다 점수 10점씩 증가
		score += 10;

		// 10초마다 Level 올림
		if (timeCount % 100 == 0) {
			if (currentLevel != LEVEL_END) {
				currentLevel = (LEVEL)(currentLevel + 1);
				SetupRectInfo();
				isLevelUp = true;
			}
		}

		prevTime = GetTickCount();
	}

	for (int i = 0; i < lvInfo[currentLevel].count; i++) {
		RECT temp;
		if (IntersectRect(&temp, &player, &rcInfo[i].rect)) {
			isGameOver = true;
			this->Release();
			break;
		}
	}
}

void MainGame2::Render(HDC hdc)
{
	GameNode::Render(hdc);

	for (int i = 0; i < lvInfo[currentLevel].count; i++) {
		RectangleMake(hdc, rcInfo[i].rect);
		//EllipseMake(hdc, rcInfo[i].rect);
	}

	RectangleMake(hdc, player);

	char str[128];
	sprintf_s(str, "점수 : %d", score);
	TextOut(hdc, WINSIZEX/ 2 - 50, 100, str, strlen(str));

	if (isGameOver) {
		TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, 
			"게임 오버", strlen("게임 오버"));
	}

	if (isLevelUp) {
		TextOut(hdc, WINSIZEX / 2 - 85, WINSIZEY / 2,
			"시작하려면 Space", strlen("시작하려면 Space"));
		switch (currentLevel)
		{
		case LEVEL_ZERO:
			TextOut(hdc, WINSIZEX / 2 - 85, WINSIZEY / 2 - 50,
				"똥 피하기 게임", strlen("똥 피하기 게임"));
			break;
		case LEVEL_ONE:
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50,
				"레벨 업!", strlen("레벨 업!"));
			TextOut(hdc, WINSIZEX / 2 - 85, WINSIZEY / 2 + 50,
				"갯수 증가, 크기 감소", strlen("갯수 증가, 크기 감소"));
			break;
		case LEVEL_TWO:
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50,
				"레벨 업!", strlen("레벨 업!"));
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 + 50,
				"속도 증가", strlen("속도 증가"));
			break;
		case LEVEL_END:
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50,
				"레벨 업!", strlen("레벨 업!"));
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 + 50,
				"속도 크기 랜덤", strlen("속도 크기 랜덤"));
			break;
		default:
			break;
		}
	}
}
