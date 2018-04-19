#include "stdafx.h"
#include "TimeManager.h"

#define Test 

TimeManager::TimeManager()
{
}


TimeManager::~TimeManager()
{
}

HRESULT TimeManager::Init()
{
	_frame = new Frame;
	_frame->Init();

	return S_OK;
}

void TimeManager::Release()
{
	if (_frame != NULL)
		SAFE_DELETE(_frame);
}

void TimeManager::Update(float lock)
{
	if (_frame != NULL) {
		_frame->Tick(lock);
	}
}

void TimeManager::Render(HDC hdc)
{
	char str[256];

	string strFrame;
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 0, 0));

	// 우리가 사용하는 if문이랑 동일 하다고 보면 됨
	// _DEBUG 비주얼에서 미리 만든 조건문이라고 보면됨
	// 개발 상태가 Debug라고 보면됨
#ifdef _DEBUG
	if (_frame != NULL) {
		sprintf_s(str, "FPS : %d", _frame->GetFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));

		sprintf_s(str, "worldTime : %f", _frame->GetWorldTime());
		TextOut(hdc, 0, 20, str, strlen(str));

		sprintf_s(str, "ElapsedTime : %f", _frame->GetElapsedTime());
		TextOut(hdc, 0, 40, str, strlen(str));
	}
#else
	if (_frame != NULL) {
		sprintf_s(str, "FPS : %d", _frame->GetFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif

	// 이거 많이 사용하는게 
	// 모바일에서 VIP 등급제 같은데서 많이 씀
	//#ifdef Test
	//	cout << Test << endl;
	//#endif // Test

#ifdef vip0
	cout << 노과금 << endl;
#else
	//cout << 과금 << endl;
#endif // vip0

}
