#pragma once

#include "GameNode.h"

// 클래스 여러번 include 여러번 하는것도 시간낭비임
// 한번만 include 하게 하는 사람도 있음
// stdafx.h 모두 넣어둠
// 여러번 stdafx.h include 하는 경우가 있는데
// 클래스에 id를 넣어두고 id보고 여러번 호출하는거 막기함

// #ifdef 활용
// 정확히는 #ifndef 이용해서 선언 안되잇으면 선언 하는식으로

#ifdef Test

#endif // Test

// 이건 반대임
// 이걸로 활용
#ifndef Test

#define Test

#endif // !1

class Object : public GameNode
{
private:
	bool bActive;
	Image* _player;

	float deltaTime;
	float _respawnTime;

	int _playerID;
	POINT position;

	int currentFrameX;
private:
	void ObjectEnable();			// 비활성화 -> 활성화
	void ObjectDisable();			// 활성화 -> 비활성화

									// 안에서 돌릴 때 쓸 녀석
	void ObjectUpdate();			// 활성화시 업데이트
									// 리스폰 시간 계산해준다든지
	void ObjectNoActiveUpdate();	// 비활성화시 업데이트
	void ObjectRender();
public:
	Object();
	~Object();

	HRESULT Init() override;
	void Release() override;

	// 외부에서 돌릴때 쓸 녀석
	void Update() override;
	void Render() override;

	void SetActive(bool bActive);
	bool GetActive() { return bActive; }

	void SetImage(Image* image) { _player = image; }
	void SetID(int id) { _playerID = id; }
};

