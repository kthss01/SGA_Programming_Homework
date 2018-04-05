#pragma once

#include "GameNode.h"

class EnemyManager;
class MainGame;

class ResultScene : public GameNode
{
private:
	bool isShow;

	Image * bg;

	EnemyManager * em;
	MainGame* mg;
public:
	ResultScene();
	~ResultScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void SetImage(Image * img) { bg = img; }

	void SetEnemyManager(EnemyManager* em) { this->em = em; }
	void SetMainGame(MainGame* mg) { this->mg = mg; }
};

