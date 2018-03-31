#include "stdafx.h"
#include "Boss.h"


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

	_currentFrameXs[BOSS_HATCH] = 8;

	_images[BOSS_HATCH] = IMAGE->FindImage("boss_hatch");
	_images[BOSS_ENEMY] = IMAGE->FindImage("boss_enemy");
	_images[BOSS_DEPLOY] = IMAGE->FindImage("boss_deploy");
	_images[BOSS_FIRE] = IMAGE->FindImage("boss_fire");

	_isHatch = true;
	_isHatchClose = false;
	_isDeploy = false;
	_isFire = false;
	_isDeployClose = false;
	_isEnemy = false;

	currentParts = BOSS_ENEMY;

	return S_OK;
}

void Boss::Release()
{
}

void Boss::Update()
{
	Animation();

	if (INPUT->GetKeyDown('T'))
		_isHatch = true;

	if (INPUT->GetKeyDown('C')) {
		if (currentParts == BOSS_ENEMY)
			currentParts = BOSS_DEPLOY;
		else if (currentParts == BOSS_DEPLOY)
			currentParts = BOSS_ENEMY;
	}
}

void Boss::Render()
{
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
		// image º¸Á¤
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

	if (_frameCount % 5 == 0) {
		if (_currentFrameXs[BOSS_HATCH] < _images[BOSS_HATCH]->GetMaxFrameX() - 2
			&& !_isHatchClose && _isHatch)
			_currentFrameXs[BOSS_HATCH]++;

		if (_currentFrameXs[BOSS_HATCH] == _images[BOSS_HATCH]->GetMaxFrameX() - 2
			&& !_isHatchClose) {
			if(currentParts == BOSS_DEPLOY)
				_isDeploy = true;
			else if(currentParts == BOSS_ENEMY)
				_isEnemy = true;
		}
		
		if (_isHatchClose 
			&& _currentFrameXs[BOSS_HATCH] > 8) {
			_currentFrameXs[BOSS_HATCH]--;
		}

		if (_isHatchClose &&
			_currentFrameXs[BOSS_HATCH] == 8) {
			_isHatchClose = false;
			_isHatch = false;
		}
	}


	if (_frameCount % 5 == 0 && _isDeploy) {
		//_currentFrameXs[BOSS_DEPLOY]++;

		if (_currentFrameXs[BOSS_DEPLOY] < _images[BOSS_DEPLOY]->GetMaxFrameX()
			&& !_isDeployClose) {
			_currentFrameXs[BOSS_DEPLOY]++;
		}

		if (_isDeployClose) {
			_currentFrameXs[BOSS_DEPLOY]--;
		}

		if (_currentFrameXs[BOSS_DEPLOY] == _images[BOSS_DEPLOY]->GetMaxFrameX()) {
			_isFire = true;
		}

		if (_isDeployClose &&
			_currentFrameXs[BOSS_DEPLOY] == 0) {
			_isDeployClose = false;
			_isDeploy = false;
			_isHatchClose = true;
		}
	}

	if (_frameCount % 5 == 0 && _isFire) {
		if (_currentFrameXs[BOSS_FIRE] < _images[BOSS_FIRE]->GetMaxFrameX())
			_currentFrameXs[BOSS_FIRE]++;

		if (_currentFrameXs[BOSS_FIRE] == _images[BOSS_FIRE]->GetMaxFrameX()) {
			_isFire = false;
			_isDeployClose = true;
			_currentFrameXs[BOSS_FIRE] = 0;
		}
	}

	if (_frameCount % 7 == 0 && _isEnemy) {
		if (_currentFrameXs[BOSS_ENEMY] < _images[BOSS_ENEMY]->GetMaxFrameX())
			_currentFrameXs[BOSS_ENEMY]++;

		if (_currentFrameXs[BOSS_ENEMY] == _images[BOSS_ENEMY]->GetMaxFrameX()) {
			_isEnemy = false;
			_isHatchClose = true;
			_currentFrameXs[BOSS_ENEMY] = 0;
		}
	}
}
