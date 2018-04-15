#pragma once
#include "SingletonBase.h"

// 전방 선언한 이유
// 원래 헤더에 include 하는게 좋은 방법이 아님
// 웬만해서는 헤더에 include 안하는게 좋음
/*
GameNode에서 이미지 include 하고 이미지에서 게임노드 include 하면
왓다갓다 하게 되서 터짐
보통은 cpp에 선언 후 헤더에 전방 선언하는 방법을 씀
*/
class GameNode;
class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string, GameNode*> mSceneList;
private:
	static GameNode* _currentScene; // 현재 플레이 씬
	mSceneList _mSceneList; // 씬 목록
public:
	SceneManager();
	~SceneManager();

	void Init();
	void Release();
	void Update();
	void Render();

	GameNode* AddScene(string sceneName, GameNode* scene);
	HRESULT ChangeScene(string sceneName);
};

#define SCENE SceneManager::GetSingleton()

/*
i++ -> i 실행 후 + 1
++i -> +1 후 실행
*/