#pragma once

#include "GameNode.h"

class Ship : public GameNode
{
protected:
	Image * _image;
	RECT _rc;
	float _x, _y;
	float _angle;
	float _speed;
	float _radius;	// 이미지의 반지름 이미지 외부에 원을 씌울꺼

	float _newAngle;
	float _destX, _destY;
	float _maxSpeed;

	vector<string> moveSound;
	int soundIndex;

	bool isDebug;
public:
	Ship();
	~Ship();

	virtual HRESULT Init(
		const char * imageName, int  x, int y);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Draw();
	// 키값 캐릭터 움직이게 설정해주고
	// 몬스터는 오버라이딩 해서 다시 작성해주면 됨
	virtual void KeyControl();
	virtual void Move();
};

