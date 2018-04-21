#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"

#include "IsoMap.h"
#include "SubIsoMap.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	SOUND->Init();
#ifdef SUBWINOPEN
	SUBWIN->Init();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거

	isDebug = false;

	IMAGE->AddImage("tile_grass", "images/tile_grass.bmp", 
		0, 0, 100, 65, true, RGB(255, 0, 255));
	IMAGE->AddImage("tile", "images/tile.bmp",
		0, 0, 100 * 8, (50 + 50) * 7, 8, 7, true, RGB(255, 0, 255));
	IMAGE->AddImage("tile2", "images/tile2.bmp",
		0, 0, 100 * 21, (50 + 50) * 8, 21, 8, true, RGB(255, 0, 255));

	TestScene * test = new TestScene;
	SCENE->AddScene("Test", test);
	
	SubIsoMap * sub = new SubIsoMap;
	SCENE->AddScene("SubIsoMap", sub);

	IsoMap * map = new IsoMap;
	SCENE->AddScene("IsoMap", map);

	SUBWIN->SetIsoMap(map);

	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

#ifdef SUBWINOPEN
	SUBWIN->SetScene(sub);
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거

	SCENE->ChangeScene("IsoMap");

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

#ifdef SUBWINOPEN
	SUBWIN->Update();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거

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

#ifdef SUBWINOPEN
		SUBWIN->Render();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거
		
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}
