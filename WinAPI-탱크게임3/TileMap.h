#pragma once

// 메인 화면 640 * 640
#define TILESIZE 32
#define TILEX 20
#define TILEY 20

#define TILESIZEX TILEX * TILESIZE
#define TILESIZEY TILEY * TILESIZE

// 16진수 0 8개인데 0 2개 당 1byte로 보면됨
// 비트연산 하려고 쓴거라고 하심
// 속성 추가하려면 *2 하는식로 하면 됨 다음값은 4
// 3d 가게되면 왜 이걸로 해야되는지 알 수 있음
#define ATTR_UNMOVE 0x00000001	// 움직일 수 없는거
#define ATTR_POSTION 0x00000002 // 

// 지형 속성
// 밟을 때 사운드 나게 하거나, 
// 이동할 수 없는 지역, 
// 타일에 따른 속도 변경 설정 할 때 사용

enum TERRAIN {
	TR_CEMENT,
	TR_GROUND,
	TR_GRASS,
	TR_WATER,
	TR_END
};

// 지형 위쪽에 배치
// 움직이거나 부서지거나 하는 것들 (몬스터, 캐릭터 이런 변화를 줄 수 있는 물체)
enum OBJECT {
	OBJ_BLOCK1,
	OBJ_BLOCK2,
	OBJ_BLOCK3,
	OBJ_TANK1,
	OBJ_TANK2,
	OBJ_FLAG1,		// 깃발 승리 조건
	OBJ_FLAG2,
	OBJ_NONE
};

// 위치 좌표
enum POS {
	POS_FLAG1,
	POS_FLAG2,
	POS_TANK1,
	POS_TANK2
};

// 타일 구조체
struct tagTile {
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};