#include "stdafx.h"
#include "ProgressBar.h"


ProgressBar::ProgressBar()
{
}


ProgressBar::~ProgressBar()
{
}

HRESULT ProgressBar::Init(char * frontImage, char * backImage, float x, float y, float width, float height)
{
	_x = x;
	_y = y;

	_rcProgressBar = RectMake(x, y, width, height);

	_progressbarFront = new Image;
	_progressbarFront->Init(frontImage,
		x, y, width, height, true, RGB(255, 0, 255));

	_progressbarBack = new Image;
	_progressbarBack->Init(backImage,
		x, y, width, height, true, RGB(255, 0, 255));

	_width = _progressbarFront->GetWidth();

	return S_OK;
}

void ProgressBar::Release()
{
	SAFE_DELETE(_progressbarFront);
	SAFE_DELETE(_progressbarBack);
}

void ProgressBar::Update()
{
	_rcProgressBar = RectMake(
		_x, _y,
		_progressbarBack->GetWidth(),
		_progressbarBack->GetHeight());

}

void ProgressBar::Render()
{
	_progressbarBack->Render(GetMemDC(),
		_rcProgressBar.left, _rcProgressBar.top);

	_progressbarFront->Render(GetMemDC(),
		_rcProgressBar.left, _rcProgressBar.top, 0, 0,
		_width, _progressbarFront->GetHeight());

}

void ProgressBar::SetGauge(float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge)
		* _progressbarBack->GetWidth();
}
