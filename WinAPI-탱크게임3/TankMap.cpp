#include "stdafx.h"
#include "TankMap.h"


TankMap::TankMap()
{
}


TankMap::~TankMap()
{
}

HRESULT TankMap::Init()
{
	Load();

	return S_OK;
}

void TankMap::Release()
{
}

void TankMap::Update()
{
}

void TankMap::Render()
{
	for (int i = 0; i < TILEX * TILEY; i++) {
		// 지형
		IMAGE->FrameRender("tileMap", GetMemDC(),
			_tiles[i].rc.left, _tiles[i].rc.top,
			_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);

		// 오브젝트
		if (_tiles[i].obj == OBJ_NONE) continue;
		IMAGE->FrameRender("tileMap", GetMemDC(),
			_tiles[i].rc.left, _tiles[i].rc.top,
			_tiles[i].objFrameX, _tiles[i].objFrameY);
	}
}

void TankMap::Load()
{
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

	ZeroMemory(_attributes, sizeof(DWORD)*TILEX*TILEY);
	for (int i = 0; i < TILEX*TILEY; i++) {
		if (_tiles[i].terrain == TR_WATER) _attributes[i] = ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_BLOCK1) _attributes[i] = ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_BLOCK2) _attributes[i] = ATTR_UNMOVE;
		if (_tiles[i].obj == OBJ_BLOCK3) _attributes[i] = ATTR_UNMOVE;
	}
}

POINT TankMap::GetOBJECTFrame(OBJECT obj)
{
	POINT pos = {};

	switch (obj)
	{
	case OBJ_BLOCK1:
		pos = { 4,17 };
		break;
	case OBJ_BLOCK2:
		pos = { 4,14 };
		break;
	case OBJ_BLOCK3:
		pos = { 5,17 };
		break;
	}

	return pos;
}
