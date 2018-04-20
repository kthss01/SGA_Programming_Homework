#include "stdafx.h"
#include "SubIsoMap.h"


SubIsoMap::SubIsoMap()
{
}


SubIsoMap::~SubIsoMap()
{
}

HRESULT SubIsoMap::Init()
{
	for (int i = 0; i < TILE_SIZE_X; i++) {
		for (int j = 0; j < TILE_SIZE_Y; j++) {
			rc[i][j] = RectMake(
				SUBWINSIZEX / TILE_SIZE_X * i,
				245 / TILE_SIZE_Y * j + 100,
				SUBWINSIZEX / TILE_SIZE_X, 245 / TILE_SIZE_Y);
		}
	}

	isDebug = false;

	return S_OK;
}

void SubIsoMap::Release()
{
}

void SubIsoMap::Update()
{
	if (SUBWIN->GetIsActive() &&
		INPUT->GetKeyDown(VK_LBUTTON)) {
		for (int i = 0; i < TILE_SIZE_X; i++) {
			for (int j = 0; j < TILE_SIZE_Y; j++) {
				if (PtInRect(&rc[i][j], SUBWIN->GetMousePos())) {
					SUBWIN->SetFramePoint(PointMake(i, j));
				}
			}
		}
	}

	if (INPUT->GetKeyDown(VK_SHIFT))
		isDebug = !isDebug;
}

void SubIsoMap::Render(HDC hdc)
{
	IMAGE->FindImage("tile")->Render(hdc, 0, 100, SUBWINSIZEX, 245);

	if (isDebug) {
		for (int i = 0; i < TILE_SIZE_X; i++) {
			for (int j = 0; j < TILE_SIZE_Y; j++) {
				RectangleMake(hdc, rc[i][j]);
			}
		}
	}
}
