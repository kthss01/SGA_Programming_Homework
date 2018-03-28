#pragma once

#include "GameNode.h"

struct tagBullet {
	RECT rc;
	float x, y;
	float fireX, fireY;
	float radius;
	float speed;
	float angle;
	bool fire;
	float moveFrame;
};

// common bullet
class Bullet : public GameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	Image* bulletImage;
public:
	Bullet();
	~Bullet();

	HRESULT Init(int bulletMax);
	void Release() override;
	void Update() override;
	void Render() override;

	void Fire(float x, float  y, float destX, float destY);
	void Move();
};

class Missile : public GameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	Image* _bulletImage;

	float _range;
public:
	~Missile() { Release(); }

	HRESULT Init(int bulletMax, float range);
	void Release() override;
	void Update() override;
	void Render() override;

	void Fire(float x, float  y);
	void Move();
	void Bomb();
};