#include "stdafx.h"
#include "Frame.h"

// 헤더에 함수명 있다고 보면됨
#include <mmsystem.h>
// 라이브러리에 cpp가 가지고 있다고 보면됨
#pragma comment(lib, "winmm.lib")

Frame::Frame()
{
}


Frame::~Frame()
{
}

HRESULT Frame::Init()
{
	// 고성능 타이머 쓸 수 있는지 확인해주는 함수
	// 더블 포인터로 되어 있음
	// 가끔 인자값으로 void * 로 되어 있는것도 있음
	if (QueryPerformanceFrequency(
		(LARGE_INTEGER*)&_periodFrequency)) {

		// 마이크로 세컨드 (1 / 1000000 (백만) ) 초

		_isHardWare = true;
		// GetTickCount랑 같은 녀석이라고 보면됨
		// lastTime에 현재 시간 넣을꺼
		QueryPerformanceCounter(
			(LARGE_INTEGER*)&_lastTime);

		// 초당 시간 계산 범위
		_timeScale = 1.0f / _periodFrequency;
	}
	else {

		// 밀리 세컨트 (1 / 1000) 초 

		_isHardWare = false;

		_lastTime = timeGetTime();
		_timeScale = 1.0f / 1000;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void Frame::Tick(float lockFPS)
{
	if (_isHardWare) {
		QueryPerformanceCounter(
			(LARGE_INTEGER*)&_curTime);
	}
	else {
		_curTime = timeGetTime();
	}

	// curTime 과 lastTime 사이는 한 프레임 (장면) 그리고 난 상황
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f) {
		// 여기서 반복문 돌면서 제한 거는거
		while (_timeElapsed < (1.0f / lockFPS)) {
			if (_isHardWare)
				QueryPerformanceCounter(
				(LARGE_INTEGER*)&_curTime);
			else
				_curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;
	// 한프레임당 경과되는 계산
	_FPSFrameCount++; // 이 녀석이 쌓이고 쌓여서 최종적인 fps 반환
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	// 1초 지나면 최종 프레임 수 초기화
	if (_FPSTimeElapsed > 1.0f) {
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

unsigned long Frame::GetFrameRate(char * str) const
{
	// 문자열 들어오면
	if (str != NULL) {
		// 들어온 문자열에 문자열 포맷으로 반환
		wsprintf(str, "FPS : %d", _frameRate);
	}

	return _frameRate;
}
