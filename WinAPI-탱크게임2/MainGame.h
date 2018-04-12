#pragma once

#include "GameNode.h"

// 선택창을 만드는데 필요한 헤더
#include <commdlg.h>
#include "TileMap.h"

//// 메인 화면 640 * 640
//#define TILESIZE 32
//#define TILEX 20
//#define TILEY 20
//
//#define TILESIZEX TILEX * TILESIZE
//#define TILESIZEY TILEY * TILESIZE
//
//// 지형 속성
//// 밟을 때 사운드 나게 하거나, 
//// 이동할 수 없는 지역, 
//// 타일에 따른 속도 변경 설정 할 때 사용
//
//enum TERRAIN {
//	TR_CEMENT,
//	TR_GROUND,
//	TR_GRASS,
//	TR_WATER,
//	TR_END
//};
//
//// 지형 위쪽에 배치
//// 움직이거나 부서지거나 하는 것들 (몬스터, 캐릭터 이런 변화를 줄 수 있는 물체)
//enum OBJECT {
//	OBJ_BLOCK1,
//	OBJ_BLOCK2,
//	OBJ_BLOCK3,
//	OBJ_TANK1,
//	OBJ_TANK2,
//	OBJ_FLAG1,		// 깃발 승리 조건
//	OBJ_FLAG2,
//	OBJ_NONE
//};
//
//// 위치 좌표
//enum POS {
//	POS_FLAG1,
//	POS_FLAG2,
//	POS_TANK1,
//	POS_TANK2
//};
//
//// 타일 구조체
//struct tagTile {
//	TERRAIN terrain;
//	OBJECT obj;
//	RECT rc;
//	int terrainFrameX;
//	int terrainFrameY;
//	int objFrameX;
//	int objFrameY;
//};

class MainGame : public GameNode
{
private:
	bool isDebug;
	bool isStart;

	POINT imageFrame;
	tagTile _tiles[TILEX*TILEY];

	int _pos[2];

	CTRL _currentCTRL;

	char str[128];
public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	void MapToolSetup();
	void SetMap();

	TERRAIN TerrainSelect(int frameX, int frameY);
	OBJECT ObjSelect(int frameX, int frameY);

	void Save();
	void Load();
};

/*
	과제
	탱크 게임 완성하기
	맵 오브젝트 파괴까지 진행함
	에너미 만들어서 전투 진행, 에너미 인공지능은 맘대로 진행

	서버 클라이언트 만들어 놓은게 잇음 올려주실꺼니
	-> 콘솔 채팅으로 만들어둠
	한번 읽어오기
*/