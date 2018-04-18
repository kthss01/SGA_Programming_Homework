#include "stdafx.h"
#include "IsoMap.h"


IsoMap::IsoMap()
{
}


IsoMap::~IsoMap()
{
}

HRESULT IsoMap::Init()
{
	_isoX = 0;
	_isoY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));

	_startX = INIT_X;
	_startY = INIT_Y;

	_isDebug = true;

	int test = RADIUS_HEIGHT;
	int test2 = CELL_HEIGHT;

	return S_OK;
}

void IsoMap::Release()
{
}

void IsoMap::Update()
{
	if (INPUT->GetKey('W')) { _startY += 5; }
	if (INPUT->GetKey('S')) { _startY -= 5; }
	if (INPUT->GetKey('A')) { _startX += 5; }
	if (INPUT->GetKey('D')) { _startX -= 5; }

	if (INPUT->GetKeyDown(VK_TAB)) {
		_isDebug = !_isDebug;
	}

	if (INPUT->GetKey(VK_LBUTTON)) {
		float cellX = (float)(g_ptMouse.x - _startX);

		if (cellX < 0) {
			cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
		}
		else {
			cellX = cellX / (float)CELL_WIDTH;
		}

		int cellY = abs(g_ptMouse.y - _startY) / CELL_HEIGHT;

		cellY = (g_ptMouse.y < _startY) ? cellY * -1 : cellY;

		int isoX = (int)cellX + (int)cellY;
		int isoY = (int)cellY - (int)cellX;

		if (isoX >= 0 && isoX < TILE_COUNT_X &&
			isoY >= 0 && isoY < TILE_COUNT_Y) {
			int corner = GetCornerIndex(isoX, isoY);

			if (IsInRhombus(corner, isoX, isoY))
				corner = 0;

			switch (corner)
			{
			case 1:
				isoX = isoX - 1;
				break;
			case 2:
				isoY = isoY - 1;
				break;
			case 3:
				isoY = isoY + 1;
				break;
			case 4:
				isoX = isoX + 1;
				break;
			}

			_tileMap[isoX][isoY].isSelected = true;

			_center = corner;
			_isoX = isoX;
			_isoY = isoY;
		}
	}
}

void IsoMap::Render()
{
	sprintf_s(str, "isoX : %d, isoY : %d, corner : %d",
		_isoX, _isoY, _center);
	TextOut(GetMemDC(), 700, 20, str, strlen(str));

	DrawTileMap();
}

void IsoMap::DrawTileMap()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			int left = _startX + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = _startY + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			if (left + CELL_WIDTH <0 || left > WINSIZEX
				|| top + CELL_HEIGHT < 0 || top > WINSIZEY) continue;

			IMAGE->Render("tile_grass", GetMemDC(), left, top);

			if (_isDebug) {
				if (_tileMap[i][j].isSelected) {
					HPEN myPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
					HPEN oldPen = (HPEN)SelectObject(GetMemDC(), myPen);
					DrawRhombus(left, top);
					DeleteObject(SelectObject(GetMemDC(), oldPen));
				}
				else {
					DrawRhombus(left, top);
					SetTextColor(GetMemDC(), RGB(0, 0, 0));
					sprintf_s(str, "(%d,%d)", i, j);
					TextOut(GetMemDC(), 
						left + RADIUS_WIDTH / 2 + 8, 
						top + RADIUS_HEIGHT / 2 + 5, str, strlen(str));
				}
			}
		}
	}
}

void IsoMap::DrawRhombus(int left, int top)
{
	int centerX = left + RADIUS_WIDTH;
	int centerY = top + RADIUS_HEIGHT;

	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - RADIUS_HEIGHT;

	p[1].x = centerX + RADIUS_WIDTH;
	p[1].y = centerY;

	p[2].x = centerX;
	p[2].y = centerY + RADIUS_HEIGHT;

	p[3].x = centerX - RADIUS_WIDTH;
	p[3].y = centerY;

	p[4].x = centerX;
	p[4].y = centerY - RADIUS_HEIGHT;

	for (int i = 1; i < 5; i++) {
		LineMake(GetMemDC(), p[i - 1], p[i]);
	}
}

int IsoMap::GetCornerIndex(int isoX, int isoY)
{
	int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
	int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

	float dx = (g_ptMouse.x - left) / (float)CELL_WIDTH;
	float dy = (g_ptMouse.y - top) / (float)CELL_HEIGHT;

	if (dx < 0.5f && dy < 0.5f) return 1;
	if (dx >= 0.5f && dy < 0.5f) return 2;
	if (dx < 0.5f && dy >= 0.5f) return 3;
	if (dx >= 0.5f && dy >= 0.5f) return 4;

	return 0;
}

bool IsoMap::IsInRhombus(int corner, int isoX, int isoY)
{
	// lefttop
	if (corner == 1) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) <= dx) return true;
		else return false;
	}
	// righttop
	else if (corner == 2) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy >= dx) return true;
		else return false;
	}
	// leftdown
	else if (corner == 3) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy < dx) return true;
		else return false;
	}
	// rightdown
	else if (corner == 4) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(g_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(g_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) > dx) return true;
		else return false;
	}

	return false;
}
