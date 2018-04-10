#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"

#include "ToolMain.h"
#include "ToolSub.h"

MainGame::MainGame()
{
	//SUBWIN->Init();
}


MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	SOUND->Init();
	SUBWIN->Init();
	isDebug = false;

	IMAGE->AddImage("tile_main", "images/tile.bmp", 0, 0,
		21 * 25, 16 * 25, 21, 16, true, RGB(255, 0, 255));
	IMAGE->AddImage("tile_sub", "images/tile.bmp", 0, 0,
		21 * 20, 16 * 20, 21, 16, true, RGB(255, 0, 255));
	IMAGE->AddImage("tile_mini", "images/tile.bmp", 0, 0,
		21 * 5, 16 * 5, 21, 16, true, RGB(255, 0, 255));

	IMAGE->AddImage("toolbar", "images/toolbar_3x1.bmp", 0, 0,
		3 * 50, 50, 3, 1, true, RGB(255, 0, 255));
	IMAGE->AddImage("miniMap", "images/miniMap.bmp", 0, 0,
		35, 35, true, RGB(255, 0, 255));
	IMAGE->AddImage("camera", "images/camera.bmp", 0, 0,
		200, 150, true, RGB(255, 0, 255));
	IMAGE->AddImage("restore", "images/restore.bmp", 0, 0,
		50, 50, true, RGB(255, 0, 255));

	IMAGE->AddImage("test", "images/miniMap.bmp", 0, 0,
		WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	//TestScene * test = new TestScene;
	//SCENE->AddScene("Test", test);
	//SOUND->AddSound("Test", "sounds/¿µÀü3.wav", true, true);
	//SUBWIN->SetScene(test);
	//SCENE->ChangeScene("Test");
	

	ToolMain * main = (ToolMain*)SCENE->AddScene("MapTool_Main", new ToolMain);
	ToolSub * sub = (ToolSub*)SCENE->AddScene("MapTool_Sub", new ToolSub);
	SUBWIN->SetScene(sub);
	sub->SetToolMain(main);

	SCENE->ChangeScene("MapTool_Main");

	return S_OK;
}

void MainGame::Release()
{
	GameNode::Release();
}

void MainGame::Update()
{
	GameNode::Update();
	SUBWIN->Update();
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
		//IMAGE->Render("test", GetMemDC(), 0, 0);

		SCENE->Render();
		SUBWIN->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}
