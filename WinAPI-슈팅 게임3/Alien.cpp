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
	Enemy::Init(imageName, position);
	
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
	if (_isLived)
		Enemy::Draw();
	else
		_died->FrameRender(GetMemDC(),
			GetRect().left - 25, GetRect().top - 25, _currentFrameX2, 0);
}

void Alien::Animation()
{
	if(_isLived)
		Enemy::Animation();
	else {
		_frameCount++;
		if (_frameCount % 5 == 0) {
			_currentFrameX2++;
			if (_currentFrameX2 == _died->GetMaxFrameX())
				_isDied = true;
		}
	}
}
