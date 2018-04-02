#pragma once

#include "GameNode.h"

#include "Bullet.h"

#define POKEMON_MAX 10
#define PADDINGX 250

#define RESPAWNTIME 3.0f

struct tagPokemon {
	Image* img;
	int row, col;
	RECT rc;
	float x, y;
	float radius;
	float angle;
	float speed;

	bool isLive;
	float diedTime;
};

struct tagGun {
	RECT rc;
	float x, y;
	float length;
	float angle;
	int radius;
};

class TestScene : public GameNode
{
private:
	bool isDebug;

	tagPokemon pokemon[POKEMON_MAX];
	tagGun gun;

	Bullet * bullet;

	int count;
	char str[128];
public:
	TestScene();
	~TestScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void Move();
};

