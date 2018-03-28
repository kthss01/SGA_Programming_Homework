#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

HRESULT Enemy::Init()
{
	_image = IMAGE->FindImage("alien");

	return S_OK;
}

HRESULT Enemy::Init(const char * imageName, POINT position)
{
	_count = 0;

	_fireCount = 0;
	_rndFireCount = RND->GetFromInto(1, FIRECOUNT);
	_image = IMAGE->FindImage(imageName);
	_pos = position;
	_rc = RectMakeCenter(
		_pos.x, _pos.y,
		_image->GetFrameWidth(),
		_image->GetFrameHeight());

	_currentFrameX = RND->GetFromInto(0, _image->GetMaxFrameX());
	_currentFrameY = 0;

	_initPos = position;
	_isLive = true;

	_speed = 2.5f;

	return S_OK;
}

void Enemy::Release()
{
}

void Enemy::Update()
{
	Animation();
}

void Enemy::Render()
{
	Draw();
}

void Enemy::Move(Direction dir)
{
	switch (dir)
	{
	case DIRECTION_LEFT:
		_pos.x -= _speed;
		break;
	case DIRECTION_RIGHT:
		_pos.x += _speed;
		break;
	case DIRECTION_DOWN:
		_pos.y += _speed;
		break;
	}

	_rc = RectMakeCenter(
		_pos.x, _pos.y,
		_image->GetFrameWidth(),
		_image->GetFrameHeight());
}

void Enemy::Draw()
{
	_image->FrameRender(GetMemDC(),
		_rc.left, _rc.top, _currentFrameX, _currentFrameY);
}

void Enemy::Animation()
{
	_count++;
	if (_count % 5 == 0) {
		_currentFrameX++;
		if (_currentFrameX >= _image->GetMaxFrameX())
			_currentFrameX = 0;
	}
}

bool Enemy::BulletCountFire()
{
	_fireCount++;
	if (_fireCount % _rndFireCount == 0) {
		_rndFireCount = RND->GetFromInto(1, FIRECOUNT);
		_fireCount = 0;
		return true;
	}
	return false;
}

bool Enemy::CheckCollision(Direction dir)
{
	switch (dir)
	{
	case DIRECTION_LEFT:
		return _rc.left < 0;
		break;
	case DIRECTION_RIGHT:
		return _rc.right > WINSIZEX;
		break;
	case DIRECTION_DOWN:
		return _rc.bottom > WINSIZEY;
		break;
	}
}

void Enemy::SetPosition(POINT pos)
{
	_rc = RectMakeCenter(
		pos.x, pos.y,
		_image->GetFrameWidth(),
		_image->GetFrameHeight());
}
