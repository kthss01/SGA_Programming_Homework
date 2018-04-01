#pragma once

#include "GameNode.h"

#include <queue>
#include <stack>

using namespace std;

#define TILEROW 12
#define TILECOL 12
#define TILEMAX TILEROW * TILECOL

struct tagCost {
	int h;
	int g;
};

struct tagTile {
	RECT rc;
	float x, y;
	float width, height;
	int row, col;
	bool block;

	bool dest;
	bool close;
	tagCost f;
	tagTile* node;
};

struct tagPlayer {
	RECT rc;
	float x, y;
	float width, height;
	float angle;
	float speed;

	bool isMove;
	float destX, destY;
	int index;
};

class TestScene : public GameNode
{
private:
	bool isDebug;

	tagTile m_tile[TILEMAX];
	tagPlayer m_player;

	stack<tagTile> pathStack;
	stack<tagTile> showStack;
	stack<tagTile> lineStack;

	HBRUSH brush;
	char str[128];
public:
	TestScene();
	~TestScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void Move();
	void AStarPathFinding(tagTile& tile);
};

