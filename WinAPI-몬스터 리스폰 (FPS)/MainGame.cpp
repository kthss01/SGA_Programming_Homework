#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	isDebug = false;


	IMAGE->AddImage("bg", "images/bg.bmp", 
		0, 0, WINSIZEX - PADDINGX, WINSIZEY, false, RGB(255, 0, 255));

	IMAGE->AddImage("pokemon", "images/pokemon.bmp", 0, 0,
		100 * 12, 100 * 13, 12, 13, true, RGB(255, 0, 255));

	IMAGE->AddImage("bullet", "images/bullet_blue_10x1.bmp", 0, 0,
		25 * 10, 25, 10, 1, true, RGB(255, 0, 255));

	//char name[64];
	//char path[128];
	//
	//for (int i = 0; i < 10; i++) {
	//	sprintf_s(name, "pokemon_%d", i + 1);
	//	sprintf_s(path, "images/pokemon (%d).bmp", i + 1);
	//	IMAGE->AddImage(name, path, 0, 0, 50, 50, true, RGB(255, 255, 255));
	//}

	SCENE->AddScene("Test", new TestScene);

	SCENE->ChangeScene("Test");

	return S_OK;
}

void MainGame::Release()
{
	GameNode::Release();
}

void MainGame::Update()
{
	GameNode::Update();

	SCENE->Update();

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame::Render()
{
	PatBlt(GetMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		SCENE->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}
