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

	_bossBullet = new Bullet;
	_bossBullet->Init((char*)"bullet", 300, 1000);

	this->SetAlien();
	//this->SetBoss();

	_isLeft = false;
	_isDown = false;
	_isInit = false;
	_isFirst = true;

	_isBossFirst = true;
	_isBossLeft = false;

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

	// 에너미들 업데이트
	for (_viAlien = _vAlien.begin(); _viAlien != _vAlien.end();) {
		(*_viAlien)->Update();

		// 에너미가 살아 있고 로켓의 미사일에 충돌 시
		if ((*_viAlien)->GetLived() &&
			_rocket->GetMissile()->CheckCollision((*_viAlien)->GetRect())) {
			// 에너미 살아 있는 상태 false로 변경
			(*_viAlien)->SetLived(false);

			SOUND->Play("alien_die", 0.5f);
			SAVEBOOK->AddKill();
		}

		// 에너미가 죽는 경우 
		// 살아 있는 상태가 false고 죽는 애니메이션이 끝나면
		if ((*_viAlien)->GetDied()) {
			// 벡터에서 에너미 지움
			_viAlien = _vAlien.erase(_viAlien);
			// 에너미가 모두 지워지고 보스가 움직임이 처음이면
			if (_vAlien.size() == 0 && _isBossFirst)
				// 보스 설정
				SetBoss();
		}
		else
			++_viAlien;
	}

	// 보스 업데이트
	for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end();) {
		(*_viBoss)->Update();

		// 보스 살아 있고 로켓의 미사일에 충돌 시
		if ((*_viBoss)->GetLived() &&
			_rocket->GetMissile()->CheckCollision((*_viBoss)->GetRect())) {
			// 보스의 살아 있는 상태 false로 변경
			(*_viBoss)->SetLived(false);
			//(*_viBoss)->SetDied(true);
			SOUND->Play("boss_damaged", 0.5f);
		}

		// 보스 죽는 경우
		// 살아 있는 상태 false고 보스 hp 0이면
		if ((*_viBoss)->GetDied()) {
			// 벡터에서 보스 지우기
			_viBoss = _vBoss.erase(_viBoss);
			// 보스 벡터의 크기가 0이면
			if (_vBoss.size() == 0) {
				// 결과씬의 이미지 ending으로 변경
				_resultScene->SetImage(IMAGE->FindImage("ending"));
				// 결과씬으로 이동
				SCENE->ChangeScene("Result");
				if (SOUND->IsPlaySound("bgm"))
					SOUND->Stop("bgm");
				if (SOUND->IsPlaySound("boss_bgm"))
					SOUND->Stop("boss_bgm");
				SOUND->Play("ending", 0.5f);
				SAVEBOOK->AddEnding();
			}
		}
		else
			++_viBoss;
	}

	// 치트키 사용
	if (INPUT->GetKeyDown(VK_TAB)) {
		_cheat = !_cheat;
	}

	// 총알 업데이트
	_bullet->Update();
	// 보스 총알 업데이트
	_bossBullet->Update();
	// 플레이어가 총알의 맞을 때 (치트 안켜져있어야 함)
	if (_bullet->CheckCollision(_rocket->GetRect())) {
		if (!_cheat) {
			// 로켓 hp 감소
			_rocket->SetHp(_rocket->GetHp() - 1);
			if (_rocket->GetHp() <= 0)
				_rocket->SetHp(0);
		}
		SOUND->Play("rocket_damaged", 0.5f);
		SAVEBOOK->AddHit();
	}
	// 플레이어가 보스 총알의 맞을 때 (치트 안켜져있어야 함)
	if (_bossBullet->CheckCollision(_rocket->GetRect())) {
		if (!_cheat) {
			// 로켓 hp 감소
			_rocket->SetHp(_rocket->GetHp() - 1);
			if (_rocket->GetHp() <= 0)
				_rocket->SetHp(0);
		}
		SOUND->Play("rocket_damaged", 0.5f);
		SAVEBOOK->AddHit();
	}

	// 에너미 이동이 처음이 아니면
	// 일정 위치로 이동 할 때까지 총알 발사 안하기 위해서
	if (!_isFirst) {
		// 에너미 총알 발사
		this->AlienBulletFire();
	}
	// 보스 이동이 처음이 아니면
	// 일정 위치로 이동 할 때 까지 총알 발사 안하기 위해서
	if (!_isBossFirst) {
		// 보스 총알 발사
		BossBulletFire();
	}
	// 에너미 이동
	this->AlienMove();
	// 보스 이동
	this->BossMove();

	// 보스 사이즈 0이고 에너미 사이즈 0이면
	// 재시작을 위해서 에너미 매니저 release()
	// 에너미 매니저가 씬이 아니라서 이렇게 관리 필요
	if (_vBoss.size() == 0 && _vAlien.size() == 0)
		this->Release();
}

