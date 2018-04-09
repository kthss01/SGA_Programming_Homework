#include "stdafx.h"
#include "ToolSub.h"

#include "ToolMain.h"

ToolSub::ToolSub()
{
}


ToolSub::~ToolSub()
{
}

HRESULT ToolSub::Init()
{
	tileImg = IMAGE->FindImage("tile_sub");

	for (int j = 0; j < MAXTILEX; j++) {
		for (int i = 0; i < MAXTILEY; i++) {
			tile[i][j].x = 20 * j;
			tile[i][j].y = 20 * i + 50;
			tile[i][j].tileX = j;
			tile[i][j].tileY = i;
			tile[i][j].rc = RectMake(tile[i][j].x, tile[i][j].y,
				tileImg->GetFrameWidth(), tileImg->GetFrameHeight());
			tile[i][j].check = true;
		}
	}

	isDebug = false;
	isStart = false;
	startX = startY = endX = endY = 0;

	toolbar = IMAGE->FindImage("toolbar");

	init = RectMake(0, 0, 50, 50);
	save = RectMake(60, 0, 50, 50);
	load = RectMake(120, 0, 50, 50);
	restore = RectMake(180, 0, 50, 50);
	
	int temp = 5;

	miniTile = IMAGE->FindImage("tile_mini");

	for (int j = 0; j < MAXWINTILEX * 2; j++) {
		for (int i = 0; i < MAXWINTILEY * 2; i++) {
			miniMap[i][j].x = j * temp + 10;
			miniMap[i][j].y = i * temp + 410;
			miniMap[i][j].rc = RectMake(miniMap[i][j].x, miniMap[i][j].y, temp, temp);
		}
	}

	return S_OK;
}

void ToolSub::Release()
{
}

