#pragma once

#include "GameNode.h"

#define BLOCKMAX 4 * 12
#define ITEMMAX 10
#define BALLMAX 3

struct tagBlockInfo {
	RECT rc;
	float x, y;
	int life;
	bool isItem;
};

struct tagBallInfo {
	POINT pt;
	float radius;
	float angle;
	float speed;
	bool isFire;
};

struct tagBarInfo {
	RECT rc;
	float x, y;
	float speed;
	float width;
	float height;
};

enum ItemKind {
	ITEM_NULL,
	ITEM_MULT,
	ITEM_FAST,
	ITEM_SLOW,
	ITEM_LONG,
	ITEM_SHORT,
	ITEM_CATCH,
	ITEM_BULLET
};

struct tagItemInfo {
	RECT rc;
	ItemKind kind;
	float x, y;
	bool isFind;
};

class MainGame13 : public GameNode
{
private:
	tagBlockInfo block[BLOCKMAX];
	tagBallInfo ball[BALLMAX];
	tagBarInfo bar;
	tagItemInfo item[ITEMMAX];
	bool isOver;
	int ballCount;
	float initSpeed;
public:
	MainGame13();
	~MainGame13();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	bool RectInCircle(RECT* rect, POINT circleCenter, float radius);
};

