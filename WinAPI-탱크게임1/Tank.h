#pragma once

#include "GameNode.h"
#include "TankMap.h"

enum TANKDIRECTION {
	TANKDIRECTION_LEFT,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_UP,
	TANKDIRECTION_DOWN
};

class Tank : public GameNode
{
private:
	TANKDIRECTION _direction;
	Image * _image;
	RECT _rc;
	float _x, _y;
	float _speed;

	TankMap * _tankMap;
public:
	Tank();
	~Tank();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void TankMove();
	void TankFire();
	void SetTankPosition();
	void SetTankMapMemorLink(TankMap * tm) { _tankMap = tm; }
};

