#include "stdafx.h"
#include "ToolMain.h"


ToolMain::ToolMain()
{
}


ToolMain::~ToolMain()
{
}

HRESULT ToolMain::Init()
{
	isDebug = false;

	img = IMAGE->FindImage("tile_main");

	for (int j = 0; j < MAXWINTILEX; j++) {
		for (int i = 0; i < MAXWINTILEY; i++) {
			ZeroMemory(&tile[i][j], sizeof(tagTileInfo));
			tile[i][j].x = j * img->GetFrameWidth();
			tile[i][j].y = i * img->GetFrameHeight();
			tile[i][j].rc = RectMake(tile[i][j].x, tile[i][j].y,
				img->GetFrameWidth(), img->GetFrameHeight());
		}
	}

	countX = countY = 0;
	currentX = currentY = 0;

	return S_OK;
}

void ToolMain::Release()
{
}

void ToolMain::Update()
{
	for (int j = 0; j < MAXWINTILEX; j++) {
		for (int i = 0; i < MAXWINTILEY; i++) {
			if (PtInRect(&tile[i][j].rc, g_ptMouse)) {
				currentX = j;
				currentY = i;
			}
		}
	}

	if (INPUT->GetKey(VK_LBUTTON)
		&& SUBWIN->GetIsActive() == false) {
		for (int j = 0; j < MAXWINTILEX; j++) {
			for (int i = 0; i < MAXWINTILEY; i++) {
				if (PtInRect(&tile[i][j].rc, g_ptMouse)) {
					//int x = tile[i][j].x;
					//int y = tile[i][j].y;
					//tile[i][j] = currentTile;
					//tile[i][j].x = x;
					//tile[i][j].y = y;
					int cnt = 0;
					if (vCurrentTile.size() != 0) {
						for (int l = j; l <= j + countX; l++) {
							if (l >= MAXWINTILEX) break;
							for (int k = i; k <= i + countY; k++) {
								if (k >= MAXWINTILEY) {
									cnt += currentY + countY - k + 1;
									break;
								}
								int x = tile[k][l].x;
								int y = tile[k][l].y;
								RECT rc = tile[k][l].rc;
								tile[k][l] = vCurrentTile[cnt++];
								tile[k][l].x = x;
								tile[k][l].y = y;
								tile[k][l].rc = rc;
							}
						}
					}
				}
			}
		}
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void ToolMain::Render()
{
	//=================================================
	{
		// test
		//img->FrameRender(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2,
		//	currentTile.tileX, currentTile.tileY);

		for (int j = 0; j < MAXWINTILEX; j++) {
			for (int i = 0; i < MAXWINTILEY; i++) {
				if (tile[i][j].check == false) continue;
				img->FrameRender(GetMemDC(),
					tile[i][j].x, tile[i][j].y,
					tile[i][j].tileX, tile[i][j].tileY);
			}
		}

		if (vCurrentTile.size() != 0) {
			int cnt = 0;
			for (int j = currentX; j <= currentX + countX; j++) {
				if (j >= MAXWINTILEX) break;
				for (int i = currentY; i <= currentY + countY; i++) {
					if (i >= MAXWINTILEY) {
						cnt += currentY + countY - i + 1;
						break;
					}
					img->FrameRender(GetMemDC(),
						tile[i][j].x, tile[i][j].y,
						vCurrentTile[cnt].tileX, vCurrentTile[cnt].tileY, 128);
					cnt++;
				}
			}
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "(%d %d)", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), WINSIZEX - 100, 0, str, strlen(str));

		for (int j = 0; j < MAXWINTILEX; j++) {
			for (int i = 0; i < MAXWINTILEY; i++) {
				RectangleMake(GetMemDC(), tile[i][j].rc);
			}
		}
	}
	//=================================================
}
