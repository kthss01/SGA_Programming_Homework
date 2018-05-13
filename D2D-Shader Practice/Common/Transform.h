#pragma once

#define RESET_POSITION 1
#define RESET_ROTATION 2
#define RESET_SCALE 4

class Transform
{
protected:
	// 같은 주소로 묶이기 때문에 (묶음끼리 주소값 공유) 
	// axis[0]이나 right나 같은 값을 호출하게됨
	// axis[1]이나 up이나 같은 값을 호출하게됨
	union {
		struct {
			Vector2 axis[2];
		};
		struct {
			Vector2 right;
			Vector2 up;
		};
	};

	// 위치값
	Vector2 position;
	// 크기값
	Vector2 scale;

	// 최종행렬 (월드 행렬)
	Matrix matFinal;

	// 회전이나 변경이 있을 때 자동으로 Update해줄지 말지 
	bool bAutoUpdate;

	// 로컬 행렬
	Matrix matLocal;

	Transform* pParent;	// 존재하면 로컬 행렬로 바뀜

	Transform* pFirstChild;	// 첫번째 자식

	Transform* pNextSibling; // 같은 부모를 가지고 있는 형제


public:
	Transform();
	~Transform();

	// Parents
	// 내 자식으로 Transform 붙이는거
	void AddChild(Transform* pChild);

	// 부모 설정해주는 녀석
	void AttachTo(Transform* pParent);

	// 부모 자식 관계 해제 시켜주는 녀석
	void ReleaseParent();

	// Transform 정보 초기화 -1이면 전체 초기화
	// flag 옵션이란 의미도 있음
	void Reset(int resetFlag = -1);

	// 월드 좌표 세팅
	void SetWorldPosition(Vector2 position);
	// 부모 있을 경우 로컬 좌표로 세팅, 부모 없을 시 월드 좌표로 변환
	void SetLocalPosition(Vector2 position);
	// 방향대로 이동하는거, 자기 자신 축을 기준으로 움직이는거
	void MovePositionSelf(Vector2 delta);
	void MovePositionWorld(Vector2 delta);
	void MovePositionLocal(Vector2 delta);

	// scale
	// 크기가 고정 대입이라고 보면됨
	void SetScale(Vector2 scale);
	// 내가 가지고 있는 스케일값에 + 더해주는거
	void SetScaling(Vector2 deltaScale);

	// rotate
	void RotateWorld(Vector2 angle);
	// 자기 자신 축으로 회전
	void RotateSelf(float angle);
	// 부모없을 경우 월드 기준 부모 있으면 부모 기준
	void RotateLocal(Vector2 angle);

	// 월드 행렬을 넣어서 회전이 될 녀석
	void SetRotateWorld(const Matrix matWorldRotate);
	void SetRotateLocal(const Matrix matLocalRotate);

	// Rotation (사원수 회전 == QUATERNION)
	
	// etc...
	// 특정 방향을 바라보게끔 회전
	void LookPosition(Vector2 pos, Vector2 up = Vector2(0, 0, -1));

	void DefaultControl2();

	// Update()
	void UpdateTransform();

	// Get 
	Matrix GetFinalMatrix() const;	// 월드 세 팅할때만 쓸 예정

	Vector2 GetWorldPosition();

	// axis 전부다 받아오는거
	void GetUnitAxis(Vector2* pVecArr) const;
	// 지정한 번호의 axis만 받아오는거
	Vector2 GetUnitAxis(int axisNum) const;

	void DrawInterface();
};

