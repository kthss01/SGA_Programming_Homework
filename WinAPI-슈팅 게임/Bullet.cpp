#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

HRESULT Bullet::Init(int bulletMax)
{
	for (int i = 0; i < bulletMax; i++) {
		tagBullet bullet;

		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = new Image;
		bullet.bulletImage
			->Init("images/bullet_blue_10x1.bmp", 25 * 10, 
				25, 10, 1, true, RGB(255, 0, 255));
		bullet.speed = 2.5f;
		bullet.fire = false;

		_vBullet.push_back(bullet);
	}

	return S_OK;
}

void Bullet::Release()
{
	for (_viBullet = _vBullet.begin();
		_viBullet != _vBullet.end(); ++_viBullet) {
		SAFE_DELETE(_viBullet->bulletImage);
	}
}

void Bullet::Update()
{
	Move();
	for (_viBullet = _vBullet.begin();
		_viBullet != _vBullet.end(); ++_viBullet) {
		if (!_viBullet->fire) continue;

		if(_viBullet->moveFrame < 10)
			_viBullet->moveFrame += 0.1f;
	}
}

void Bullet::Render()
{
	for (tagBullet iter : _vBullet) {
		if (!iter.fire) continue;

		iter.bulletImage->FrameRender(GetMemDC(), iter.rc.left, iter.rc.top,
			(int)iter.moveFrame, 0);
	}
}

void Bullet::Fire(float x, float y, float destX, float destY)
{
	_viBullet = _vBullet.begin();

	for (; _viBullet != _vBullet.end(); ++_viBullet) {
		if (_viBullet->fire) continue;

		_viBullet->fire = true;
		_viBullet->moveFrame = 0;
		_viBullet->x = x;
		_viBullet->y = y;
		_viBullet->angle = GetAngle(x, y, destX, destY);
		_viBullet->rc = RectMakeCenter(
			_viBullet->x,
			_viBullet->y,
			_viBullet->bulletImage->GetFrameWidth(),
			_viBullet->bulletImage->GetFrameHeight());
		break;
	}
}

void Bullet::Move()
{
	_viBullet = _vBullet.begin();

	for (; _viBullet != _vBullet.end(); ++_viBullet) {
		if (!_viBullet->fire) continue;

		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y -= sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(
			_viBullet->x,
			_viBullet->y,
			_viBullet->bulletImage->GetFrameWidth(),
			_viBullet->bulletImage->GetFrameHeight());

		if (_viBullet->rc.top >= WINSIZEY)
			_viBullet->fire = false;
	}
}

/******************************************************/

HRESULT Missile::Init(int bulletMax, float range)
{
	_range = range;

	for (int i = 0; i < bulletMax; i++) {
		tagBullet bullet;

		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = new Image;
		bullet.bulletImage
			->Init("images/missile.bmp", 26, 124,
				true, RGB(255, 0, 255));
		bullet.speed = 2.5f;
		bullet.fire = false;
		bullet.moveFrame = 0;

		_vBullet.push_back(bullet);
	}

	return S_OK;
}

void Missile::Release()
{
	for (_viBullet = _vBullet.begin();
		_viBullet != _vBullet.end(); ++_viBullet) {
		SAFE_DELETE(_viBullet->bulletImage);
	}
}

void Missile::Update()
{
	Move();
}

void Missile::Render()
{
	for (tagBullet iter : _vBullet) {
		if (!iter.fire) continue;

		iter.bulletImage->Render(GetMemDC(), iter.rc.left, iter.rc.top);
	}
}

void Missile::Fire(float x, float y)
{
	_viBullet = _vBullet.begin();

	for (; _viBullet != _vBullet.end(); ++_viBullet) {
		if (_viBullet->fire) continue;

		_viBullet->fire = true;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->rc = RectMakeCenter(
			_viBullet->x,
			_viBullet->y,
			_viBullet->bulletImage->GetWidth(),
			_viBullet->bulletImage->GetHeight());
		break;
	}
}

void Missile::Move()
{
	_viBullet = _vBullet.begin();

	for (; _viBullet != _vBullet.end(); ++_viBullet) {
		if (!_viBullet->fire) continue;

		_viBullet->y -= _viBullet->speed;

		_viBullet->rc = RectMakeCenter(
			_viBullet->x,
			_viBullet->y,
			_viBullet->bulletImage->GetWidth(),
			_viBullet->bulletImage->GetHeight());

		if (_range < GetDistance(
			_viBullet->fireX, _viBullet->fireY,
			_viBullet->x, _viBullet->y)) {
			_viBullet->fire = false;
		}
	}
}

void Missile::Bomb()
{
	_viBullet = _vBullet.begin();
	for (; _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire) continue;

		_viBullet->fire = false;
	}
}
