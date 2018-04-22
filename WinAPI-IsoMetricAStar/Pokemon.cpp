#include "stdafx.h"
#include "Pokemon.h"


Pokemon::Pokemon()
{
}


Pokemon::~Pokemon()
{
}

HRESULT Pokemon::Init()
{
	_image = IMAGE->FindImage("pokemon");

	_startX = _map->GetStartPos().x;
	_startY = _map->GetStartPos().y;

	_aStarStartX = 1;
	_aStarStartY = 1;

	_x = _map->GetIsoPos(_aStarStartX, _aStarStartY).x 
		- _image->GetFrameWidth() / 2;
	_y = _map->GetIsoPos(_aStarStartX, _aStarStartY).y 
		- _image->GetFrameHeight() + 10;
	_rc = RectMake(_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());

	_state = POKEMONSTATE_IDLE;
	_direction = POKEMONDIRECTION_LEFTDOWN;

	frameX = FindDirectionFrame(_direction);
	frameY = 0;

	_speed = 5;
	_cnt = 0;

	return S_OK;
}

void Pokemon::Release()
{
}

void Pokemon::Update()
{
	//// test
	//if (INPUT->GetKeyDown(VK_SPACE)) {
	//	frameX++;
	//	if (frameX > 24)
	//		frameX = 0;
	//}

	if (!SUBWIN->GetIsActive() &&
		INPUT->GetKey(VK_LBUTTON)) {
		float cellX = (float)(g_ptMouse.x - _startX);

		if (cellX < 0) {
			cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
		}
		else {
			cellX = cellX / (float)CELL_WIDTH;
		}

		float cellY = abs(g_ptMouse.y - _startY) / CELL_HEIGHT;

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

			tagTile(*tiles)[TILE_COUNT_Y] = _map->GetTiles();
			
			if (tiles[isoX][isoY].index == 0 &&
				tiles[isoX][isoY].tileKind[0] == TILEKIND_TERRAIN) {
				_endX = isoX;
				_endY = isoY;
				PathInit();
				PathFind();
			}
		}
	}

	if (_targetTile != NULL)
		PokemonMove();
}

void Pokemon::Render()
{
	_image->FrameRender(GetMemDC(), _rc.left, _rc.top, frameX, frameY);
}

int Pokemon::FindDirectionFrame(POKEMONDIRECTION dir)
{
	return dir * 3;
}

void Pokemon::PokemonMove()
{
	int isoX = _targetTile->isoX;
	int isoY = _targetTile->isoY;

	float destX, destY;
	destX = _map->GetIsoPos(isoX, isoY).x - _image->GetFrameWidth() / 2;
	destY = _map->GetIsoPos(isoX, isoY).y - _image->GetFrameHeight() + 10;

	if (GetDistance(_x, _y, destX, destY) > 10) {
		float angle = GetAngle(_x, _y, destX, destY);

		if (_state == POKEMONSTATE_MOVE) {
			_cnt++;
			if(_cnt % 5 == 0)
				frameX++;
			if (frameX >= FindDirectionFrame(_direction) + 3)
				frameX = FindDirectionFrame(_direction);
		}
		if (_state == POKEMONSTATE_IDLE) {
			_state = POKEMONSTATE_MOVE;

			float angleTemp = angle;
			if (angleTemp <= 0) angleTemp += PI * 2;
			if (angleTemp >= PI * 2) angleTemp -= PI * 2;

			angleTemp = angleTemp + (PI / 8);
			int dir = int(angleTemp / (PI / 4));

			switch (dir) {
			case 0:
				_direction = POKEMONDIRECTION_RIGHT;
				break;
			case 1:
				_direction = POKEMONDIRECTION_RIGHTUP;
				break;
			case 2:
				_direction = POKEMONDIRECTION_UP;
				break;
			case 3:
				_direction = POKEMONDIRECTION_LEFTUP;
				break;
			case 4:
				_direction = POKEMONDIRECTION_LEFT;
				break;
			case 5:
				_direction = POKEMONDIRECTION_LEFTDOWN;
				break;
			case 6:
				_direction = POKEMONDIRECTION_DOWN;
				break;
			case 7:
				_direction = POKEMONDIRECTION_RIGHTDOWN;
				break;
			}

			frameX = FindDirectionFrame(_direction);
		}

		_x += cosf(angle) * _speed;
		_y -= sinf(angle) * _speed;
		_rc = RectMake(_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());
	}
	else {
		_x = destX;
		_y = destY;
		_rc = RectMake(_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());
		_aStarStartX = _targetTile->isoX;
		_aStarStartY = _targetTile->isoY;
		_targetTile = _targetTile->parent;
		_state = POKEMONSTATE_IDLE;

		if (_targetTile == NULL) {
			frameX = FindDirectionFrame(_direction);
			_aStarStartX = _endX;
			_aStarStartY = _endY;
		}
		_cnt = 0;
	}
}

