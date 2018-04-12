#include "stdafx.h"
#include "Enemy.h"

#include "Tank.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

HRESULT Enemy::Init()
{
	_direction = TANKDIRECTION_UP;
	frameX = 0;
	frameY = 0;

	_image = IMAGE->AddImage("tank", "images/tank.bmp", 0, 0,
		256, 128, 8, 4, true, RGB(255, 0, 255));

	_speed = 100.0f;

	cnt = 0;
	currentDir = _direction;

	isLive = true;

	return S_OK;
}

void Enemy::Release()
{
}

void Enemy::Update()
{
	// 죽었거나 탱크가 죽으면 Update() 진행안하고 반환
	if (!isLive || !tank->GetIsLive()) return;

	// 그냥 랜덤으로 이동
	
	cnt++;
	if (cnt % 50 == 0)
		currentDir = (TANKDIRECTION)RND->GetInt(4);

	switch (currentDir) {
	case 0:
		// Left Move
		if (_x - (_rc.right - _rc.left) / 2 > 0) {
			_direction = TANKDIRECTION_LEFT;
			ray = Ray(_x, _y, -1 * WINSIZEX, 0 + _y);
			TankMove();
		}
		break;
	case 1:
		// Right Move
		if (_x + (_rc.right - _rc.left) / 2 < WINSIZEX) {
			_direction = TANKDIRECTION_RIGHT;
			ray = Ray(_x, _y, 1 * WINSIZEX, 0 + _y);
			TankMove();
		}
		break;
	case 2:
		// Up Move
		if (_y - (_rc.bottom - _rc.top) / 2 > 0) {
			_direction = TANKDIRECTION_UP;
			ray = Ray(_x, _y, 0 + _x, -1 * WINSIZEY);
			TankMove();
		}
		break;
	case 3:
		// Down Move
		if (_y + (_rc.bottom - _rc.top) / 2 < WINSIZEY) {
			_direction = TANKDIRECTION_DOWN;
			ray = Ray(_x, _y, 0 + _x, 1 * WINSIZEY);
			TankMove();
		}
		break;
	}

	_rc = RectMakeCenter(_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());


	vTileIndex.clear();

	for (int i = 0; i < TILEX*TILEY; i++) {
		if (_tankMap->GetTiles()[i].obj == OBJ_NONE) continue;

		if (_tankMap->GetTiles()[i].obj <= OBJ_BLOCK3) {
			if (ray.CollisionRect(_tankMap->GetTiles()[i].rc)
				//&& _tankMap->GetTiles()[i].obj != OBJ_NONE
				) {
				vTileIndex.push_back(i);
			}
		}

		//if (_direction == TANKDIRECTION_LEFT
		//	|| _direction == TANKDIRECTION_UP) {
		//	reverse(vTile.begin(), vTile.end());
		//}
	}

	// 총알 발사
	// 레이에 타일들 하나라도 걸리면 총알 발사
	// 또는 탱크가 레이에 걸려도 발사
	if (vTileIndex.size() != 0 ||
		ray.CollisionRect(tank->GetRect())) {
		if (cnt % 25 == 0) {
			SOUND->Play("Fire", 0.5f);
			TankFire();
		}
	}
}

void Enemy::Render()
{
	// 죽었거나 탱크가 죽으면 Render() 진행안하고 반환
	if (!isLive || !tank->GetIsLive()) return;

	ray.DrawRay(GetMemDC());
	_image->FrameRender(GetMemDC(), _rc.left, _rc.top, frameX, frameY);
	//_image->Render(GetMemDC());
}

void Enemy::TankMove()
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
		frameY = 3;
		_x -= moveSpeed;
		rcCollision = RectMakeCenter(_x, _y,
			_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	case TANKDIRECTION_RIGHT:
		frameY = 2;
		_x += moveSpeed;
		rcCollision = RectMakeCenter(_x, _y,
			_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	case TANKDIRECTION_UP:
		frameY = 0;
		_y -= moveSpeed;
		rcCollision = RectMakeCenter(_x, _y,
			_image->GetFrameWidth(), _image->GetFrameHeight());
		break;
	case TANKDIRECTION_DOWN:
		frameY = 1;
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

void Enemy::TankFire()
{
	if (vTileIndex.size() != 0) {
		// 가장 가까운 타일 찾기
		tagTile temp = _tankMap->GetTiles()[vTileIndex[0]];

		int distance = GetDistance(_x, _y,
			temp.rc.left + (temp.rc.right - temp.rc.left) / 2,
			temp.rc.top + (temp.rc.bottom - temp.rc.top) / 2);
		int index = vTileIndex[0];
		for (int i = 1; i < vTileIndex.size(); i++) {
			tagTile tempTile = _tankMap->GetTiles()[vTileIndex[i]];
			int temp = GetDistance(_x, _y,
				tempTile.rc.left + (tempTile.rc.right - tempTile.rc.left) / 2,
				tempTile.rc.top + (tempTile.rc.bottom - tempTile.rc.top) / 2);
			if (temp < distance) {
				distance = temp;
				index = vTileIndex[i];
			}
		}

		// 탱크와의 거리 계산해서 찾은 타일보다 탱크가 더 가까우면 에너미 제거
		if (ray.CollisionRect(tank->GetRect()) &&
			tank->GetIsLive() &&
			distance > GetDistance(_x, _y,
				tank->GetRect().left + (tank->GetRect().right - tank->GetRect().left) / 2,
				tank->GetRect().top + (tank->GetRect().bottom - tank->GetRect().top) / 2)) {
			tank->SetIsLive(false);
		}
		// 타일이 가까우면 타일 제거
		else {
			// 타일 처리
			POINT pos;

			if (_tankMap->GetTiles()[index].obj == OBJ_BLOCK1) {
				_tankMap->GetTiles()[index].obj = OBJ_NONE;
				_tankMap->GetAttribute()[index] = 0;
			}
			else {
				_tankMap->GetTiles()[index].obj =
					(OBJECT)(_tankMap->GetTiles()[index].obj - 1);
				pos = _tankMap->
					GetOBJECTFrame(_tankMap->GetTiles()[index].obj);
				_tankMap->GetTiles()[index].objFrameX = pos.x;
				_tankMap->GetTiles()[index].objFrameY = pos.y;
			}
		}
	}
	// 제거할 타일이 없을 때
	else {
		// 탱크 제거
		if (ray.CollisionRect(tank->GetRect())) {
			tank->SetIsLive(false);
		}
	}
}

void Enemy::SetTankPosition()
{
	_rc = _tankMap->GetTiles()[_tankMap->GetPosSecond()].rc;
	_x = _rc.left + (_rc.right - _rc.left) / 2;
	_y = _rc.top + (_rc.bottom - _rc.top) / 2;
}
