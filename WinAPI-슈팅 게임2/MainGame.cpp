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

	IMAGE->AddImage("intro", "images/intro.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	IMAGE->AddImage("bg", "images/background.bmp", 0, -WINSIZEY,
		WINSIZEX, WINSIZEY * 2, false, RGB(255, 0, 255));

	IMAGE->AddImage(
		"rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));
	IMAGE->AddImage(
		"shadow", "images/shadow.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));

	IMAGE->AddImage("bullet", "images/bullet_blue_10x1.bmp", 0, 0,
		25 * 10, 25, 10, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("alien", "images/alien_idle_16x1.bmp", 0, 0,
		50 * 16, 50, 16, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("missile", "images/missile.bmp",
		0, 0, 26, 124, true, RGB(255, 0, 255));

	_em = new EnemyManager;
	_em->Init();

	_em->SetRocket((Rocket*)SCENE->AddScene("Rocket", new Rocket));

	//SCENE->ChangeScene("Rocket");

	//SCENE->AddScene("Test", new TestScene);

	//SCENE->ChangeScene("Test");

	offsetY = 0;

	isStart = false;

	return S_OK;
}

void MainGame::Release()
{
	GameNode::Release();
	_em->Release();
	SAFE_DELETE(_em);
}

void MainGame::Update()
{
	GameNode::Update();

	if(isStart)
		_em->Update();

	SCENE->Update();


	if (INPUT->GetKeyDown(VK_RETURN) && !isStart) {
		isStart = true;
		SCENE->ChangeScene("Rocket");
	}

	if (isStart) {
		offsetY--;
	}

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
		if (isStart) {
			IMAGE->FindImage("bg")->LoopRender(GetMemDC(),
				&RectMake(0, 0, WINSIZEX, WINSIZEY), 0, offsetY);
		}
		else
			IMAGE->FindImage("intro")->Render(GetMemDC());

		SCENE->Render();

		if(isStart)
			_em->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	this->SetBackBuffer()->Render(GetHDC());
}
