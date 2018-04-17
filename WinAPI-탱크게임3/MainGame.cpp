#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "TankGameScene.h"

MainGame::MainGame()
{
	// 이거 때문에 한동안 MainGame의 Render가 안된거 같음
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

	SUBWIN->SetMainGame(this);

	TestScene * test = new TestScene;
	SCENE->AddScene("Test", test);

	SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	SOUND->AddSound("Fire", "sounds/fire.wav", false, false);

	SUBWIN->SetScene(test);

//	SCENE->ChangeScene("Test");

	IMAGE->AddImage("tileMap", "images/tilemap.bmp", 0, 0,
			256, 640, 8, 20, false, RGB(0, 0, 0));

	IMAGE->AddImage("camera", "images/카메라.bmp", 0, 0,
		32, 32, true, RGB(255, 0, 255));

	IMAGE->AddImage("gameover", "images/GameOver.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	IMAGE->AddImage("flag", "images/flag.bmp", 0, 0,
		32, 32, true, RGB(255, 0, 255));

	imageFrame = { 7, 2 };

	MapToolSetup();

	_currentCTRL = CTRL_TERRAINDRAW;

	SCENE->AddScene("TankGame", new TankGameScene);
	//SCENE->ChangeScene("TankGame");

	isStart = false;

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

	//// 이거 문제 있음 해결방안 찾아보자
	//// 마우스 움직이는 윈도우 위치에 따라 active 값 바꾸게했음
	if (SUBWIN->GetIsActive() == false && !isStart) {
		if (INPUT->GetKey(VK_LBUTTON)) {
			SetMap();
		}
	}

	if (INPUT->GetKeyDown(VK_RETURN)) { 
		isStart = true; 
		SCENE->ChangeScene("TankGame");
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
		SCENE->Render();
		SUBWIN->Render();

		if (!isStart) {
			for (int i = 0; i < TILEX * TILEY; i++) {
				// 지형
				if (_tiles[i].terrain == TR_CEMENT ||
					_tiles[i].terrain == TR_GROUND ||
					_tiles[i].terrain == TR_GRASS ||
					_tiles[i].terrain == TR_WATER) {
					IMAGE->FrameRender("tileMap", GetMemDC(),
						_tiles[i].rc.left, _tiles[i].rc.top,
						_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
				}
				// 오브젝트
				if (_tiles[i].obj == OBJ_NONE) continue;

				if (_tiles[i].obj == OBJ_BLOCK1 ||
					_tiles[i].obj == OBJ_BLOCK2 ||
					_tiles[i].obj == OBJ_BLOCK3) {
					IMAGE->FrameRender("tileMap", GetMemDC(),
						_tiles[i].rc.left, _tiles[i].rc.top,
						_tiles[i].objFrameX, _tiles[i].objFrameY);
				}
			}
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		char str[128];
		sprintf_s(str, "%d %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), 0, 80, str, strlen(str));

		sprintf_s(str, "%d %d", SUBWIN->GetMousePos().x, SUBWIN->GetMousePos().y);
		TextOut(GetMemDC(), 0, 100, str, strlen(str));
	}
	//=================================================
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}

void MainGame::MapToolSetup()
{
	for (int i = 0; i < TILEY; i++) {
		for (int j = 0; j < TILEX; j++) {
			SetRect(&_tiles[i * TILEX + j].rc,
				j * TILESIZE,				// left
				i * TILESIZE,				// top
				j * TILESIZE + TILESIZE,	// right
				i * TILESIZE + TILESIZE);	// bottom
				
		}
	}

	// 처음 깔려 있는 타일
	for (int i = 0; i < TILEX * TILEY; i++) {
		_tiles[i].terrainFrameX = 7;
		_tiles[i].terrainFrameY = 2;

		_tiles[i].objFrameX = 0;
		_tiles[i].objFrameY = 0;

		_tiles[i].terrain = 
			TerrainSelect(_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);

		_tiles[i].obj = OBJ_NONE;
	}
}

void MainGame::SetMap()
{
	imageFrame = SUBWIN->GetFramePoint();
	
	// static이라 하나만 있어서 클래스로 가져 올 수 있음
	// 싱글턴 안쓰고도
	_currentCTRL = Window::GetCTRL();

	for (int i = 0; i < TILEX*TILEY; i++) {
		if (PtInRect(&_tiles[i].rc, g_ptMouse)) {
			if (_currentCTRL == CTRL_TERRAINDRAW) {
				_tiles[i].terrainFrameX = imageFrame.x;
				_tiles[i].terrainFrameY = imageFrame.y;

				_tiles[i].terrain = TerrainSelect(imageFrame.x, imageFrame.y);
				break;
			}
			if (_currentCTRL == CTRL_OBJECTDRAW) {
				_tiles[i].objFrameX = imageFrame.x;
				_tiles[i].objFrameY = imageFrame.y;

				_tiles[i].obj = ObjSelect(imageFrame.x, imageFrame.y);

				//// test
				//_tiles[i].objFrameX = imageFrame.y;
				//_tiles[i].objFrameY = imageFrame.x;

				_tiles[i].obj = ObjSelect(imageFrame.y, imageFrame.x);
				break;
			}

			if (_currentCTRL == CTRL_ERASER) {
				_tiles[i].terrainFrameX = 7;
				_tiles[i].terrainFrameY = 2;

				_tiles[i].terrain = TerrainSelect(7, 2);

				// object도 지워야한다면 같이 지워주면 됨 자유롭게하기
				_tiles[i].obj = OBJ_NONE;
				break;
			}
		}
	}
}

TERRAIN MainGame::TerrainSelect(int frameX, int frameY)
{
	// 시멘트 타일
	if (frameX == 7 && frameY == 1) return TR_CEMENT;
	// 흙 타일
	if (frameX == 7 && frameY == 2) return TR_GROUND;
	// 풀 타일
	if (frameX == 7 && frameY == 3) return TR_GRASS;
	// 물 타일
	if (frameX == 7 && frameY == 4) return TR_WATER;

	return TR_GROUND;
}

OBJECT MainGame::ObjSelect(int frameX, int frameY)
{
	// 한방 타일
	if (frameX == 17 && frameY == 4) return OBJ_BLOCK1;
	// 두방 타일
	if (frameX == 14 && frameY == 4) return OBJ_BLOCK2;
	// 세방 타일
	if (frameX == 17 && frameY == 5) return OBJ_BLOCK3;

	return OBJ_NONE;
}

void MainGame::Save()
{
	if (isStart) return;

	// Ini로 저장하는 방법
	//for (int i = 0; i < TILEX*TILEY; i++) {
	//	sprintf_s(str, "tile %d", i);
	//	INIDATA->AddData(str, "obj", to_string(_tiles[i].obj).c_str());
	//	INIDATA->AddData(str, "objFrameX", to_string(_tiles[i].objFrameX).c_str());
	//	INIDATA->AddData(str, "objFrameY", to_string(_tiles[i].objFrameY).c_str());
	//	INIDATA->AddData(str, "terrain", to_string(_tiles[i].terrain).c_str());
	//	INIDATA->AddData(str, "terrainFrameX", to_string(_tiles[i].terrainFrameX).c_str());
	//	INIDATA->AddData(str, "terrainFrameY", to_string(_tiles[i].terrainFrameY).c_str());
	//}
	//INIDATA->SaveData("TilesInfo");

	// api가 가지고있는 save 함수 사용
	// 구조체로 넣을 수도 있고
	// 마찬가지로 배열로도 넣을 수 있음
	// 바이너리 파일이라 이미지 같은 웬만한거 다됨
	HANDLE file;
	DWORD write;

	// CreateFile Binary 형식으로 들어감
	// Binary 16진수 데이터 형식으로 들어감
	// Binary 한글 관련으로 문제가 있었음
	// binary라 일반적인 fopen 보다 빠름 대신 한줄로 쭉 써짐
	file = CreateFile(
		"save/tileMap.map",	// 생성할 파일 또는 로드할 파일의 이름
		GENERIC_WRITE,	// 파일이나 장치를 만들거나 열때의 권한
		0,				// 파일 공유 모드 입력
		NULL,			// 파일 또는 장치를 열 때 보안 및 특성
						// 항상 처음부터 작성하겠다는거 다 지우고
						// 여기 바꿔주면 라인별로 넣을 수 있음
		CREATE_ALWAYS,	// 파일이나 장치를 열 때 취할 행동
		FILE_ATTRIBUTE_NORMAL,	// 파일, 장치를 열 때, 만들 때 갖는 특성
		NULL			// 만들어질 파일이 갖게 될 특성 etc 저장되는 핸들
	);

	// 구분점은 따로 없고 쭉 들어가므로 사이즈로 구분해서 읽어와야함
	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	WriteFile(file, _pos, sizeof(int) * 2, &write, NULL);

	CloseHandle(file);
}

void MainGame::Load()
{
	if (isStart) return;

	// Ini 읽는방법
	//for (int i = 0; i < TILEX*TILEY; i++) {
	//	sprintf_s(str, "tile %d", i);
	//	_tiles[i].obj = (OBJECT)INIDATA->LoadDataInteger("TilesInfo", str, "obj");
	//	_tiles[i].objFrameX = INIDATA->LoadDataInteger("TilesInfo", str, "objFrameX");
	//	_tiles[i].objFrameY = INIDATA->LoadDataInteger("TilesInfo", str, "objFrameY");
	//	_tiles[i].terrain = (TERRAIN)INIDATA->LoadDataInteger("TilesInfo", str, "terrain");
	//	_tiles[i].terrainFrameX = INIDATA->LoadDataInteger("TilesInfo", str, "terrainFrameX");
	//	_tiles[i].terrainFrameY = INIDATA->LoadDataInteger("TilesInfo", str, "terrainFrameY");
	//}

	// api가 가지고있는 save 함수 사용
	// 구조체로 넣을 수도 있고
	// 마찬가지로 배열로도 넣을 수 있음
	HANDLE file;
	DWORD read;

	// CreateFile Binary 형식으로 들어감
	// Binary 16진수 데이터 형식으로 들어감
	// Binary 한글 관련으로 문제가 있었음
	// binary라 일반적인 fopen 보다 빠름 대신 한줄로 쭉 써짐
	file = CreateFile(
		"save/tileMap.map",	// 생성할 파일 또는 로드할 파일의 이름
		GENERIC_READ, /// 수정	// 파일이나 장치를 만들거나 열때의 권한
		0,				// 파일 공유 모드 입력
		NULL,			// 파일 또는 장치를 열 때 보안 및 특성
						// 항상 처음부터 작성하겠다는거 다 지우고
						// 여기 바꿔주면 라인별로 넣을 수 있음
		OPEN_EXISTING, /// 수정	// 파일이나 장치를 열 때 취할 행동
		FILE_ATTRIBUTE_NORMAL,	// 파일, 장치를 열 때, 만들 때 갖는 특성
		NULL			// 만들어질 파일이 갖게 될 특성 etc 저장되는 핸들
	);

	// 구분점은 따로 없고 쭉 들어가므로 사이즈로 구분해서 읽어와야함
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	ReadFile(file, _pos, sizeof(int) * 2, &read, NULL);

	CloseHandle(file);
}
