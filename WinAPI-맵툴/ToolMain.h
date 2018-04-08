#pragma once

#include "MapTool.h"

#define MAXWINTILEX WINSIZEX / 25
#define MAXWINTILEY WINSIZEY / 25

class ToolMain : public MapTool
{
private:
	Image * img;

	char str[128];

	tagTileInfo tile[MAXWINTILEY][MAXWINTILEX];

	//tagTileInfo currentTile;
	vector<tagTileInfo> vCurrentTile;
	int countX, countY;
	int currentX, currentY;

	bool isDebug;
public:
	ToolMain();
	~ToolMain();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	//void SetCurrentTile(tagTileInfo tile) { currentTile = tile; }
	void SetTileXY(int endX, int endY) { this->countX = endX; this->countY = endY; }
	void AddCurrentTile(tagTileInfo tile) { vCurrentTile.push_back(tile); }
	void ClearCurrentTile() { vCurrentTile.clear(); }

	void InitTile();
	void SaveTile();
	void LoadTile();
};

