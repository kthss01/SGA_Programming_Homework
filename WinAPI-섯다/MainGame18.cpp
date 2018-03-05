#include "stdafx.h"
#include "MainGame18.h"


MainGame18::MainGame18()
{
}


MainGame18::~MainGame18()
{
}

HRESULT MainGame18::Init()
{
	GameNode::Init();

	logo = new Image;
	logo->Init("images/logo.bmp", WINSIZEX / 2 , WINSIZEY / 2);

	card = new Image;
	card->Init("images/card.bmp", WINSIZEX / 4 * 3 - 70, WINSIZEY / 4 + 20, 10, 2);


	for (int i = 0; i < BG_SIZE; i++) {
		bg[i] = new Image;
		if (i == 24)
			sprintf_s(str, "images/betting.bmp");
		else
			sprintf_s(str, "images/shuffle%d.bmp", i + 1);
		bg[i]->Init(str, WINSIZEX, WINSIZEY);
	}

	for (int i = 0; i < 14; i++) {
		combi[i] = new Image;
		sprintf_s(str, "images/combi%d.bmp", i + 1);
		combi[i]->Init(str, WINSIZEX / 4 - 25, WINSIZEY / 4);
	}

	return S_OK;
}

void MainGame18::Release()
{
	GameNode::Release();
	SAFE_DELETE(logo);
	SAFE_DELETE(card);
	for (int i = 0; i < BG_SIZE; i++) {
		SAFE_DELETE(bg[i]);
	}
	for (int i = 0; i < COMBI_SIZE; i++) {
		SAFE_DELETE(combi[i]);
	}
}

void MainGame18::Update()
{
	GameNode::Update();

	if (INPUT->GetKeyDown(VK_CONTROL)) { isCombi = !isCombi; }

	if (INPUT->GetKeyDown(VK_SPACE)) { moveFrame += 1.0f; }
	

	// 애니메이션 test
	{
		// BG test
		//if(moveFrame < BG_SIZE-1)
		//	moveFrame += 0.1f;

		// COMBI test
		//if (moveFrame < COMBI_SIZE - 1)
		//	moveFrame += 0.1f;

		//// card test
		//if (moveFrame < 20)
		//	moveFrame += 0.1f;
	}
}

void MainGame18::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	// 애니메이션 렌더 test
	{
		//logo->Render(memDC, WINSIZEX / 4, WINSIZEY / 4);
		//card->Render(memDC, WINSIZEX / 4, WINSIZEY / 4);
		//bg[24]->Render(memDC);
		//// card 1 test
		//card->FrameRender(memDC, 
		//	WINSIZEX / 2 - WINSIZEX / 8 - 6, WINSIZEY - WINSIZEY / 6 - 13,
		//	(int)moveFrame % 10, (int) moveFrame / 10);
		//// card 2 test
		//card->FrameRender(memDC,
		//	WINSIZEX / 2 - WINSIZEX / 8 - 6 + card->GetFrameWidth(), WINSIZEY - WINSIZEY / 6 - 13,
		//	(int)moveFrame % 10, (int)moveFrame / 10);
		//bg[(int)moveFrame]->Render(memDC);
		//if(isCombi)
		//	combi[(int)moveFrame]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);

		// mouse x y
		//sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		//TextOut(memDC, 10, 10, str, strlen(str));

		// other card test
		/*card->FrameRender(memDC,
			38, 216 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			38 + card->GetFrameWidth(), 216 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			8, 417 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			8 + card->GetFrameWidth(), 417 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			864, 217 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			864 + card->GetFrameWidth(), 217 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			1017 - card->GetFrameWidth() * 2, 415 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			1017 - card->GetFrameWidth(), 415 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);*/

		// money text test
		// player
		//sprintf_s(str, "%4d억 %4d만 %4d냥", 1234, 5678, 9999);
		//TextOut(memDC, 324, 161, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 58, 237, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 29, 440, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 832, 237, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 862, 439, str, strlen(str));

		//// betting text test
		//sprintf_s(str, "%d만냥", 1234);
		//TextOut(memDC, 485, 501, str, strlen(str));

		//sprintf_s(str, "기본머니 %d만냥", 1000);
		//TextOut(memDC, 448, 550, str, strlen(str));

		// text test
		/*sprintf_s(str, "삼팔광땡");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 5,
			str, strlen(str));

		sprintf_s(str, " 여 덜 끗 ");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 65,
			str, strlen(str));
		
		sprintf_s(str, "   독  사   ");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 115,
			str, strlen(str));*/
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