void EnemyManager::Render()
{
	for (int i = 0; i < _vBoss.size(); i++) {
		_vBoss[i]->Render();
	}

	for (int i = 0; i < _vAlien.size(); i++) {
		_vAlien[i]->Render();
	}

	_bullet->Render();
	_bossBullet->Render();

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
		// 처음 위치 따로 설정
		alien->SetInitPos(PointMake(
			100 + (i % 8) * 60, 100 + (i / 8) * 60));
		_vAlien.push_back(alien);
	}
}

void EnemyManager::AddAlien()
{
	//_viBoss = _vBoss.begin();
	//for (; _viBoss != _vBoss.end(); ++_viBoss) {
	//	Enemy* alien;
	//	alien = new Alien;
	//	alien->Init("alien_idle", 
	//		PointMake((*_viBoss)->GetPos().x, (*_viBoss)->GetPos().y + 50));
	//	alien->SetInitPos(
	//		PointMake((*_viBoss)->GetPos().x, (*_viBoss)->GetPos().y + 50));
	//	_vAlien.push_back(alien);
	//}

	Enemy* alien;
	alien = new Alien;
	alien->Init("alien_idle",
		PointMake(
		(*_viBoss)->GetPos().x + RND->GetFromInto(-150, 150), 
			(*_viBoss)->GetPos().y + 150));
	alien->SetSpeed(alien->GetSpeed() + RND->GetFromInto(-1, 2));
	alien->SetInitPos(
		PointMake(
		(*_viBoss)->GetPos().x + RND->GetFromInto(-150, 150), 
			(*_viBoss)->GetPos().y + 150));
	_vAlien.push_back(alien);
}

void EnemyManager::AlienBulletFire()
{
	_viAlien = _vAlien.begin();
	for (; _viAlien != _vAlien.end(); ++_viAlien) {
		// 에너미가 총알을 발사해야하는 상태이면
		if ((*_viAlien)->BulletCountFire()) {
			RECT rc = (*_viAlien)->GetRect();
			// 로켓과 에너미간의 각도 계산
			float angle =
				GetAngle(
					rc.left + (rc.right - rc.left) / 2,
					rc.bottom + 30,
					_rocket->GetImage()->GetX()
					+ _rocket->GetImage()->GetWidth() / 2,
					_rocket->GetImage()->GetY()
					+ _rocket->GetImage()->GetHeight() / 2
				);
			// 각도로 발사
			_bullet->Fire(
				rc.left + (rc.right - rc.left) / 2,
				rc.bottom + 30, angle, 2.5f);
			SOUND->Play("alien_fire", 0.5f);
		}
	}
}

