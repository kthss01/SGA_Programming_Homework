#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
}


Ship::~Ship()
{
}

HRESULT Ship::Init(const char * imageName, int x, int y)
{
	_image = IMAGE->FindImage(imageName);

	_x = x;
	_y = y;

	_angle = 0.0f;
	_rc = RectMakeCenter(
		_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());

	_speed = 0;
	_radius = _image->GetFrameHeight() / 2;

	isDebug = false;

	moveSound.push_back("battle_move1");
	moveSound.push_back("battle_move2");
	moveSound.push_back("battle_move3");
	moveSound.push_back("battle_move4");
	
	soundIndex = 0;

	_destX = _x;
	_destY = _y;
	_newAngle = 0.0f;

	_maxSpeed = 1;

	return S_OK;
}

void Ship::Release()
{
}

void Ship::Update()
{
	KeyControl();
	Move();

	if (INPUT->GetKeyDown(VK_SHIFT))
		isDebug = !isDebug;
}

void Ship::Render()
{
	Draw();
}

void Ship::Draw()
{
	_image->FrameRender(GetMemDC(), _rc.left, _rc.top);

	if (isDebug) {
		char str[128];
		sprintf_s(str, "%d도 %3.2f", int(_angle * 180 / M_PI), _speed);
		TextOut(GetMemDC(), _rc.left, _rc.top - 15, str, strlen(str));
		sprintf_s(str, "new %d도 %3.2f", int(_newAngle * 180 / M_PI), _speed);
		TextOut(GetMemDC(), _rc.left, _rc.top - 30, str, strlen(str));
	}
}

void Ship::KeyControl()
{
	//if (INPUT->GetKey(VK_LEFT)) _angle += 0.04f;
	//if (INPUT->GetKey(VK_RIGHT)) _angle -= 0.04f;
	//if (INPUT->GetKey(VK_UP)) _speed += 0.04f;
	//if (INPUT->GetKey(VK_DOWN)) _speed -= 0.04f;

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		SOUND->Play(moveSound[soundIndex++], 0.5f);
		if (soundIndex == moveSound.size())
			soundIndex = 0;

		_destX = g_ptMouse.x;
		_destY = g_ptMouse.y;
		_newAngle = GetAngle(_x, _y, _destX, _destY);
	}


	// test
	//if (INPUT->GetKeyDown(VK_SPACE)) {
	//	SOUND->Play(moveSound[soundIndex++], 0.5f);
	//	if (soundIndex == moveSound.size())
	//		soundIndex = 0;
	//}
}

void Ship::Move()
{

	int frame;
	float angle;

	if (_angle <= 0) _angle += M_PI * 2;
	if (_angle >= M_PI * 2) _angle -= M_PI * 2;

	if (_newAngle <= 0) _newAngle += M_PI * 2;
	if (_newAngle >= M_PI * 2) _newAngle -= M_PI * 2;


	if (_newAngle > _angle) {
		if (_newAngle - _angle < _angle + M_PI * 2 - _newAngle) {
			if (_newAngle - _angle > 0.04f)
				_angle += 0.08f;
			else
				_angle = _newAngle;
		}
		else {
			if (_angle + M_PI * 2 - _newAngle > 0.04f)
				_angle -= 0.08f;
			else
				_angle = _newAngle;
		}
	}
	if (_newAngle < _angle) {
		if (_angle - _newAngle < _newAngle + M_PI * 2 - _angle) {
			if (_angle - _newAngle > 0.04f)
				_angle -= 0.08f;
			else
				_angle = _newAngle;
		}
		else {
			if (_newAngle + M_PI * 2 - _angle > 0.04f)
				_angle += 0.08f;
			else
				_angle = _newAngle;
		}
	}

	if (_newAngle == _angle) {
		int dist = GetDistance(_x, _y, _destX, _destY);
		if (dist > 5) {
			_speed += 0.04;
			if (_speed > _maxSpeed)
				_speed = _maxSpeed;
			_speed = _maxSpeed;
		}
		else {
			_x = _destX;
			_y = _destY;
			_speed = 0;
		}
	}
	else
		_speed = 0;

	// 각도 (프레임 계산 해줄)
	// 각도는 현재 각도에서 16등분 할꺼
	angle = _angle + (M_PI / 16);

	frame = int(angle / (M_PI / 8));

	if (frame == 16) frame = 0;

	_image->SetFrameX(frame);

	float elapsedTime = FRAME->GetElapsedTime();
	float moveSpeed = elapsedTime * 100;

	_x += cosf(_angle) * _speed * moveSpeed;
	_y -= sinf(_angle) * _speed * moveSpeed;

	_rc = RectMakeCenter(
		_x, _y, _image->GetFrameWidth(), _image->GetFrameHeight());
}
