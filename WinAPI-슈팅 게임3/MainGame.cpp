#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "ResultScene.h"

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

	// 이미지 추가
	// 게임 시작화면
	IMAGE->AddImage("intro", "images/intro.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	// 게임 배경화면
	IMAGE->AddImage("bg", "images/background.bmp", 0, -WINSIZEY,
		WINSIZEX, WINSIZEY * 2, false, RGB(255, 0, 255));

	// 게임 오버 화면
	IMAGE->AddImage("gameover", "images/gameover.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	// 엔딩 화면
	IMAGE->AddImage("ending", "images/ending.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	// 플레이어 로켓
	IMAGE->AddImage(
		"rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));
	// 로켓 그림자
	IMAGE->AddImage(
		"shadow", "images/shadow.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));

	// 기본 총알
	IMAGE->AddImage("bullet", "images/bullet_blue_10x1.bmp", 0, 0,
		25 * 10, 25, 10, 1, true, RGB(255, 0, 255));

	// alien idle
	IMAGE->AddImage("alien_idle", "images/alien_idle_16x1.bmp", 0, 0,
		50 * 16, 50, 16, 1, true, RGB(255, 0, 255));
	// alien died
	IMAGE->AddImage("alien_died", "images/alien_died_21x1.bmp", 0, 0,
		75 * 21, 75, 21, 1, true, RGB(255, 0, 255));

	// 플레이어 총알 (미사일)
	IMAGE->AddImage("missile", "images/missile.bmp",
		0, 0, 416, 64, 13, 1, true, RGB(255, 0, 255));

	// 위치 지정 편하게 하기 위한 임시 변수 값
	int temp = 100;

	// boss 이미지 
	// 보스 총
	IMAGE->AddImage("boss_deploy", "images/boss/boss_deploy_15x1.bmp", 0, 0,
		temp / 4 * 15, 50, 15, 1, true, RGB(255, 0, 255));

	// 보스 총 발사
	IMAGE->AddImage("boss_fire", "images/boss/boss_fire_18x1.bmp", 0, 0,
		temp * 2.5 * 18, 100, 18, 1, true, RGB(255, 0, 255));

	// 보스 에너미 소환
	IMAGE->AddImage("boss_enemy", "images/boss/boss_enemy_9x1.bmp", 0, 0,
		60 * 9, 50, 9, 1, true, RGB(255, 0, 255));
	
	// 보스 해치 (
	IMAGE->AddImage("boss_hatch", "images/boss/boss_hatch_18x1.bmp", 0, 0,
		temp * 18, 25, 18, 1, true, RGB(255, 0, 255));

	// 보스 idle
	IMAGE->AddImage("boss_idle", "images/boss/boss_idle_24x1.bmp", 0, 0,
	300 * 24, 250, 24, 1, true, RGB(255, 0, 255));

	_em = new EnemyManager;
	_em->Init();

	Rocket* rocket = (Rocket*)SCENE->AddScene("Rocket", new Rocket);
	rocket->SetEnemyManager(_em); // enemyManager rocket에 포인터로 연결

	_em->SetRocket(rocket); // rocket enemyManger에 포인터로 연결

	_loading = new ProgressBar;
	//_loading->Init((char*)"images/bar_front.bmp", (char*)"images/bar_back.bmp", 
	//	WINSIZEX / 2 - 350, WINSIZEY - 100, 700, 50);

	_loading->Init(
		IMAGE->AddImage(
			"loading_front", "images/bar_front.bmp", 0, 0, 700, 50, true, RGB(255,0,255)),
		IMAGE->AddImage(
			"loading_back", "images/bar_back.bmp", 0, 0, 700, 50, true, RGB(255,0,255)),
		WINSIZEX / 2 - 350, WINSIZEY - 100, 700, 50);

	loadCount = 0;

	//SCENE->ChangeScene("Rocket");

	//SCENE->AddScene("Test", new TestScene);

	//SCENE->ChangeScene("Test");

	// 결과 씬
	ResultScene* scene = (ResultScene*)SCENE->AddScene("Result", new ResultScene);
	scene->SetImage(IMAGE->FindImage("gameover"));
	scene->SetEnemyManager(_em);	// 결과 씬에 enemyManger 포인터로 연결
	scene->SetMainGame(this);		// 결과 씬에 mainGame 포인터로 연결

	_em->SetResultScene(scene);		// enemyManger에 결과씬 포인터로 연결

	offsetY = 0;

	isStart = false;
	isLoad = false;

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


	// 엔터키 누르면 로딩바 시작
	if (INPUT->GetKeyDown(VK_RETURN) && !isStart) {
		//isStart = true;
		//SCENE->ChangeScene("Rocket");
		isLoad = true;
	}

	if (isLoad) {
		loadCount++;

		_loading->SetGauge(loadCount, LOADMAX);
		_loading->Update();

		// 로딩바 꽉차면 로켓 씬으로 씬 변경
		if (loadCount == LOADMAX) {
			isLoad = false;
			isStart = true;
			SCENE->ChangeScene("Rocket");
		}
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

		if (isLoad) {
			_loading->Render();
		}

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

void MainGame::ReStart()
{
	offsetY = 0;
	_em->Init();

	isStart = false;
	isLoad = true;

	loadCount = 0;

	IMAGE->FindImage("rocket")->SetX(WINSIZEX / 2);
	IMAGE->FindImage("rocket")->SetY(WINSIZEY - 200);

}
