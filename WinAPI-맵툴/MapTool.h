#pragma once

#include "GameNode.h"

// tile 21 x 16
#define MAXTILEX 21
#define MAXTILEY 16

#define MAXWINTILEX WINSIZEX / 25	// 40
#define MAXWINTILEY WINSIZEY / 25	// 30

struct tagTileInfo {
	RECT rc;
	int x, y;				// 윈도우에서 타일의 위치 x, y
	int tileX, tileY;		// 이미지에서 읽어올 타일 x, y
	bool check;				// 그려졌는지 체크
};

class MapTool : public GameNode
{
public:
	MapTool();
	~MapTool();
};

