#include "stdafx.h"
#include "Alien.h"

Alien::Alien()
{

}

Alien::~Alien()
{

}

HRESULT Alien::Init(const char * imageName, POINT position)
{
	// 에너미에서 먼저 init
	Enemy::Init(imageName, position);
	
	// 추가적으로 Init
	this->_died = IMAGE->FindImage("alien_died");

	_frameCount = 0;
	_currentFrameX2 = 0;

	return S_OK;
}

void Alien::Release()
{
	Enemy::Release();
}

void Alien::Update()
{
	Animation();
}

void Alien::Render()
{
	Draw();
}

void Alien::Draw()
{
	// 살아 있는 상태면 에너미꺼만 그리기 (idle 이미지)
	if (_isLived)
		Enemy::Draw();
	// 살아 있는 상태가 아니면
	// 죽는 이미지 그리기
	else
		_died->FrameRender(GetMemDC(),
			GetRect().left - 25, GetRect().top - 25, _currentFrameX2, 0);
}

void Alien::Animation()
{
	// 살아 있는 상태면 에너미 꺼만 애니메이션 동작 (idle)
	if(_isLived)
		Enemy::Animation();
	// 아니면 죽는 애니메이션 동작
	else {
		_frameCount++;
		if (_frameCount % 5 == 0) {
			_currentFrameX2++;
			if (_currentFrameX2 == _died->GetMaxFrameX())
				_isDied = true;
		}
	}
}
