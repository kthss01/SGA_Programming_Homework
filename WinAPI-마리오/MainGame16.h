#pragma once

#include "GameNode.h"

/*
	ani 정리
	row 1
	RightWalk 0, 1
	RightJumpUp 2
	RightJumpDown 3
	LeftWalk 4 5
	LeftJumpUp 6
	LeftJumpDown 7

	row 2
	RightRunChange 8
	RightRun 9 10 11
	LeftRunChange 12
	LeftRun 13 14 15

	row 3 필요 x

	row 4
	RightIdle 24
	RightVictory 25
	RightForward 26
	Backward 27
	LeftIdle 28
	LeftVictory 29
	LeftForWard 30
*/

enum MarioState {
	STATE_LEFTIDLE = 28,
	STATE_RIGHTIDLE = 24,
	STATE_LEFTWALK = 4,
	STATE_RIGHTWALK = 0,
	STATE_LEFTRUN = 13,
	STATE_RIGHTRUN = 9,
	STATE_LEFTJUMPUP = 6,
	STATE_LEFTJUMPDOWN,
	STATE_RIGHTJUMPUP = 2,
	STATE_RIGHTJUMPDOWN,
	STATE_LEFTVICTORY = 29,
	STATE_RIGHTVICTORY = 25,
	STATE_RIGHTRUNCHANGE = 12,
	STATE_LEFTRUNCHANGE = 8
};

struct tagAnimationInfo {
	float offsetX, offsetY;
	float width, height;
};

struct tagMarioInfo {
	float x, y;
	float offsetX, offsetY;
	float width, height;
	float speed;
	MarioState state;
	int stateCount;
	bool isRun;
};

class MainGame16 : public GameNode
{
private:
	Image * mario;
	Image * bg;
	Image * tile;

	tagAnimationInfo ani[32];
	tagMarioInfo marioInfo;

	RECT rc;

	float gravity;
	float currentTime;
	float prevTime;
public:
	MainGame16();
	~MainGame16();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void CheckAniForState();
};

