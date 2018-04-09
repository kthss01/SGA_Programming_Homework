#pragma once

#include "MapTool.h"

#include <stack>

using namespace std;

struct tagTileData {
	tagTileInfo tile[MAXWINTILEY * 2][MAXWINTILEX * 2];
};

class ToolMain : public MapTool
{
private:
	Image * img;

	char str[128];

	//tagTileInfo tile[MAXWINTILEY * 2][MAXWINTILEX * 2];
	tagTileData tile;
	stack<tagTileData> stack;

	//tagTileInfo currentTile;
	vector<tagTileInfo> vCurrentTile;
	int countX, countY;
	int currentX, currentY;

	int startX, startY;
	int endX, endY;

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
	void RestoreTile();

	// 편법
	//tagTileInfo* GetTile() { return (tagTileInfo*)tile; }
	// 2차원 배열 반환
	tagTileInfo (* GetTile())[MAXWINTILEX * 2] { return tile.tile; }

	int GetStartX() { return startX; }
	int GetStartY() { return startY; }
	int GetEndX() { return endX; }
	int GetEndY() { return endY; }
};

