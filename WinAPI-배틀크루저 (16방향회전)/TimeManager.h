#pragma once

#include "SIngletonBase.h"
#include "Frame.h"

class TimeManager : public SingletonBase<TimeManager>
{
private:
	Frame * _frame;
public:
	TimeManager();
	~TimeManager();

	HRESULT Init();
	void Release();
	void Update(float lock = 0.0f);
	// GameNode 상속 받지 않고 있어서 hdc 필요함
	// 만약 GetMemDC() 함수 상속 받고 싶으면 GameNode 다중 상속받으면 되나 
	// 다중 상속 안쓰는게 좋음
	void Render(HDC hdc);

	float GetElapsedTime() const { return _frame->GetElapsedTime(); }
	float GetWorldTime() const { return _frame->GetWorldTime(); }
};

#define FRAME TimeManager::GetSingleton()

