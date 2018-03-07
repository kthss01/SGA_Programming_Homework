#pragma once

#include "GameNode.h"
#include <vector>

using namespace std;

#define SPEED 3
#define GRAVITY 0.25f
#define JUMP -12

enum STATUS {
	STATUS_WALK,
	STATUS_IDLE = 4,
	STATUS_JUMPUP,
	STATUS_JUMPDOWN
};

struct tagMonsterInfo {
	float x, y;
	float speed;
	int monsterNumber;
};

class MainGame19 : public GameNode
{
private:
	Image * bg;
	Image * arthur;
	Image * monster[3];

	vector<tagMonsterInfo> monsterInfo;

	STATUS status;
	float moveFrame;
	float vy;

	// armor 0 unArmor 3 
	int vector;	

	int loopX;

	int oldScore;
	int score;

	char str[128];
	RECT box;
	RECT temp;

	bool isDebug;
	bool isNextLevel;
	bool isOver;
	bool isStart;

	DWORD prevTime;
	DWORD curTime;
public:
	MainGame19();
	~MainGame19();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void Jump();
	void AddMonster(int number);
};

