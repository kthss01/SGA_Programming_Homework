#include "stdafx.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::Init()
{
	_bullet = new Bullet;
	_bullet->Init((char*)"bullet", 100, 1000);

	this->SetAlien();

	_isLeft = true;
	_isDown = false;
	_isInit = false;

	return S_OK;
}

void EnemyManager::Release()
{
	for (int i = 0; i < _vAlien.size(); i++) {
		SAFE_DELETE(_vAlien[i]);
	}
}

void EnemyManager::Update()
{
	//for (int i = 0; i < _vAlien.size(); i++) {
	//	_vAlien[i]->Update();
	//}

	for (_viAlien = _vAlien.begin(); _viAlien != _vAlien.end();) {
		(*_viAlien)->Update();

		if (_rocket->GetMissile()->CheckCollision((*_viAlien)->GetRect())) {
			_viAlien = _vAlien.erase(_viAlien);
		}
		else
			++_viAlien;
	}

	_bullet->Update();
	if (_bullet->CheckCollision(_rocket->GetRect())) {
		_rocket->SetHp(_rocket->GetHp() - 1);
		if (_rocket->GetHp() <= 0)
			_rocket->SetHp(0);
	}
	this->AlienBulletFire();
	this->AlienMove();

}

void EnemyManager::Render()
{
	for (int i = 0; i < _vAlien.size(); i++) {
		_vAlien[i]->Render();
	}
	_bullet->Render();
}

void EnemyManager::SetAlien()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			Enemy* alien;
			alien = new Alien;
			alien->Init("alien", PointMake(
				100 + j * 60, 100 + i * 60));
			_vAlien.push_back(alien);
		}
	}
}

void EnemyManager::AlienBulletFire()
{
	_viAlien = _vAlien.begin();
	for (; _viAlien != _vAlien.end(); ++_viAlien) {
		if ((*_viAlien)->BulletCountFire()) {
			RECT rc = (*_viAlien)->GetRect();
			float angle =
				GetAngle(
					rc.left + (rc.right - rc.left) / 2,
					rc.bottom + 30,
					_rocket->GetImage()->GetX()
					+ _rocket->GetImage()->GetWidth() / 2,
					_rocket->GetImage()->GetY()
					+ _rocket->GetImage()->GetHeight() / 2
				);
			_bullet->Fire(
				rc.left + (rc.right - rc.left) / 2,
				rc.bottom + 30, angle, 5.0f);
		}
	}
}

void EnemyManager::AlienMove()
{
	if (_isLeft && !_isDown) {
		for (int i = 0; i < _vAlien.size(); i++) {
			_vAlien[i]->Move(DIRECTION_LEFT);

			if (_vAlien[i]->CheckCollision(DIRECTION_LEFT)) {
				_isLeft = false;
				_isDown = true;
				_delay = 0;
			}
		}
	}
	else if (!_isLeft && !_isDown) {
		for (int i = 0; i < _vAlien.size(); i++) {
			_vAlien[i]->Move(DIRECTION_RIGHT);

			if (_vAlien[i]->CheckCollision(DIRECTION_RIGHT)) {
				_isLeft = true;
				_isDown = true;
				_delay = 0;
			}
		}
	}

	if (_isDown) {
		for (int i = 0; i < _vAlien.size(); i++) {
			_vAlien[i]->Move(DIRECTION_DOWN);

			_delay++;
			if (_delay % 300 == 0)
				_isDown = false;

			if (_vAlien[i]->CheckCollision(DIRECTION_DOWN)) {
				_isInit = true;
			}
		}
		//_isDown = false;
	}

	if (_isInit) {
		_isInit = false;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				_vAlien[i * 8 + j]->SetPosition(
					PointMake(100 + j * 60, 100 + i * 60));
			}
		}
	}
}
