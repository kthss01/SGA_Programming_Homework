#pragma once

#include "GameNode.h"

// 에너미 부모 클래스

#define FIRECOUNT 300

enum Direction {
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
};

class Enemy : public GameNode
{
private:
	Image * _image;
	RECT _rc;
	POINT _pos;
	float _speed;

	// 프레임 이미지를 돌리기 위한 변수
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	// 계속해서 주기적으로 증가
	int _fireCount;
	// 랜덤한 값을 가지고 있다가 fireCount가 쌓이게 되면 발사하는 형식으로
	int _rndFireCount;

	POINT _initPos;
protected:
	bool _isLived;
	bool _isDied;
public:
	Enemy();
	~Enemy();

	virtual HRESULT Init();
	virtual HRESULT Init(const char* imageName, POINT position);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Move(Direction dir);
	virtual void Draw();
	virtual void Animation();

	virtual bool BulletCountFire();

	bool CheckCollision(Direction dir);

	void SetPosition(POINT pos);

	RECT GetRect() { return _rc; }
	POINT GetPos() { return _pos; }

	void SetInitPos(POINT pos) { _initPos = pos; }
	POINT GetInitPos() { return _initPos; }

	void SetLived(bool isLived) { _isLived = isLived; }
	bool GetLived() { return _isLived; }

	void SetDied(bool isDied) { _isDied = isDied; }
	bool GetDied() { return _isDied; }

	void SetSpeed(float speed) { _speed = speed; }
	float GetSpeed() { return _speed; }
};