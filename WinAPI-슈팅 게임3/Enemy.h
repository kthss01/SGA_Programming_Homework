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
	// 살아있는지와 죽어있는지 여부는 상속에서 쓰게 되서 protected로
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

	// 상속을 해서 Alien과 Boss에서 오버라이딩 해야함
	// EnemyManager에서 Enemy Vector로 관리하고 있기 때문 (부모 클래스로)
	
	// 에너미 이동
	virtual void Move(Direction dir);
	// 에너미 그리기
	virtual void Draw();
	// 에너미 애니메이션
	virtual void Animation();
	// 에너미 총알 발사 상태인지 확인
	virtual bool BulletCountFire();
	// 에너미가 충돌했는지 확인
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