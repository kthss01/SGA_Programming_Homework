#include "stdafx.h"
#include "MainGame22.h"
#include "GameScene1.h"

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

	SCENE->ChangeScene("Minesweeper");

	return S_OK;
}

void MainGame22::Release()
{
	GameNode::Release();
	SCENE->Release();
}

void MainGame22::Update()
{
	GameNode::Update();

	SCENE->Update();

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
		SCENE->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), 10, 10, str, strlen(str));
	}
	//=================================================
	this->SetBackBuffer()->Render(GetHDC());
}
