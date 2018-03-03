#pragma once

#include "GameNode.h"

/*
	록맨 스프라이트 정리
	12 x 6
	왼쪽 걷기 11개
	오른쪽 걷기 11개
	왼쪽 평시 6개 왼쪽벽잡기 3개
	오른쪽 평시 6개 오른쪽벽잡기 3개
	왼쪽점프 12개
	오른쪽 점프 12개
*/

enum RockmanState {
	STATE_LEFTIDLE,
	STATE_RIGHTIDLE,
	STATE_LEFTJUMP,
	STATE_RIGHTJUMP,
	STATE_LEFTWALLCATCH,
	STATE_LEFTWALK,
	STATE_RIGHTWALLCATCH,
	STATE_RIGHTWALK
};

struct tagRockmanInfo {
	RockmanState state;
	bool isGround;
};

class MainGame17 : public GameNode
{
private:
	Image * rockman;
	Image * bg;

	tagRockmanInfo rockmanInfo;

	RECT ground[3];

	float gravity;

	bool checkRect;
	bool checkStatus;

	DWORD prevTime;
	DWORD curTime;
public:
	MainGame17();
	~MainGame17();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

