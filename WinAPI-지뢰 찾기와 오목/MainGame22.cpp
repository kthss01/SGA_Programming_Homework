#include "stdafx.h"
#include "MainGame22.h"
#include "GameScene1.h"
#include "GameScene2.h"
#include "NoneScene.h"

MainGame22::MainGame22()
{
}


MainGame22::~MainGame22()
{
}

HRESULT MainGame22::Init()
{
	GameNode::Init();
	isDebug = false;

	SCENE->AddScene("Minesweeper", new GameScene1);
	SCENE->AddScene("Gomoku", new GameScene2);
	SCENE->AddScene("None", new NoneScene);

	// 씬 지우기 위해 만든 빈 씬
	SCENE->ChangeScene("None");
	//SCENE->ChangeScene("Gomoku");

	bg = new Image;
	bg->Init("images/bg.bmp", WINSIZEX, WINSIZEY);

	icon1 = new Image;
	icon1->Init("images/icon1.bmp", 70, 65, true, RGB(0, 128, 128));

	icon2 = new Image;
	icon2->Init("images/icon2.bmp", 70, 65, true, RGB(0, 128, 128));

	rcMineStart = RectMake(143, 120, 73, 65 + 20);
	rcGomokuStart = RectMake(140, 240, 70, 65 + 30);

	return S_OK;
}

void MainGame22::Release()
{
	GameNode::Release();
	SCENE->Release();
	SAFE_DELETE(bg);
	SAFE_DELETE(icon1);
	SAFE_DELETE(icon2);
}

void MainGame22::Update()
{
	GameNode::Update();

	SCENE->Update();

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		// 해당 아이콘 클릭씨 씬 변화
		if (PtInRect(&rcMineStart, g_ptMouse)) {
			SCENE->ChangeScene("Minesweeper");
		}

		if (PtInRect(&rcGomokuStart, g_ptMouse)) {
			SCENE->ChangeScene("Gomoku");
		}
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame22::Render()
{
	PatBlt(GetMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		bg->Render(GetMemDC());

		icon1->Render(GetMemDC(), 145, 120);
		icon2->Render(GetMemDC(), 140, 240);

		// 투명한 배경색상 문자 출력후에도 배경 안바뀜
		SetBkMode(GetMemDC(), TRANSPARENT);
		// 문자색 변경
		SetTextColor(GetMemDC(), RGB(255, 255, 255));

		HFONT hFont = CreateFont(17, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		HFONT OldFont = (HFONT)SelectObject(GetMemDC(), hFont);

		sprintf_s(str, "지뢰찾기");
		TextOut(GetMemDC(), 143, 120 + icon1->GetHeight(), str, strlen(str));

		sprintf_s(str, "오목");
		TextOut(GetMemDC(), 158, 249 + icon2->GetHeight(), str, strlen(str));

		SelectObject(GetMemDC(), OldFont);
		DeleteObject(hFont);

		SCENE->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), 10, 10, str, strlen(str));

		RectangleMake(GetMemDC(), rcMineStart);
		RectangleMake(GetMemDC(), rcGomokuStart);
		
	}
	//=================================================
	this->SetBackBuffer()->Render(GetHDC());
}
