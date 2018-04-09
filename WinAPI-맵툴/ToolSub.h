#pragma once

#include "MapTool.h"

class ToolMain;

class ToolSub : public MapTool
{
private:
	bool isDebug;

	char str[128];

	Image * tileImg;
	tagTileInfo tile[MAXTILEY][MAXTILEX];

	int startX, startY;
	int endX, endY;
	bool isStart;

	Image * toolbar;
	RECT init, save, load, restore;

	tagTileInfo miniMap[MAXWINTILEY * 2][MAXWINTILEX * 2];
	Image * miniTile;

	ToolMain * toolMain;
public:
	ToolSub();
	~ToolSub();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetToolMain(ToolMain* toolMain) { this->toolMain = toolMain; }

	void SetMiniMap();
};

