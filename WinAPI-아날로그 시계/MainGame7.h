#pragma once

#include "GameNode6.h"

struct tagTimeLineInfo {
	int x;
	int y;
};

enum TIME {
	TIME_SECOND,
	TIME_MINUTE,
	TIME_HOUR,
	TIME_END
};

struct tagTimeNameInfo {
	int time;
	int x;
	int y;
};

class MainGame7 : public GameNode6
{
private:
	RECT clock;

	tagTimeLineInfo timeLineInfo[3];
	tagTimeNameInfo timeNameInfo[12];

	SYSTEMTIME st;
public:
	MainGame7();
	~MainGame7();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

