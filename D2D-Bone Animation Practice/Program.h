#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Camera* mainCamera;
	class Rect* rect;
	class BackGround* bg;
public:
	Program();
	~Program();

	void Update();
	void Render();
};

/*
	����
	������ �پ�ٴϰ� �����ϰ� �ϴ� �� �����ϱ�
*/