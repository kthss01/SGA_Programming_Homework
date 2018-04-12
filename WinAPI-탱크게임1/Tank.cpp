#include "stdafx.h"
#include "Tank.h"


Tank::Tank()
{
}


Tank::~Tank()
{
}

HRESULT Tank::Init()
{
	_direction = TANKDIRECTION_RIGHT;

	_image = IMAGE->AddImage("tank", "images/tank.bmp", 0, 0,
		256, 128, 8, 4, true, RGB(255, 0, 255));

	_speed = 100.0f;

	return S_OK;
}

void Tank::Release()
{
}

void Tank::Update()
{
	// Left Move
	if (INPUT->GetKey(VK_LEFT) && _x - (_rc.right - _rc.left) / 2 > 0) {
		_direction = TANKDIRECTION_LEFT;
		TankMove();
	}
	// Right Move
	if (INPUT->GetKey(VK_RIGHT) && _x + (_rc.right - _rc.left) / 2 < WINSIZEX) {
		_direction = TANKDIRECTION_RIGHT;
		TankMove();
	}
	// Up Move
	if (INPUT->GetKey(VK_UP) && _y - (_rc.bottom - _rc.top) / 2 > 0) {
		_direction = TANKDIRECTION_UP;
		TankMove();
	}
	// Down Move
	if (INPUT->GetKey(VK_DOWN) && _y + (_rc.bottom - _rc.top) / 2 < WINSIZEY) {
		_direction = TANKDIRECTION_DOWN;
		TankMove();
	}

	if (INPUT->GetKeyDown(VK_SPACE)) {
		SOUND->Play("Fire", 0.5f);
		TankFire();
	}

	_rc = RectMakeCenter(_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());
}

void Tank::Render()
{
	_image->FrameRender(GetMemDC(), _rc.left, _rc.top);
	//_image->Render(GetMemDC());
}

