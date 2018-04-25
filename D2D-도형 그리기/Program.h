#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	// 임시로 벡버퍼 써서 사용할꺼고
	// Direct에서는 자동적으로 초기화할때 백버퍼 자동으로 만들어짐
	HDC hdc;
	HDC memDC;

	HBITMAP hOldBitmap, hBitMap;

	// 물체에 대한 꼭지점, 좌표 정보가 들어가 있는거
	// 도형을 만드는데 기본 단위
	// 순서대로 곱해주면 렌더링 파이프 라인
	vector<Vector2> vecVertex;	// 로컬 좌표

	Matrix matWorld;			// 월드 행렬
	Matrix matView;				// 뷰 행렬
	Matrix matProjection;		// 투영 행렬
	Matrix matViewport;			// 뷰포트 행렬

	Vector2 vEye;				// 내가 촬영하고 싶은 카메라의 위치
	Vector2 vLookAt;			// 내가 바라볼 대상
	// 카메라 뒤집히지 않는 이상 카메라 윗방향 y 1
	Vector2 vUp;				// 카메라의 윗방향

	// Box가 Rect로 표현하기도 함
	Vector2 vBoxPos;			// 박스의 위치값
	float fAngle;				// 회전 시키 위한 각도

	Matrix matChild;
	Matrix matChildTrans;

	char str[128];
	bool isDebug;
public:
	Program();
	~Program();

	void Update();
	void Render();
};

