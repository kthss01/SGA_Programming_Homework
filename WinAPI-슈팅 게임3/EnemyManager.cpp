#include "stdafx.h"
#include "EnemyManager.h"

#include "ResultScene.h"

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
	//this->SetBoss();

	_isLeft = false;
	_isDown = false;
	_isInit = false;
	_isFirst = true;

	_cheat = true;

	return S_OK;
}

void EnemyManager::Release()
{
	for (int i = 0; i < _vAlien.size(); i++) {
		SAFE_DELETE(_vAlien[i]);
	}
	_vAlien.clear();

	for (int i = 0; i < _vBoss.size(); i++) {
		SAFE_DELETE(_vBoss[i]);
	}
	_vBoss.clear();
	//SAFE_DELETE(_bullet);
}

void EnemyManager::Update()
{
	//for (int i = 0; i < _vAlien.size(); i++) {
	//	_vAlien[i]->Update();
	//}

	for (_viAlien = _vAlien.begin(); _viAlien != _vAlien.end();) {
		(*_viAlien)->Update();


		if ((*_viAlien)->GetLived() &&
			_rocket->GetMissile()->CheckCollision((*_viAlien)->GetRect())) {
			(*_viAlien)->SetLived(false);
		}

		if ((*_viAlien)->GetDied()) {
			_viAlien = _vAlien.erase(_viAlien);
			if (_vAlien.size() == 0)
				SetBoss();
		}
		else
			++_viAlien;
	}

	for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end();) {
		(*_viBoss)->Update();

		if ((*_viBoss)->GetLived() &&
			_rocket->GetMissile()->CheckCollision((*_viBoss)->GetRect())) {
			(*_viBoss)->SetLived(false);
			(*_viBoss)->SetDied(true);
		}

		if ((*_viBoss)->GetDied()) {
			_viBoss = _vBoss.erase(_viBoss);
			if (_vBoss.size() == 0) {
				_resultScene->SetImage(IMAGE->FindImage("ending"));
				SCENE->ChangeScene("Result");
			}
		}
		else
			++_viBoss;
	}

	if (_vBoss.size() == 0 && _vAlien.size() == 0)
		this->Release();

	if (INPUT->GetKeyDown(VK_TAB)) {
		_cheat = !_cheat;
	}

	_bullet->Update();
	// 플레이어 총알 피격
	if (_bullet->CheckCollision(_rocket->GetRect())
		&& !_cheat) {
		_rocket->SetHp(_rocket->GetHp() - 1);
		if (_rocket->GetHp() <= 0)
			_rocket->SetHp(0);
	}

	if(!_isFirst)
		this->AlienBulletFire();
	this->AlienMove();
	this->BossMove();

}

void EnemyManager::Render()
{
	for (int i = 0; i < _vAlien.size(); i++) {
		_vAlien[i]->Render();
	}

	for (int i = 0; i < _vBoss.size(); i++) {
		_vBoss[i]->Render();
	}

	_bullet->Render();

	if (_cheat) {
		SetBkMode(GetMemDC(), TRANSPARENT);
		SetTextColor(GetMemDC(), RGB(255, 0, 0));
		TextOut(GetMemDC(), WINSIZEX - 100, WINSIZEY - 50, 
			"On Cheat", strlen("On Cheat"));
	}
}

void EnemyManager::SetAlien()
{
	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		Enemy* alien;
	//		alien = new Alien;
	//		alien->Init("alien", PointMake(
	//			100 + j * 60, 100 + i * 60));
	//		_vAlien.push_back(alien);
	//	}
	//}
	for (int i = 0; i < 32; i++) {
		Enemy* alien;
		alien = new Alien;
		//alien->Init("alien", PointMake(
		//	100 + (i % 8) * 60, 100 + (i / 8) * 60));
		// 처음 위치 보다 더 위에서 시작
		alien->Init("alien_idle", PointMake(
			100 + (i % 8) * 60, 100 + (i / 8) * 60 - 400 - 50 * (31 - i)));
		// 처음 위치 설정
		alien->SetInitPos(PointMake(
			100 + (i % 8) * 60, 100 + (i / 8) * 60));
		_vAlien.push_back(alien);
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
				rc.bottom + 30, angle, 2.5f);
		}
	}
}

