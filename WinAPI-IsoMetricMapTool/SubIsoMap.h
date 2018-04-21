#pragma once

#include "GameNode.h"
#include "IsoTile.h"

class SubIsoMap : public GameNode
{
private:
	bool isDebug;

	int currentTile;

	RECT rc[TILE_SIZE_X][TILE_SIZE_Y];

	int frameX, frameY;
public:
	SubIsoMap();
	~SubIsoMap();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);
};

