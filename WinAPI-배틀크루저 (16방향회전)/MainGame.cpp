#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "StarcraftScene.h"

MainGame::MainGame()
{
#ifdef SUBWINOPEN
	SUBWIN->Init();
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거
}

MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	SOUND->Init();
	isDebug = false;

	////////////////////////////////////////////////////////
	IMAGE->AddImage("bg", "images/bg2.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));
	
	IMAGE->AddImage("cursor", "images/cursor.bmp", 0, 0,
		20, 20, true, RGB(255, 0, 255));
	
	IMAGE->AddImage("Battle", "images/battle.bmp", 0, 0,
		1536, 79, 16, 1, true, RGB(255, 0, 255));
	////////////////////////////////////////////////////////
	TestScene * test = new TestScene;
	SCENE->AddScene("Test", test);
	
	SCENE->AddScene("Starcraft", new StarcraftScene);

	////////////////////////////////////////////////////////
	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);

	SOUND->AddSound("battle_move1", 
		"sounds/battleCru/BattleCru_Starc1_1.mp3", false, false);
	SOUND->AddSound("battle_move2",
		"sounds/battleCru/BattleCru_Starc1_2.mp3", false, false);
	SOUND->AddSound("battle_move3",
		"sounds/battleCru/BattleCru_Starc1_3.mp3", false, false);
	SOUND->AddSound("battle_move4",
		"sounds/battleCru/BattleCru_Starc1_4.mp3", false, false);
	////////////////////////////////////////////////////////
#ifdef SUBWINOPEN
	SUBWIN->SetScene(test);
#endif // SUBWINDOW 필요시 MainGame.h 주석 제거

	//SCENE->ChangeScene("Test");
	SCENE->ChangeScene("Starcraft");

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