void EnemyManager::AlienMove()
{
	if (_vAlien.size() == 0) return;

	// 처음 위치까지는 계속 아래로 이동
	if (_isFirst) {

		for (int i = 0; i < _vAlien.size(); i++) {
			//if ((_vAlien[i]->GetPos().x != _vInitPos[i].x) ||
			//	(_vAlien[i]->GetPos().y != _vInitPos[i].y))
			if(GetDistance(_vAlien[i]->GetPos().x, _vAlien[i]->GetPos().y,
				_vAlien[i]->GetInitPos().x, _vAlien[i]->GetInitPos().y) > 10)
				_vAlien[i]->Move(DIRECTION_DOWN);
		}

		//if ((_vAlien[0]->GetPos().x != _vInitPos[0].x) &&
		//	(_vAlien[0]->GetPos().y != _vInitPos[0].y)) {
		// 가장 마지막에 내려오는 에너미가 처음 위치에 도착하면 처음 위치로 이동 끝
		if (GetDistance(_vAlien[0]->GetPos().x, _vAlien[0]->GetPos().y,
			_vAlien[0]->GetInitPos().x, _vAlien[0]->GetInitPos().y) <= 10) {
			_isFirst = false;
			_isLeft = false;
		}
	}

	if (!_isFirst) {
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
			_delay++;

			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_DOWN);

				if (_delay % 10 == 0)
					_isDown = false;

				if (_vAlien[i]->CheckCollision(DIRECTION_DOWN)) {
					_isInit = true;
				}
			}
			//_isDown = false;
		}
	}

	if (_isInit) {
		_isInit = false;
		_isDown = false;

		//for (int i = 0; i < 4; i++) {
		//	for (int j = 0; j < 8; j++) {
		//		_vAlien[i * 8 + j]->SetPosition(
		//			PointMake(100 + j * 60, 100 + i * 60));
		//	}
		//}
		for (int i = 0; i < _vAlien.size(); i++) {

			_vAlien[i]->SetPosition(
				PointMake(100 + (i % 8) * 60, 100 + (i / 8) * 60));
		}
	}
}

void EnemyManager::SetBoss()
{
	Enemy* boss;
	boss = new Boss;
	boss->Init("boss_idle", PointMake(
		WINSIZEX / 2, WINSIZEY / 2 - 200 - 500));
	boss->SetInitPos(PointMake(
		WINSIZEX / 2, WINSIZEY / 2 - 200));
	_vBoss.push_back(boss);
	_isFirst = true;

}

void EnemyManager::BossBulletFire()
{
}

void EnemyManager::BossMove()
{
	if (_vBoss.size() == 0) return;

	// 처음 위치까지는 계속 아래로 이동
	if (_isFirst) {

		for (int i = 0; i < _vBoss.size(); i++) {
			//if ((_vBoss[i]->GetPos().x != _vInitPos[i].x) ||
			//	(_vBoss[i]->GetPos().y != _vInitPos[i].y))
			if (GetDistance(_vBoss[i]->GetPos().x, _vBoss[i]->GetPos().y,
				_vBoss[i]->GetInitPos().x, _vBoss[i]->GetInitPos().y) > 10)
				_vBoss[i]->Move(DIRECTION_DOWN);
		}

		//if ((_vBoss[0]->GetPos().x != _vInitPos[0].x) &&
		//	(_vBoss[0]->GetPos().y != _vInitPos[0].y)) {
		// 가장 마지막에 내려오는 에너미가 처음 위치에 도착하면 처음 위치로 이동 끝
		if (GetDistance(_vBoss[0]->GetPos().x, _vBoss[0]->GetPos().y,
			_vBoss[0]->GetInitPos().x, _vBoss[0]->GetInitPos().y) <= 10) {
			_isFirst = false;
			_isLeft = false;
		}
	}

	if (!_isFirst) {
		if (_isLeft && !_isDown) {
			for (int i = 0; i < _vBoss.size(); i++) {
				_vBoss[i]->Move(DIRECTION_LEFT);

				if (_vBoss[i]->CheckCollision(DIRECTION_LEFT)) {
					_isLeft = false;
				}
			}
		}
		else if (!_isLeft) {
			for (int i = 0; i < _vBoss.size(); i++) {
				_vBoss[i]->Move(DIRECTION_RIGHT);

				if (_vBoss[i]->CheckCollision(DIRECTION_RIGHT)) {
					_isLeft = true;
				}
			}
		}
	}
}
