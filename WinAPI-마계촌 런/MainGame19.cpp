#include "stdafx.h"
#include "MainGame19.h"


MainGame19::MainGame19()
{
}


MainGame19::~MainGame19()
{
}

HRESULT MainGame19::Init()
{
	GameNode::Init();

	isDebug = false;
	vector = 0;

	bg = new Image;
	bg->Init("images/bg.bmp", WINSIZEX, WINSIZEY);

	arthur = new Image;
	arthur->Init("images/arthur.bmp", 
		WINSIZEX, WINSIZEY * 2, 8, 10, false, RGB(255,0,255));
	arthur->SetFrameX(STATUS_WALK);
	arthur->SetFrameY(vector);
	arthur->SetX(306);
	arthur->SetY(532);

	monster[0] = new Image;
	monster[0]->Init("images/monster1.bmp",
		WINSIZEX / 10, WINSIZEY / 6 + 10, true, RGB(181, 184, 182));
	monster[1] = new Image;
	monster[1]->Init("images/monster2.bmp",
		WINSIZEX / 10, WINSIZEY / 6 + 10, true, RGB(181, 184, 182));
	/*monster[2]->Init("images/monster3.bmp",
		WINSIZEX / 10, WINSIZEY / 6, true, RGB(181, 184, 182));*/

	AddMonster(0);

	box = { 0, 601, WINSIZEX, WINSIZEY };

	loopX = 0;

	score = 0;
	oldScore = 0;

	isNextLevel = false;
	isOver = false;
	isStart = false;

	prevTime = GetTickCount();

	return S_OK;
}

void MainGame19::Release()
{
	GameNode::Release();

	SAFE_DELETE(bg);
	SAFE_DELETE(arthur);
	for(int i=0; i < 3; i++)
		SAFE_DELETE(monster[i]);
}

void MainGame19::Update()
{
	GameNode::Update();

	if (INPUT->GetKeyDown(VK_RETURN)) { isStart = true; }
	if (!isStart) return;

	curTime = GetTickCount();
	if (curTime - prevTime > 5000) {
		isNextLevel = !isNextLevel;

		if (isNextLevel) {
			if (score < 300)
				AddMonster(0);
			else
				AddMonster(1);
		}

		prevTime = GetTickCount();
	}

 	switch (status)
	{
	case STATUS_WALK:
		moveFrame += 0.1f;
		if (moveFrame >= 4)
			moveFrame = 0;
		Jump();
		break;
	case STATUS_IDLE:
		moveFrame += 0.1f;
		if ((int)moveFrame > STATUS_IDLE) {
			status = STATUS_WALK;
			moveFrame = 0;
		}
		Jump();
		break;
	case STATUS_JUMPUP:
		arthur->SetY(arthur->GetY() + vy);
		vy += GRAVITY;
		if (vy >= 0) {
			status = STATUS_JUMPDOWN;
			moveFrame += 1;
		}
		break;
	case STATUS_JUMPDOWN:
		arthur->SetY(arthur->GetY() + vy);
		vy += GRAVITY;

		if (IntersectRect(&temp, &box, &arthur->GetFrameBoundingBox())) {
			arthur->SetY(box.top - arthur->GetFrameHeight() / 2);
			moveFrame = STATUS_IDLE;
			status = STATUS_IDLE;
		}

		break;
	default:
		break;
	}

	arthur->SetFrameX((int)moveFrame);

	// loop 렌더
	loopX += WINSIZEX / 1000;
	if (loopX >= WINSIZEX)
		loopX = 0;

	// monster move
	for (int i = 0; i < monsterInfo.size(); i++) {
		monsterInfo[i].x -= monsterInfo[i].speed;
		if (monsterInfo[i].x
			+ monster[monsterInfo[i].monsterNumber]->GetWidth() / 2 < 0) {
			monsterInfo[i].x = WINSIZEX
				+ monster[monsterInfo[i].monsterNumber]->GetWidth() / 2;
			score += 100;
		}
	}

	// monster collision
	for (int i = 0; i < monsterInfo.size(); i++) {
		if (IntersectRect(&temp,
			&RectMakeCenter(
				arthur->GetX(), arthur->GetY(),
				arthur->GetFrameWidth() / 2,
				arthur->GetFrameHeight() / 2),
			&RectMakeCenter(
				monsterInfo[i].x, monsterInfo[i].y,
				monster[monsterInfo[i].monsterNumber]->GetWidth() / 2,
				monster[monsterInfo[i].monsterNumber]->GetHeight() / 2))) {
			KillTimer(g_hWnd, 1);
			isNextLevel = false;
			isOver = true;
		}
	}


	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}

	if (INPUT->GetKeyDown(VK_CONTROL)) {
		//arthur->SetFrameX(arthur->GetFrameX() + 1);
		//monster1->SetFrameX(monster1->GetFrameX() + 1);
	}
	//==================================================//
}

