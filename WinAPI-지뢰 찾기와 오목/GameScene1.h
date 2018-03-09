#pragma once

#include "GameNode.h"

enum MineStatus {
	STATUS_ONE,
	STATUS_TWO,
	STATUS_THREE,
	STATUS_FOUR,
	STATUS_BOOM,
	STATUS_EMPTY,
	STATUS_FLAG,
	STATUS_MINE,
	STATUS_NCB
};

struct tagMineInfo {
	RECT rc;
	float x, y;
	int nearMineCount;
	bool isMine;
	bool isFlag;
	MineStatus status;
};

/*
	마인 위치할 필드 크기
	x : 25 ~ 486   = 대략 450
	y : 256 ~ 722  = 대략 470
*/

#define ROW 10
#define COL 10
#define MINECOUNT 50

#define FIELDX float(486 - 25)
#define FIELDY float(725 - 256)

class GameScene1 : public GameNode
{
private:
	Image * board;
	Image * number[6];

	Image * mine[9];
	Image * result;

	float sizeX, sizeY;
	int flagCount;

	tagMineInfo mineInfo[ROW][COL];

	bool isMine[ROW * COL];
	char str[128];
	bool isClear;
	bool isDebug;
public:
	GameScene1();
	~GameScene1();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void ChangeNumber(bool mine, int number);
	void MineShuffle();
	void MineCheck();
	void GameOver();
	void GameClear();
	void FindMine(int row, int col);
};

