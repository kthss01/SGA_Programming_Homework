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
	img = IMAGE->FindImage("tile_sub");

	for (int j = 0; j < MAXTILEX; j++) {
		for (int i = 0; i < MAXTILEY; i++) {
			tile[i][j].x = 20 * j;
			tile[i][j].y = 20 * i + 50;
			tile[i][j].tileX = j;
			tile[i][j].tileY = i;
			tile[i][j].rc = RectMake(tile[i][j].x, tile[i][j].y,
				img->GetFrameWidth(), img->GetFrameHeight());
			tile[i][j].check = true;
		}
	}

	isDebug = false;
	isStart = false;
	startX = startY = endX = endY = 0;

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
				img->FrameRender(hdc, tile[i][j].x, tile[i][j].y,
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
	}
	//=================================================
}
