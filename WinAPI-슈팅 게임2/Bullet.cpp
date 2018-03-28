#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

HRESULT Bullet::Init(char* imageName, int bulletMax, float range)
{
	_imageName = imageName;
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void Bullet::Release()
{
}

void Bullet::Update()
{
	Move();
	Animation();
}

void Bullet::Render()
{
	for (tagBullet iter : _vBullet) {
		iter.bulletImage->FrameRender(GetMemDC(), iter.rc.left, iter.rc.top,
			(int)iter.moveFrame, 0);
	}
}

void Bullet::Fire(float x, float y, float angle, float speed)
{

	if (_bulletMax < _vBullet.size()) return;
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = IMAGE->FindImage(_imageName);
	bullet.angle = angle;
	bullet.speed = speed;
	bullet.fireX = bullet.x = x;
	bullet.fireY = bullet.y = y;
	bullet.rc = RectMakeCenter(
		bullet.x, bullet.y,
		bullet.bulletImage->GetWidth(),
		bullet.bulletImage->GetHeight());

	_vBullet.push_back(bullet);
}

void Bullet::Move()
{
	_viBullet = _vBullet.begin();

	for (; _viBullet != _vBullet.end();) {
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y -= sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(
			_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->GetFrameWidth(),
			_viBullet->bulletImage->GetFrameHeight());

		if (_range < GetDistance(
			_viBullet->fireX, _viBullet->fireY,
			_viBullet->x, _viBullet->y)) {
			_viBullet = _vBullet.erase(_viBullet);
		}
		else {
			++_viBullet;
		}
	}
}

void Bullet::Animation()
{
	for (_viBullet = _vBullet.begin();
		_viBullet != _vBullet.end(); ++_viBullet) {
		if (_viBullet->moveFrame < 10)
			_viBullet->moveFrame += 0.1f;
	}
}

bool Bullet::CheckCollision(RECT rc)
{
	RECT temp;

	//for (int i = 0; i < _vBullet.size(); i++) {
	//	if (IntersectRect(&temp, &_vBullet[i].rc, &rc)) {
	//		return true;
	//	}
	//}

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();) {
		if (IntersectRect(&temp, &(*_viBullet).rc, &rc)) {
			_viBullet = _vBullet.erase(_viBullet);
			return true;
		}
		else
			++_viBullet;
	}

	return false;
}

/******************************************************/

HRESULT Missile::Init(int bulletMax, float range)
{
	_range = range;

	for (int i = 0; i < bulletMax; i++) {
		tagBullet bullet;

		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = IMAGE->FindImage("missile");
		bullet.speed = 2.5f;
		bullet.fire = false;
		bullet.moveFrame = 0;

		_vBullet.push_back(bullet);
	}

	return S_OK;
}

void Missile::Release()
{
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

bool Missile::CheckCollision(RECT rc)
{
	RECT temp;

	for (int i = 0; i < _vBullet.size(); i++) {
		if (!_vBullet[i].fire) continue;

		if (IntersectRect(&temp, &_vBullet[i].rc, &rc)) {
			_vBullet[i].fire = false;
			return true;
		}
	}

	return false;
}
