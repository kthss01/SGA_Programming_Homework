#include "stdafx.h"
#include "SceneManager.h"
#include "GameNode.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

GameNode* SceneManager::_currentScene = NULL;

void SceneManager::Init()
{
	return;
}

void SceneManager::Release()
{
	map<string, GameNode*>::iterator iter;
	iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();) {
		// 삭제
		if (iter->second != NULL) {
			if (iter->second == _currentScene) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else {
			++iter;
		}
	}
	_mSceneList.clear();
}

void SceneManager::Update()
{
	if (_currentScene) _currentScene->Update();
}

void SceneManager::Render()
{
	// hdc 필요한데 뺄 거
	if (_currentScene) _currentScene->Render();
}

GameNode * SceneManager::AddScene(string sceneName, GameNode * scene)
{
	// 씬이 없는 경우
	if (!scene) return NULL;
	// 동일한 이름 존재하는 경우
	if (_mSceneList.count(sceneName) > 0) return NULL;

	_mSceneList[sceneName] = scene;

	return _mSceneList[sceneName];
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	// 씬 존재 여부 확인
	if (_mSceneList.count(sceneName) < 1) return E_FAIL;
	if (_mSceneList[sceneName] == _currentScene) return E_FAIL;

	// 여기까지 왔으면 씬 변경
	if (SUCCEEDED(_mSceneList[sceneName]->Init())) {
		// 기존 씬이 존재 하면 기존 씬 릴리즈
		if (_currentScene) _currentScene->Release();
		_currentScene = _mSceneList[sceneName];

		return S_OK;
	}

	return E_FAIL;
}
