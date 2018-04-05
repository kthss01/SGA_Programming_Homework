#pragma once

#include "GameNode.h"
#include "Rocket.h"
#include "EnemyManager.h"

#include "ProgressBar.h"

#define LOADMAX 100

class MainGame : public GameNode
{
private:
	bool isDebug;
	bool isStart;			// 시작 되었는지 확인 하는 변수 에니미 매니저 제어
	bool isLoad;			// 로딩이 시작되었는지 확인 하는 변수

	EnemyManager * _em;		// 에니미 매니저
	Rocket * _rocket;		// 플레이어 로켓

	ProgressBar * _loading;	// 로딩 바를 위한 프로그래스 바

	int loadCount;			// 로딩 바 진행을 위한 변수

	float offsetY;			// 배경화면 루프렌더를 위한 변수
public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
	
	void ReStart();			// 다시 재시작을 위해 변수들 초기화를 위한 함수
};
