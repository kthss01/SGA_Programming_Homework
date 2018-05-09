#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	Matrix matView;
	Matrix matProjection;
	// viewPort 필요없음 
	// 윈도우 쓰겠다고 선언된 순간부터 window 기준으로 되어있음
	Vector2 vEye;

	class Rect* rect;
	class BackGround* bg;
public:
	Program();
	~Program();

	void Update();
	void Render();
};

/*
	과제
	마리오 뛰어다니고 점프하고 하는 거 구현하기
*/