void EnemyManager::AlienMove()
{
	if (_vAlien.size() == 0) return;

	// 처음 위치까지는 계속 아래로 이동
	// 처음 이동이면
	if (_isFirst) {

		for (int i = 0; i < _vAlien.size(); i++) {
			//if ((_vAlien[i]->GetPos().x != _vInitPos[i].x) ||
			//	(_vAlien[i]->GetPos().y != _vInitPos[i].y))
			
			// 에너미의 초기 위치와 현재 위치와의 거리가 10보다 크면 이동
			if(GetDistance(_vAlien[i]->GetPos().x, _vAlien[i]->GetPos().y,
				_vAlien[i]->GetInitPos().x, _vAlien[i]->GetInitPos().y) > 10)
				_vAlien[i]->Move(DIRECTION_DOWN);
		}

		//if ((_vAlien[0]->GetPos().x != _vInitPos[0].x) &&
		//	(_vAlien[0]->GetPos().y != _vInitPos[0].y)) {
		// 가장 마지막에 내려오는 에너미가 처음 위치에 도착하면 처음 위치로 이동 끝

		// 첫번째의 에너미의 초기 위치와 현재 위치와의 거리가 
		// 10보다 작거나 같으면 처음 이동 멈춤
		// 첫번째 에너미가 가장 마지막으로 내려와서 이렇게 설정
		if (GetDistance(_vAlien[0]->GetPos().x, _vAlien[0]->GetPos().y,
			_vAlien[0]->GetInitPos().x, _vAlien[0]->GetInitPos().y) <= 10) {
			_isFirst = false;
			_isLeft = false;
		}
	}

	// 처음 이동이 아니면
	if (!_isFirst) {
		// 왼쪽 이동이고 아래 이동이 아니면
		if (_isLeft && !_isDown) {
			// 에너미 모두 왼쪽으로 이동
			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_LEFT);

				// 에너미 한명이라도
				// 왼쪽으로 이동시 벽 충돌하면 아래로 이동 시작 
				// 방향은 반대로 변경
				if (_vAlien[i]->CheckCollision(DIRECTION_LEFT)) {
					_isLeft = false;
					_isDown = true;
					_delay = 0;
				}
			}
		}
		// 왼쪽 이동이 아니고 아래 이동이 아니면
		else if (!_isLeft && !_isDown) {
			// 에너미 모두 오른쪽으로 이동
			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_RIGHT);

				// 에너미 한명이라도
				// 오른쪽으로 이동시 벽 충돌하면 아래로 이동 시작
				// 방향 반대로 변경
				if (_vAlien[i]->CheckCollision(DIRECTION_RIGHT)) {
					_isLeft = true;
					_isDown = true;
					_delay = 0;
				}
			}
		}

		// 아래로 이동시
		if (_isDown) {
			// 아래로 이동시간 증가
			_delay++;

			// 에너미 모두 아래로 이동
			for (int i = 0; i < _vAlien.size(); i++) {
				_vAlien[i]->Move(DIRECTION_DOWN);

				// 일정 딜레이 시 아래 이동 금지
				if (_delay % 10 == 0)
					_isDown = false;

				// 에너미 하나라도 아래 벽과 충돌 시
				// 초기 위치로 이동
				if (_vAlien[i]->CheckCollision(DIRECTION_DOWN)) {
					_isInit = true;
				}
			}
			//_isDown = false;
		}
	}

	// 초기 위치로 이동해야하는 경우
	if (_isInit) {
		_isInit = false;
		// 아래로 이동하던 상황 중지
		_isDown = false;

		//for (int i = 0; i < 4; i++) {
		//	for (int j = 0; j < 8; j++) {
		//		_vAlien[i * 8 + j]->SetPosition(
		//			PointMake(100 + j * 60, 100 + i * 60));
		//	}
		//}
		
		// 에너미들 위치 모두 초기 위치로 변경 
		for (int i = 0; i < _vAlien.size(); i++) {
			_vAlien[i]->SetPosition(_vAlien[i]->GetInitPos());
		}
	}
}

void EnemyManager::SetBoss()
{
	Boss* boss;
	boss = new Boss;
	boss->Init("boss_idle", PointMake(
		WINSIZEX / 2, WINSIZEY / 2 - 150 - 500));
	boss->SetInitPos(PointMake(
		WINSIZEX / 2, WINSIZEY / 2 - 150));
	boss->SetEnemyManager(this);
	_vBoss.push_back(boss);
	_isFirst = false;

	if (SOUND->IsPlaySound("bgm"))
		SOUND->Stop("bgm");
	SOUND->Play("boss_bgm", 0.7f);
}

void EnemyManager::BossBulletFire()
{
	_viBoss = _vBoss.begin();
	for (; _viBoss != _vBoss.end(); ++_viBoss) {
		// 보스가 총알을 발사해야 하는 상황이면
		if ((*_viBoss)->BulletCountFire()) {

			RECT rc = (*_viBoss)->GetRect();
			// 보스 중심으로 아래 방향으로 
			// 15도부터 165도까지 15도마다 총 16발의 총알 발사 
			// 부채꼴로 총알 발사
			for (int i = 0; i < 16; i++) {
				_bossBullet->Fire(
					rc.left + (rc.right - rc.left) / 2,
					rc.bottom + 30, (-15 - (10 * i)) * PI/180, 2.0f);
				// boss_fire 별로여서 이걸로
				SOUND->Play("alien_fire", 0.5f);
			}
		}
	}
}

void EnemyManager::BossMove()
{
	// 보스가 없으면 그냥 리턴
	if (_vBoss.size() == 0) return;

	// 처음 위치까지는 계속 아래로 이동
	if (_isBossFirst) {

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
			_isBossFirst = false;
			_isBossLeft = false;
		}
	}

	// 에너미와 같은 방법으로 보스 이동
	if (!_isBossFirst) {
		if (_isBossLeft) {
			for (int i = 0; i < _vBoss.size(); i++) {
				_vBoss[i]->Move(DIRECTION_LEFT);

				if (_vBoss[i]->CheckCollision(DIRECTION_LEFT)) {
					_isBossLeft = false;
				}
			}
		}
		else if (!_isBossLeft) {
			for (int i = 0; i < _vBoss.size(); i++) {
				_vBoss[i]->Move(DIRECTION_RIGHT);

				if (_vBoss[i]->CheckCollision(DIRECTION_RIGHT)) {
					_isBossLeft = true;
				}
			}
		}
	}
}
