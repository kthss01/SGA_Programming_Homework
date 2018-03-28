#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
	Release();
}

HRESULT Enemy::Init(int enemyMax)
{
	_enemyImage = new Image;
	_enemyImage->Init("images/alien_idle_16x1.bmp",
		50 * 16, 50, 16, 1, true, RGB(255, 0, 255));

	for (int i = 0; i < enemyMax; i++) {
		tagEnemy enemy;
		ZeroMemory(&enemy, sizeof(tagEnemy));
		//enemy.enemyImage = new Image;
		//enemy.enemyImage
		//	->Init("images/alien_idle_16x1.bmp",
		//		50 * 16, 50, 16, 1, true, RGB(255, 0, 255));
		//enemy.width = enemy.enemyImage->GetFrameWidth();
		//enemy.height = enemy.enemyImage->GetFrameHeight();
		enemy.width = _enemyImage->GetFrameWidth();
		enemy.height = _enemyImage->GetFrameHeight();
		enemy.speed = 2.5f;
		//enemy.moveFrame = 0;
		enemy.moveFrame = RND->GetFromInto(0,16);

		enemy.x = 60 * (i % 8) + 350;
		enemy.y = 60 * (i / 8) + 150;
		enemy.rc = RectMakeCenter(enemy.x, enemy.y, enemy.width, enemy.height);

		_vEnemy.push_back(enemy);
	}

	_isLeft = true;
	_isDown = false;
	_isInit = false;

	_bullet = new Bullet;
	_bullet->Init(100 * 32);

	return S_OK;
}

void Enemy::Release()
{
	//for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy) {
	//	SAFE_DELETE(_viEnemy->enemyImage);
	//}
	SAFE_DELETE(_enemyImage);
	SAFE_DELETE(_bullet);
}

void Enemy::Update()
{
	_bullet->Update();

	for (int i = 0; i < _vEnemy.size(); i++) {
		_vEnemy[i].moveFrame += 0.25f;
		if (_vEnemy[i].moveFrame >= 16)
			_vEnemy[i].moveFrame = 0;
	}
}

void Enemy::Render()
{
	_bullet->Render();
	for (tagEnemy enemy : _vEnemy) {
		_enemyImage->FrameRender(GetMemDC(), enemy.rc.left, enemy.rc.top,
			(int)enemy.moveFrame, 0);
	}
}

void Enemy::Move()
{
	if (_isLeft && !_isDown) {
		for (int i = 0; i < _vEnemy.size(); i++) {
			_vEnemy[i].x -= _vEnemy[i].speed;
			
			if (_vEnemy[i].x - _vEnemy[i].width / 2 < 0) {
				_isLeft = false;
				_isDown = true;
				_delay = 0;
			}
			
			_vEnemy[i].rc = RectMakeCenter(
				_vEnemy[i].x, _vEnemy[i].y, _vEnemy[i].width, _vEnemy[i].height);
		}
	}
	else if (!_isLeft && !_isDown) {
		for (int i = 0; i < _vEnemy.size(); i++) {
			_vEnemy[i].x += _vEnemy[i].speed;
			
			if (_vEnemy[i].x + _vEnemy[i].width / 2 > WINSIZEX) {
				_isLeft = true;
				_isDown = true;
				_delay = 0;
			}

			_vEnemy[i].rc = RectMakeCenter(
				_vEnemy[i].x, _vEnemy[i].y, _vEnemy[i].width, _vEnemy[i].height);
		}
	}

	if (_isDown) {
		for (int i = 0; i < _vEnemy.size(); i++) {
			_vEnemy[i].y += _vEnemy[i].speed;

			_delay++;
			if (_delay % 300 == 0)
				_isDown = false;

			if (_vEnemy[i].y + _vEnemy[i].height / 2 > WINSIZEY) {
				_isInit = true;
			}

			_vEnemy[i].rc = RectMakeCenter(
				_vEnemy[i].x, _vEnemy[i].y, _vEnemy[i].width, _vEnemy[i].height);
		}
	}

	if (_isInit) {
		_isInit = false;

		for (int i = 0; i < _vEnemy.size(); i++) {
			_vEnemy[i].x = 50 * (i % 8) + 350;
			_vEnemy[i].y = 50 * (i / 8) + 150;
			_vEnemy[i].rc = RectMakeCenter(
				_vEnemy[i].x, _vEnemy[i].y, _vEnemy[i].width, _vEnemy[i].height);
		}
	}
}

void Enemy::Fire(float destX, float destY)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy) {
		_bullet->Fire(_viEnemy->x, _viEnemy->y, destX, destY);
	}
}
