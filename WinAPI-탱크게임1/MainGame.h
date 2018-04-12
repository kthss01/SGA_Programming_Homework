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
	맵 만들어오기
	파란색 타일이 꼭 들어가야함 마지막 타일에서 파란색 타일
*/

/*
	과제 -> 맵에 오브젝트 배치
	스페이스 바를 누르면 발포음 추가
	현재 진행 방향 일직선상에 오브젝트 데미지 주기
	1,2,3 오브젝트 (1방,2방,3방 오브젝트) 배치 맘대로 하고 데미지 깍기게
	3방짜리가 2방짜리로 바뀌고 1방짜리 맞으면 삭제
	일직선 에 여러개있으면 젤 앞에꺼만
	오브젝트는 알아보기 쉽게 맨밑 회색이 3방 초록색 2방 파란색 1방으로
	이미지 맘대로 해서
*/