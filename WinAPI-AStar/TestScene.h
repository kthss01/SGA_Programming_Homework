#pragma once

#include "GameNode.h"

#define TILEX 25
#define TILEY 25
#define TILESIZE TILEX * TILEY

enum Select {
	SELECT_START,
	SELECT_END,
	SELECT_BLOCK,
};

enum Direction {
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFTUP,
	DIRECTION_RIGHTDOWN,
	DIRECTION_LEFTDOWN,
	DIRECTION_RIGHTUP
};

enum State {
	STATE_NONE,
	STATE_OPEN,
	STATE_CLOSE,
	STATE_PATH
};

struct tagTile {
	RECT rc;
	bool block;

	int node;

	int showState;

	int f, g, h;
};

class TestScene : public GameNode
{
private:
	bool isDebug;

	tagTile tiles[TILESIZE];

	vector<int> openList;
	vector<int> closeList;
	vector<int>::iterator iter;

	Select currentSelect;

	int startTile;
	int endTile;
	int currentTile;
	bool isFind;
	bool noPath;
	bool startAStar;

	RECT rc[6];

	HBRUSH brush;
	HFONT font;
	HFONT oldFont;
	char str[128];
	int temp;
public:
	TestScene();
	~TestScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void AStar();
};

