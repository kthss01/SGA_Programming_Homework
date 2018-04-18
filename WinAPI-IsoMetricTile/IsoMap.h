#pragma once

#include "GameNode.h"
#include "IsoTile.h"

class IsoMap : public GameNode
{
private:
	tagTile _tileMap[TILE_COUNT_X][TILE_COUNT_Y];
	int _isoX, _isoY;
	int _center;
	char str[128];

	int _startX;
	int _startY;

	bool _isDebug;
public:
	IsoMap();
	~IsoMap();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void DrawTileMap();
	void DrawRhombus(int left, int top);
	int GetCornerIndex(int isoX, int isoY);
	bool IsInRhombus(int corner, int isoX, int isoY);
};