void ToolSub::Update()
{	
	if (INPUT->GetKeyDown(VK_LBUTTON)
		&& SUBWIN->GetIsActive()) {
		for (int j = 0; j < MAXTILEX; j++) {
			for (int i = 0; i < MAXTILEY; i++) {
				if (PtInRect(&tile[i][j].rc, SUBWIN->GetPtMouse())) {
					//currentTile = tile[i][j];
					//toolMain->SetCurrentTile(tile[i][j]);
					startX = j;
					startY = i;
					isStart = true;
				}
			}
		}

		if (PtInRect(&init, SUBWIN->GetPtMouse())) {
			toolMain->InitTile();
		}
		if (PtInRect(&save, SUBWIN->GetPtMouse())) {
			toolMain->SaveTile();
		}
		if (PtInRect(&load, SUBWIN->GetPtMouse())) {
			toolMain->LoadTile();
		}
		if (PtInRect(&restore, SUBWIN->GetPtMouse())) {
			toolMain->RestoreTile();
		}
	}
	if (INPUT->GetKey(VK_LBUTTON)
		&& SUBWIN->GetIsActive()) {
		for (int j = 0; j < MAXTILEX; j++) {
			for (int i = 0; i < MAXTILEY; i++) {
				if (PtInRect(&tile[i][j].rc, SUBWIN->GetPtMouse())) {
					//currentTile = tile[i][j];
					//toolMain->SetCurrentTile(tile[i][j]);
					endX = j;
					endY = i;
				}
			}
		}
	}
	if (INPUT->GetKeyUp(VK_LBUTTON)
		&& SUBWIN->GetIsActive()) {
		for (int j = 0; j < MAXTILEX; j++) {
			for (int i = 0; i < MAXTILEY; i++) {
				if (PtInRect(&tile[i][j].rc, SUBWIN->GetPtMouse())) {
					//currentTile = tile[i][j];
					//toolMain->SetCurrentTile(tile[i][j]);
					endX = j;
					endY = i;
					toolMain->ClearCurrentTile();
					toolMain->SetTileXY(endX - startX, endY - startY);
					for (int j = startX; j <= endX; j++) {
						for (int i = startY; i <= endY; i++) {
							toolMain->AddCurrentTile(tile[i][j]);
						}
					}
				}
			}
		}
	}

	SetMiniMap();

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_SHIFT)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void ToolSub::Render(HDC hdc)
{
	// test
	IMAGE->Render("tile_sub", hdc, 0, 50);

	//=================================================
	{
		//RectangleMake(hdc,
		//	tile[startY][startX].rc.left, tile[startY][startX].rc.top,
		//	tile[endY][endX].rc.right, tile[endY][endX].rc.bottom);

		for (int j = 0; j < MAXTILEX; j++) {
			for (int i = 0; i < MAXTILEY; i++) {
				tileImg->FrameRender(hdc, tile[i][j].x, tile[i][j].y,
					tile[i][j].tileX, tile[i][j].tileY);
			}
		}

		if (isStart) {
			HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
			HPEN oldPen = (HPEN)SelectObject(hdc, pen);

			MoveToEx(hdc,
				tile[startY][startX].rc.left, tile[startY][startX].rc.top, NULL);
			LineTo(hdc, tile[endY][endX].rc.right, tile[startY][startX].rc.top);
			LineTo(hdc, tile[endY][endX].rc.right, tile[endY][endX].rc.bottom);
			LineTo(hdc, tile[startY][startX].rc.left, tile[endY][endX].rc.bottom);
			LineTo(hdc, tile[startY][startX].rc.left, tile[startY][startX].rc.top);

			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}

		toolbar->FrameRender(hdc, init.left, init.top, 0, 0);
		toolbar->FrameRender(hdc, save.left, save.top, 1, 0);
		toolbar->FrameRender(hdc, load.left, load.top, 2, 0);
		IMAGE->Render("restore", hdc, restore.left, restore.top);

		for (int j = 0; j < MAXWINTILEX * 2; j++) {
			for (int i = 0; i < MAXWINTILEY * 2; i++) {
				if (miniMap[i][j].check == false) continue;
				miniTile->FrameRender(hdc, miniMap[i][j].x, miniMap[i][j].y,
					miniMap[i][j].tileX, miniMap[i][j].tileY);
			}
		}

		LineMake(hdc, PointMake(0, 400), PointMake(SUBWINSIZEX, 400));
		IMAGE->Render("miniMap", hdc, 10, 365);
		TextOut(hdc, 55, 375, "MiniMap", strlen("MiniMap"));
		IMAGE->Render("camera", hdc, 
			miniMap[toolMain->GetStartY()][toolMain->GetStartX()].x,
			miniMap[toolMain->GetStartY()][toolMain->GetStartX()].y);
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "(%d %d)", SUBWIN->GetPtMouse().x, SUBWIN->GetPtMouse().y);
		TextOut(hdc, 0, 0, str, strlen(str));

		for (int j = 0; j < MAXTILEX; j++) {
			for (int i = 0; i < MAXTILEY; i++) {
				RectangleMake(hdc, tile[i][j].rc);
			}
		}

		RectangleMake(hdc, init);
		RectangleMake(hdc, save);
		RectangleMake(hdc, load);
		RectangleMake(hdc, restore);

		for (int j = 0; j < MAXWINTILEX * 2; j++) {
			for (int i = 0; i < MAXWINTILEY * 2; i++) {
				RectangleMake(hdc, miniMap[i][j].rc);
			}
		}
	}
	//=================================================
}

void ToolSub::SetMiniMap()
{
	tagTileInfo (*tile)[MAXWINTILEX * 2] = toolMain->GetTile();

	for (int j = 0; j < MAXWINTILEX * 2; j++) {
		for (int i = 0; i < MAXWINTILEY * 2; i++) {
			int x = miniMap[i][j].x;
			int y = miniMap[i][j].y;
			RECT rc = miniMap[i][j].rc;
			//miniMap[i][j] = tile[j * MAXWINTILEY * 2 + i];
			miniMap[i][j] = tile[i][j];
			miniMap[i][j].x = x;
			miniMap[i][j].y = y;
			miniMap[i][j].rc = rc;
		}
	}
}