void Tank::TankMove()
{
	RECT rcCollision;	// 충돌 렉트
	// 0은 내가 밟고있는 타일, 1은 내가 진행하고 있는 방향의 타일
	int tileIndex[2];	
	int tileX, tileY;	// 타일 x,y
	rcCollision = _rc;

	float elapsedTime = FRAME->GetElapsedTime();
	float moveSpeed = elapsedTime * _speed;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		_image->SetFrameX(0);
		_image->SetFrameY(3);
		_x -= moveSpeed;
		rcCollision = RectMakeCenter(_x, _y, 
				_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	case TANKDIRECTION_RIGHT:
		_image->SetFrameX(0);
		_image->SetFrameY(2);
		_x += moveSpeed;
		rcCollision = RectMakeCenter(_x, _y,
			_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	case TANKDIRECTION_UP:
		_image->SetFrameX(0);
		_image->SetFrameY(0);
		_y -= moveSpeed;
		rcCollision = RectMakeCenter(_x, _y,
			_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	case TANKDIRECTION_DOWN:
		_image->SetFrameX(0);
		_image->SetFrameY(1);
		_y += moveSpeed;
		rcCollision = RectMakeCenter(_x, _y,
			_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	}
	
	// 잘 통과하기 위해서 충돌판정하는 rc값만 1씩 작게 해주는거
	rcCollision.left += 1;
	rcCollision.top += 1;
	rcCollision.right -= 1;
	rcCollision.bottom -= 1;

	// 현태 탱크가 어느 타일 위치에 있는지 계산
	// 탱크 위치가 1, 40 이면 1 / 32 -> 0 , 40 / 32 -> 1이라 (0,1) 타일 위치
	// 원래대로 따지면 중앙이 정확한데 left, top으로 설정하신거
	tileX = (rcCollision.left + (rcCollision.right - rcCollision.left) / 2) / TILESIZE;
	tileY = (rcCollision.top + (rcCollision.bottom - rcCollision.top) / 2) / TILESIZE;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = (tileX - 1) + tileY * TILEX;
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = (tileX + 1) + tileY * TILEX;
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY - 1) * TILEX;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + (tileY + 1) * TILEX;
		break;
	}
	
	for (int i = 0; i < 2; i++) {
		RECT temp;
		if ((_tankMap->GetAttribute()[tileIndex[i]] & ATTR_UNMOVE == ATTR_UNMOVE)
			&& IntersectRect(&temp, &_tankMap->GetTiles()[tileIndex[i]].rc,
				&rcCollision)) {
			switch (_direction)
			{
				// 탱크의 위치 원위치로
			case TANKDIRECTION_LEFT:
				_rc.left = _tankMap->GetTiles()[tileIndex[i]].rc.right;
				_rc.right = _rc.left + 32;
				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_RIGHT:
				_rc.right = _tankMap->GetTiles()[tileIndex[i]].rc.left;
				_rc.left = _rc.right - 32;
				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_UP:
				_rc.top = _tankMap->GetTiles()[tileIndex[i]].rc.bottom;
				_rc.bottom = _rc.top + 32;
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			case TANKDIRECTION_DOWN:
				_rc.bottom = _tankMap->GetTiles()[tileIndex[i]].rc.top;
				_rc.top = _rc.bottom - 32;
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
		}
	}
	// 할 필요는 없음
	rcCollision = RectMakeCenter(_x, _y, 
		_image->GetFrameWidth(), _image->GetFrameHeight());
}

void Tank::TankFire()
{
	int tileX = (_rc.left + (_rc.right - _rc.left) / 2) / TILESIZE;
	int tileY = (_rc.top + (_rc.bottom - _rc.top) / 2) / TILESIZE;

	POINT pos;

	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		for (int i = tileX - 1; i >= 0; i--) {
			if (_tankMap->GetTiles()[i + tileY * TILEX].obj <= OBJ_BLOCK3) {
				if (_tankMap->GetTiles()[i + tileY * TILEX].obj == OBJ_BLOCK1) {
					_tankMap->GetTiles()[i + tileY * TILEX].obj = OBJ_NONE;
					_tankMap->GetAttribute()[i + tileY * TILEX] = 0;
				}
				else {
					_tankMap->GetTiles()[i + tileY * TILEX].obj =
						(OBJECT)(_tankMap->GetTiles()[i + tileY * TILEX].obj - 1);
					pos = _tankMap->
						GetOBJECTFrame(_tankMap->GetTiles()[i + tileY * TILEX].obj);
					_tankMap->GetTiles()[i + tileY * TILEX].objFrameX = pos.x;
					_tankMap->GetTiles()[i + tileY * TILEX].objFrameY = pos.y;
				}
				break;
			}
		}
		break;
	case TANKDIRECTION_RIGHT:
		for (int i = tileX + 1; i < TILEX; i++) {
			if (_tankMap->GetTiles()[i + tileY * TILEX].obj <= OBJ_BLOCK3) {
				if (_tankMap->GetTiles()[i + tileY * TILEX].obj == OBJ_BLOCK1) {
					_tankMap->GetTiles()[i + tileY * TILEX].obj = OBJ_NONE;
					_tankMap->GetAttribute()[i + tileY * TILEX] = 0;
				}
				else {
					_tankMap->GetTiles()[i + tileY * TILEX].obj =
						(OBJECT)(_tankMap->GetTiles()[i + tileY * TILEX].obj - 1);
					pos = _tankMap->
						GetOBJECTFrame(_tankMap->GetTiles()[i + tileY * TILEX].obj);
					_tankMap->GetTiles()[i + tileY * TILEX].objFrameX = pos.x;
					_tankMap->GetTiles()[i + tileY * TILEX].objFrameY = pos.y;
				}
				break;
			}
		}
		break;
	case TANKDIRECTION_UP:
		for (int i = tileY - 1; i >= 0; i--) {
			if (_tankMap->GetTiles()[tileX + i * TILEX].obj <= OBJ_BLOCK3) {
				if (_tankMap->GetTiles()[tileX + i * TILEX].obj == OBJ_BLOCK1) {
					_tankMap->GetTiles()[tileX + i * TILEX].obj = OBJ_NONE;
					_tankMap->GetAttribute()[tileX + i * TILEX] = 0;
				}
				else {
					_tankMap->GetTiles()[tileX + i * TILEX].obj =
						(OBJECT)(_tankMap->GetTiles()[tileX + i * TILEX].obj - 1);
					pos = _tankMap->
						GetOBJECTFrame(_tankMap->GetTiles()[tileX + i * TILEX].obj);
					_tankMap->GetTiles()[tileX + i * TILEX].objFrameX = pos.x;
					_tankMap->GetTiles()[tileX + i * TILEX].objFrameY = pos.y;
				}
				break;
			}
		}
		break;
	case TANKDIRECTION_DOWN:
		for (int i = tileY + 1; i < TILEY; i++) {
			if (_tankMap->GetTiles()[tileX + i * TILEX].obj <= OBJ_BLOCK3) {
				if (_tankMap->GetTiles()[tileX + i * TILEX].obj == OBJ_BLOCK1) {
					_tankMap->GetTiles()[tileX + i * TILEX].obj = OBJ_NONE;
					_tankMap->GetAttribute()[tileX + i * TILEX] = 0;
				}
				else {
					_tankMap->GetTiles()[tileX + i * TILEX].obj =
						(OBJECT)(_tankMap->GetTiles()[tileX + i * TILEX].obj - 1);
					pos = _tankMap->
						GetOBJECTFrame(_tankMap->GetTiles()[tileX + i * TILEX].obj);
					_tankMap->GetTiles()[tileX + i * TILEX].objFrameX = pos.x;
					_tankMap->GetTiles()[tileX + i * TILEX].objFrameY = pos.y;
				}
				break;
			}
		}
		break;
	}
}

void Tank::SetTankPosition()
{
	_rc = _tankMap->GetTiles()[_tankMap->GetPosFirst()].rc;
	_x = _rc.left + (_rc.right - _rc.left) / 2;
	_y = _rc.top + (_rc.bottom - _rc.top) / 2;
}
