#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "Rocket.h"

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

	IMAGE->AddImage("bg", "images/background.bmp", 0, -WINSIZEY,
		WINSIZEX, WINSIZEY * 2, false, RGB(255, 0, 255));

	SCENE->AddScene("Test", new TestScene);
	SCENE->AddScene("Rocket", new Rocket);

	//SCENE->ChangeScene("Test");
	SCENE->ChangeScene("Rocket");

	offsetY = 0;

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

	offsetY--;

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


		IMAGE->FindImage("bg")->LoopRender(GetMemDC(),
			&RectMake(0, 0, WINSIZEX, WINSIZEY), 0, offsetY);

		SCENE->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	this->SetBackBuffer()->Render(GetHDC());
}
