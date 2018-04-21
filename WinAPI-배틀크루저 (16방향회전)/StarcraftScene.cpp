#include "stdafx.h"
#include "StarcraftScene.h"

// ¾È½áµµ µÊ
//#include "Ship.h"
#include "BattleCruiser.h"

StarcraftScene::StarcraftScene()
{
}


StarcraftScene::~StarcraftScene()
{
}

HRESULT StarcraftScene::Init()
{
	_battle = new BattleCruiser;
	_battle->Init("Battle", WINSIZEX / 2, WINSIZEY / 2);

	ShowCursor(false);

	isDebug = false;

	return S_OK;
}

void StarcraftScene::Release()
{
	SAFE_DELETE(_battle);
}

void StarcraftScene::Update()
{
	_battle->Update();

	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
		ShowCursor(isDebug);
	}
}

void StarcraftScene::Render()
{
	IMAGE->Render("bg", GetMemDC(), 0, 0);

	IMAGE->Render("cursor", GetMemDC(), g_ptMouse.x - 3, g_ptMouse.y);

	_battle->Render();

	if (isDebug) {
		char str[128];
		sprintf_s(str, "mouse pt : %d %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), 0, 60, str, strlen(str));
	}
}