void MainGame19::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		//bg->Render(memDC);
		// 원래 그려져야 할 부분
		bg->Render(memDC, 0, 0, loopX, 0, WINSIZEX, WINSIZEY);
		// 잘려질 부분 *잘려진 부분은 오른쪽부터 채워져나가야 함 -> WINSIZEX에서 빼 줌
		bg->Render(memDC, WINSIZEX - loopX, 0, 0, 0, loopX, WINSIZEY);

		// 홀수 번째 이미지 마젠타 제거 분홍색
		//arthur->SetTransColor(true, RGB(117, 146, 252));
		// 짝수 번째 이미지 마젠타 제거 파란색
		//arthur->SetTransColor(true, RGB(252, 105, 216));
		// 몇몇 이미지 마젠타 제거 초록색 쓸일 없음
		//arthur->SetTransColor(true, RGB(167, 226, 110));
		//arthur->Render(memDC);
		
		if(arthur->GetFrameX() % 2 == 0)
			arthur->SetTransColor(true, RGB(252, 105, 216));
		else
			arthur->SetTransColor(true, RGB(117, 146, 252));

		arthur->FrameRender(memDC, 
			arthur->GetX() - arthur->GetFrameWidth() / 2, 
			arthur->GetY() - arthur->GetFrameHeight() / 2);

		//monster[0]->SetTransColor(false, RGB(117, 146, 252));

		//monster[0]->Render(memDC, 
		//	monster[0]->GetX() - monster[0]->GetWidth() / 2, 
		//	monster[0]->GetY() - monster[0]->GetHeight() / 2);
		//monster[1]->Render(memDC,
		//	monster[1]->GetX() - monster[1]->GetWidth() / 2,
		//	monster[1]->GetY() - monster[1]->GetHeight() / 2);

		for (int i = 0; i < monsterInfo.size(); i++) {
			monster[monsterInfo[i].monsterNumber]->Render(memDC,
				monsterInfo[i].x
				- monster[monsterInfo[i].monsterNumber]->GetWidth() / 2, 
				monsterInfo[i].y
				- monster[monsterInfo[i].monsterNumber]->GetHeight() / 2);
		}

		// 투명한 배경색상 문자 출력후에도 배경 안바뀜
		SetBkMode(memDC, TRANSPARENT);
		// 문자색 변경
		SetTextColor(memDC, RGB(255, 255, 255));

		HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

		if (!isStart) {
			sprintf_s(str, "게임 시작 엔터");
			TextOut(memDC, WINSIZEX / 2, WINSIZEY / 2 - 50, str, strlen(str));
		}

		if (isNextLevel) {
			sprintf_s(str, "다음 레벨 몬스터 추가");
			TextOut(memDC, WINSIZEX / 2, WINSIZEY / 2 - 50, str, strlen(str));
		}

		if (isOver) {
			sprintf_s(str, "게임 오버");
			TextOut(memDC, WINSIZEX / 2, WINSIZEY / 2 - 50, str, strlen(str));
		}

		SelectObject(memDC, OldFont);
		DeleteObject(hFont);

		HFONT hFont2 = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		HFONT OldFont2 = (HFONT)SelectObject(memDC, hFont2);

		sprintf_s(str, "Score : %d", score);
		TextOut(memDC, 10, 10, str, strlen(str));

		SelectObject(memDC, OldFont2);
		DeleteObject(hFont2);
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "mouse x : %d , y : %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(memDC, 10, 10, str, strlen(str));

		//sprintf_s(str, "image x : %f , y : %f", arthur->GetX(), arthur->GetY());
		//TextOut(memDC, 10, 30, str, strlen(str));

		//RectangleMake(memDC, arthur->GetFrameBoundingBox());
		RectangleMake(memDC, RectMakeCenter(
			arthur->GetX(), arthur->GetY(),
			arthur->GetFrameWidth() / 2,
			arthur->GetFrameHeight() / 2));
		for (int i = 0; i < monsterInfo.size(); i++) {
			RectangleMake(memDC, RectMakeCenter(
				monsterInfo[i].x, monsterInfo[i].y,
				monster[monsterInfo[i].monsterNumber]->GetWidth() / 2,
				monster[monsterInfo[i].monsterNumber]->GetHeight() / 2));
		}
		//RectangleMake(memDC, box);
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame19::Jump()
{
	if (INPUT->GetKeyDown(VK_SPACE)) {
		moveFrame = 5;
		vy = JUMP;
		status = STATUS_JUMPUP;
	}
}

void MainGame19::AddMonster(int number)
{
	tagMonsterInfo temp;		
	temp.monsterNumber = number;
	temp.x = WINSIZEX
		+ monster[temp.monsterNumber]->GetWidth() / 2;
	temp.y = 475
		+ monster[temp.monsterNumber]->GetHeight() / 2;
	temp.speed = SPEED;

	monsterInfo.push_back(temp);
}
