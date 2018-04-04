#pragma once

#include "GameNode.h"

struct tagBullet {
	Image* bulletImage;
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

	char * _imageName;
	float _range;
	int _bulletMax;
public:
	Bullet();
	~Bullet();

	HRESULT Init(char* imageName, int bulletMax, float range);
	void Release() override;
	void Update() override;
	void Render() override;

	void Fire(float x, float  y, float angle, float speed);
	void Move();
	void Animation();

	bool CheckCollision(RECT rc);
};

class Missile : public GameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
public:
	~Missile() { Release(); }

	HRESULT Init(int bulletMax, float range);
	void Release() override;
	void Update() override;
	void Render() override;

	void Fire(float x, float  y);
	void Move();
	void Animation();
	void Bomb();

	bool CheckCollision(RECT rc);
};