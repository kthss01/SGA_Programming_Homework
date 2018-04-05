#include "stdafx.h"
#include "Boss.h"

#include "EnemyManager.h"

Boss::Boss()
{
}


Boss::~Boss()
{
}

HRESULT Boss::Init(const char * imageName, POINT position)
{
	Enemy::Init(imageName, position);

	_frameCount = 0;

	for (int i = 0; i < BOSS_END; i++) {
		_currentFrameXs[i] = 0;
	}

	// 해치 열고 닫는 애니메이션은 시작을 8로 설정
	// 이유 : 이미지를 잘 못 가공해서
	_currentFrameXs[BOSS_HATCH] = 8;

	_images[BOSS_HATCH] = IMAGE->FindImage("boss_hatch");
	_images[BOSS_ENEMY] = IMAGE->FindImage("boss_enemy");
	_images[BOSS_DEPLOY] = IMAGE->FindImage("boss_deploy");
	_images[BOSS_FIRE] = IMAGE->FindImage("boss_fire");

	_isHatch = false;
	_isHatchClose = false;
	_isDeploy = false;
	_isFire = false;
	_isDeployClose = false;
	_isEnemy = false;

	// 에너미 소환부터 하도록 설정
	//currentParts = BOSS_DEPLOY;
	currentParts = BOSS_ENEMY;

	_hpBar = new ProgressBar;
	_hp = _maxHp = 1000;
	_hpBar->Init(
		IMAGE->AddImage(
			"hp_front", "images/bar_front.bmp", 0, 0, 700, 50, true, RGB(255, 0, 255)),
		IMAGE->AddImage(
			"hp_back", "images/bar_back.bmp", 0, 0, 700, 50, true, RGB(255, 0, 255)),
		WINSIZEX / 2 - 350, 25, 700, 50);

	_count = 0;
	_fireCount = 0;

	return S_OK;
}

void Boss::Release()
{
	Enemy::Release();
}

void Boss::Update()
{
	Animation();

	// 테스트 용
	//if (INPUT->GetKeyDown('T'))
	//	_isHatch = true;
	//
	//if (INPUT->GetKeyDown('C')) {
	//	if (currentParts == BOSS_ENEMY)
	//		currentParts = BOSS_DEPLOY;
	//	else if (currentParts == BOSS_DEPLOY)
	//		currentParts = BOSS_ENEMY;
	//}

	ChoicePattern();

	CheckDamaged();

	_hpBar->SetGauge(_hp, _maxHp);
	_hpBar->Update();
}

void Boss::Render()
{
	_hpBar->Render();
	Draw();
}

void Boss::Move(Direction dir)
{
	Enemy::Move(dir);
}

void Boss::Draw()
{
	Enemy::Draw();
	
	if (true) {
		_images[BOSS_HATCH]->FrameRender(GetMemDC(),
			GetRect().left + 96,
			GetRect().bottom - 14, _currentFrameXs[BOSS_HATCH], 0);
	}

	if (_isDeploy) {
		_images[BOSS_DEPLOY]->FrameRender(GetMemDC(),
			GetRect().left + 135,
			GetRect().bottom - 10 - 15 + _currentFrameXs[BOSS_DEPLOY],
			_currentFrameXs[BOSS_DEPLOY], 0);
	}

	if (_isEnemy) {
		int temp = 0;
		if (_currentFrameXs[BOSS_ENEMY] < 6)
			temp = 15;

		_images[BOSS_ENEMY]->FrameRender(GetMemDC(),
			GetRect().left + 117,
			GetRect().bottom - 10 - temp,
			_currentFrameXs[BOSS_ENEMY], 0);
	}

	if (_isFire) {
		int temp = 0;
		// image 보정
		switch (_currentFrameXs[BOSS_FIRE]) {
		case 0:
			temp = -10;
			break;
		case 1:
			temp = -10;
			break;
		case 2:
			temp = -7;
			break;
		case 3:
			temp = -6;
			break;
		case 4:
			temp = -6;
			break;
		case 5:
			temp = -4;
			break;
		case 6:
			temp = -4;
			break;
		case 7:
			temp = -4;
			break;
		case 8:
			temp = -2;
			break;
		case 9:
			temp = -2;
			break;
		case 10:
			temp = -2;
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			temp = 2;
			break;
		case 14:
			temp = 2;
			break;
		case 15:
			temp = 2;
			break;
		case 16:
			break;
		case 17:
			break;
		}
		_images[BOSS_FIRE]->FrameRender(GetMemDC(),
			GetRect().left + 25,
			GetRect().bottom - 16 + temp,
			_currentFrameXs[BOSS_FIRE], 0);
	}
}

