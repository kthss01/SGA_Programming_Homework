#include "stdafx.h"
#include "PokemonMap.h"


PokemonMap::PokemonMap()
{
}


PokemonMap::~PokemonMap()
{
}

HRESULT PokemonMap::Init()
{
	_startX = INIT_X;
	_startY = INIT_Y;

	_isDebug = false;

	Load();

	return S_OK;
}

void PokemonMap::Release()
{
}

void PokemonMap::Update()
{
	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}
}

void PokemonMap::Render()
{
	DrawTile();
}

void PokemonMap::DrawRhombus(int left, int top)
{
	int centerX = left + RADIUS_WIDTH;
	int centerY = top + RADIUS_HEIGHT;

	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - RADIUS_HEIGHT;

	p[1].x = centerX + RADIUS_WIDTH;
	p[1].y = centerY;

	p[2].x = centerX;
	p[2].y = centerY + RADIUS_HEIGHT;

	p[3].x = centerX - RADIUS_WIDTH;
	p[3].y = centerY;

	p[4].x = centerX;
	p[4].y = centerY - RADIUS_HEIGHT;

	for (int i = 1; i < 5; i++) {
		LineMake(GetMemDC(), p[i - 1], p[i]);
	}
}

void PokemonMap::DrawTile()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			int left = _startX + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = _startY + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			_tileMap[i][j].left = left;
			_tileMap[i][j].top = top;

			if (left + CELL_WIDTH <0 || left > WINSIZEX
				|| top + CELL_HEIGHT < 0 || top > WINSIZEY) continue;

			//IMAGE->FrameRender("tile", GetMemDC(), left, top,
			//	0, 0);

			for (int z = 0; z <= _tileMap[i][j].index; z++) {
				if (_tileMap[i][j].tileKind[z] != TILEKIND_NONE) {
					switch (_tileMap[i][j].tileNum[z])
					{
					case 0:
						IMAGE->FrameRender("tile", GetMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height * z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					case 1:
					case 2:
						IMAGE->FrameRender("tile2", GetMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height * z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					}
				}
			}

			if (_isDebug) {
				DrawRhombus(left, top);
				SetTextColor(GetMemDC(), RGB(0, 0, 0));
				sprintf_s(str, "(%d,%d)", i, j);
				TextOut(GetMemDC(),
					left + RADIUS_WIDTH / 2 + 8,
					top + RADIUS_HEIGHT / 2 + 5, str, strlen(str));
			}
		}
	}
}

void PokemonMap::Load()
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
	ReadFile(file, _tileMap,
		sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &read, NULL);

	CloseHandle(file);
}
