#pragma once

#include "GameNode.h"

struct RectInfo {
	RECT rect;
	float speed;	// rect 속도
};

struct LevelInfo{
	int	count;		// rect 갯수 level 오를수록 갯수가 많아짐
	int size;		// rect 크기 level 오를수록 작아짐
	float speed;	// rect 속도 level 오를수록 속도가 빨라짐
};

enum LEVEL {
	LEVEL_ZERO,		// 기본적인 수의 rect가 떨어짐
	LEVEL_ONE,		// rect 수 증가
	LEVEL_TWO,		// rect 속도 증가
	LEVEL_END		// 최고 레벨, 크기와 속도 증가
};

class MainGame2 : public GameNode
{
private:
	RECT player;
	
	RectInfo rcInfo[100];	// Level에 따라 rect 정보 저장하는 구조체
	LevelInfo lvInfo[4];	// Level 정보 저장하는 구조체

	LEVEL currentLevel;

	bool isLevelUp;
	bool isGameOver;

	int score;

	int timeCount;
	float prevTime;
	float currentTime;
public:
	MainGame2();
	~MainGame2();

	void SetupLevelInfo();			// levelInfo setup
	void SetupRectInfo();			// rectInfo setup level up시도 조정

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