void Boss::Animation()
{
	Enemy::Animation();

	_frameCount++;

	// 해치를 열고 닫는 애니메이션
	if (_frameCount % 5 == 0) {
		// 마지막 애니메이션이 문제가 있어서 마지막 애니메이션 - 2 까지만
		// 여는 애니메이션
		if (_currentFrameXs[BOSS_HATCH] < _images[BOSS_HATCH]->GetMaxFrameX() - 2
			&& !_isHatchClose && _isHatch)
			_currentFrameXs[BOSS_HATCH]++;

		// 마지막 애니메이션 - 2인 경우 
		// 패턴 상태의 따라 총알 또는 에너미 소환 애니메이션 시작
		if (_currentFrameXs[BOSS_HATCH] == _images[BOSS_HATCH]->GetMaxFrameX() - 2
			&& !_isHatchClose) {
			// 총 쏘는 애니메이션인 경우
			if (currentParts == BOSS_DEPLOY) {
				_isDeploy = true;
			}
			// 애너미 소환하는 애니메이션인 경우
			else if(currentParts == BOSS_ENEMY)
				_isEnemy = true;
		}
		
		// 닫는 애니메이션인 경우
		if (_isHatchClose 
			&& _currentFrameXs[BOSS_HATCH] > 8) {
			_currentFrameXs[BOSS_HATCH]--;
		}

		// 닫는 애니메이션의 처음 애니메이션으로 돌아오는 경우
		// 애니메이션 제어 초기화 후 보스 패턴 다음 패턴으로 변경
		if (_isHatchClose &&
			_currentFrameXs[BOSS_HATCH] == 8) {
			_isHatchClose = false;
			_isHatch = false;

			if (currentParts == BOSS_ENEMY)
				currentParts = BOSS_DEPLOY;
			else if (currentParts == BOSS_DEPLOY)
				currentParts = BOSS_ENEMY;
		}
	}

	// 총 꺼내고 닫는 애니메이션
	if (_frameCount % 5 == 0 && _isDeploy) {
		//_currentFrameXs[BOSS_DEPLOY]++;

		// 총 여는 애니메이션
		// 총 닫는 상태가 아닐때만 하도록 진행
		if (_currentFrameXs[BOSS_DEPLOY] < _images[BOSS_DEPLOY]->GetMaxFrameX()
			&& !_isDeployClose) {
			_currentFrameXs[BOSS_DEPLOY]++;
		}

		// 총 닫는 상태인 경우 닫는 애니메이션
		if (_isDeployClose) {
			_currentFrameXs[BOSS_DEPLOY]--;
		}

		// 총 여는 애니메이션이 마지막 애니매이션인 경우
		// 총 쏘는 애니메이션 시작
		if (_currentFrameXs[BOSS_DEPLOY] == _images[BOSS_DEPLOY]->GetMaxFrameX()) {
			_isFire = true;
		}

		// 총 닫는 상태이고 처음 애니메이션인 경우
		// 해치 닫는 상태 시작
		if (_isDeployClose &&
			_currentFrameXs[BOSS_DEPLOY] == 0) {
			_isDeployClose = false;
			_isDeploy = false;
			_isHatchClose = true;
		}
	}

	// 총 쏘는 애니메이션 
	if (_frameCount % 5 == 0 && _isFire) {
		// 마지막 애니메이션이 아닌 경우 총 쏘는 애니메이션 진행
		if (_currentFrameXs[BOSS_FIRE] < _images[BOSS_FIRE]->GetMaxFrameX())
			_currentFrameXs[BOSS_FIRE]++;

		// 마지막 애니메이션 인 경우
		// 총 닫는 상태 시작 하고 총 쏘는 애니메이션 초기화
		if (_currentFrameXs[BOSS_FIRE] == _images[BOSS_FIRE]->GetMaxFrameX()) {
			_isFire = false;
			_isDeployClose = true;
			_currentFrameXs[BOSS_FIRE] = 0;
		}
	}

	// 에너미 소환하는 애니메이션
	if (_frameCount % 7 == 0 && _isEnemy) {
		// 마지막 애니메이션이 아닌 경우 에너미 소환하는 애니메이션 진행
		if (_currentFrameXs[BOSS_ENEMY] < _images[BOSS_ENEMY]->GetMaxFrameX())
			_currentFrameXs[BOSS_ENEMY]++;

		// 마지막 애니메이션인 경우
		// 해치 닫는 상태 시작하고 에너미 쏘는 애니메이션 초기화
		if (_currentFrameXs[BOSS_ENEMY] == _images[BOSS_ENEMY]->GetMaxFrameX()) {
			_isEnemy = false;
			_isHatchClose = true;
			_currentFrameXs[BOSS_ENEMY] = 0;
			
			// 에너미 매니저에서 에너미 추가
			_em->AddAlien();
		}
	}
}

void Boss::CheckDamaged()
{
	// 보스가 살아있는 상태가 아니면
	if (!GetLived()) {
		// hp 감소
		_hp -= 10;
		// 다시 살아있는 상태로 변경
		SetLived(true);
		// hp가 0보다 작아지면 죽어있는 상태로 변경
		if (_hp <= 0) {
			_hp = 0;
			SetDied(true);
		}
	}
}

void Boss::ChoicePattern()
{
	_count++;

	// 딜레이마다 해치 여는 상태로 변경
	if (_count % 400 == 0) {
		_isHatch = true;
	}
}

bool Boss::BulletCountFire()
{
	_fireCount++;
	// 총쏘는 상태이고 일정 총 발사 카운트인 경우 
	// true 반환
	if (_fireCount % 10 == 0 &&
		_isFire) {
		_fireCount = 0;
		return true;
	}

	return false;
}