int Pokemon::GetCornerIndex(int isoX, int isoY)
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

bool Pokemon::IsInRhombus(int corner, int isoX, int isoY)
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

void Pokemon::PathInit()
{
	tagTile(*tiles)[TILE_COUNT_Y] = _map->GetTiles();

	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_aStarTile[i][j].listOn = false;
			_aStarTile[i][j].parent = NULL;
			_aStarTile[i][j].F = BIGNUM;
			_aStarTile[i][j].G = 0;
			_aStarTile[i][j].H = 0;
			_aStarTile[i][j].isoX = i;
			_aStarTile[i][j].isoY = j;

			if (tiles[i][j].index == 0 &&
				tiles[i][j].tileKind[tiles[i][j].index] ==
				TILEKIND_TERRAIN)
				_aStarTile[i][j].walkable = true;
			else
				_aStarTile[i][j].walkable = false;
		}
	}

	_openList.clear();
	_closeList.clear();

	_aStarTile[_endX][_endY].listOn = true;
	_Cx = _aStarTile[_endX][_endY].isoX;
	_Cy = _aStarTile[_endX][_endY].isoY;
	_Cg = _aStarTile[_endX][_endY].G;

	_closeList.push_back(&_aStarTile[_endX][_endY]);

	_lastIndex = 0;
	_targetTile = NULL;

	_state = POKEMONSTATE_IDLE;
}

void Pokemon::PathFind()
{
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool check[8];

	while (true) {
		for (int i = 0; i < 8; i++) check[i] = false;

		// AddOpenList()
		_Cx = _closeList[_lastIndex]->isoX;
		_Cy = _closeList[_lastIndex]->isoY;
		_Cg = _closeList[_lastIndex]->G;

		for (int i = 0; i < 8; i++) {
			int x = _Cx + dx[i];
			int y = _Cy + dy[i];
			if (0 <= x && x < TILE_COUNT_X && 0 <= y && y < TILE_COUNT_Y) {
				if (_aStarTile[x][y].walkable) {
					check[i] = true;
					if (i < 4) {
						if (!_aStarTile[x][y].listOn) {
							_aStarTile[x][y].listOn = true;
							_aStarTile[x][y].G = _Cg + 10;
							_aStarTile[x][y].parent =
								_closeList[_lastIndex];
							_openList.push_back(&_aStarTile[x][y]);
						}
						else {
							if (_Cg + 10 < _aStarTile[x][y].G) {
								_aStarTile[x][y].G = _Cg + 10;
								_aStarTile[x][y].parent =
									_closeList[_lastIndex];
							}
						}
					}
					else {
						// leftup
						if ((i == 4 && check[0] && check[2]) ||
							// rightdown
							(i == 5 && check[1] && check[3]) ||
							// leftdown	
							(i == 6 && check[0] && check[3]) ||
							// rightup
							(i == 7 && check[1] && check[2])) {

							if (!_aStarTile[x][y].listOn) {
								_aStarTile[x][y].listOn = true;
								_aStarTile[x][y].G = _Cg + 14;
								_aStarTile[x][y].parent =
									_closeList[_lastIndex];
								_openList.push_back(&_aStarTile[x][y]);
							}
							else {
								if (_Cg + 14 < _aStarTile[x][y].G) {
									_aStarTile[x][y].G = _Cg + 14;
									_aStarTile[x][y].parent =
										_closeList[_lastIndex];
								}
							}
						}
					}
				}
			}
		}

		// no way
		if (_openList.size() == 0) {
			_targetTile = NULL;
			break;
		}

		// CalCulate H, F
		for (int i = 0; i < _openList.size(); i++) {
			int vertical = (_aStarStartX - _openList[i]->isoX) * 10;
			int horizontal = (_aStarStartY - _openList[i]->isoY) * 10;

			// 방향이 반대로 넘어가는 경우
			if (vertical < 0) vertical *= -1;
			if (horizontal < 0) horizontal *= -1;

			_openList[i]->H = vertical + horizontal;

			_openList[i]->F = _openList[i]->G + _openList[i]->H;
		}

		// AddCloseList()
		int index = 0;
		int min = BIGNUM;

		for (int i = 0; i < _openList.size(); i++) {
			if (_openList[i]->F < min) {
				min = _openList[i]->F;
				index = i;
			}
		}

		_closeList.push_back(_openList[index]);
		_openList.erase(_openList.begin() + index);

		_lastIndex++;

		// Check Arrive()
		// found path
		if (_closeList[_lastIndex]->isoX == _aStarStartX &&
			_closeList[_lastIndex]->isoY == _aStarStartY) {
			_targetTile = _closeList[_lastIndex]->parent;
			break;
		}
	}
}
