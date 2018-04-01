#pragma once

#include "GameNode.h"

// 큐, 우선순위 큐
/*
	자료 구조
	먼저 넣은게 먼저 나오는 자료 구조 (FIFO - First In First Out)
	줄을 세우면 맨 앞부터 시작하는 거 같은 구조라고 생각하면 됨

	*여기서 사용한 priority queue(우선 순위 큐)는 힙 구조로 되어 있는 큐로
	maxHeap으로 기본 설정 되어 있어 
	넣은 것 중 가장 큰 값이 먼저 나오게 됨
*/
#include <queue>

// 스택
/*
	자료구조
	먼저 넣은게 마지막으로 나오는 자료 구조 (LIFO - List In First Out)
	쟁반 쌓아 올리면 위에꺼부터 꺼내는 것 같은 구조라고 생각하면 됨
*/
#include <stack>

using namespace std;

#define TILEROW 12	// 타일의 행
#define TILECOL 12  // 타일의 열
#define TILEMAX TILEROW * TILECOL // 타일 최대 크기 행 * 열

// a* 알고리즘에서 사용될 각 타일 가중치 값
struct tagCost {
	// H 비용은 맨하탄 방식(Manhattan Method)로 구함
	// 현재 타일에서 목표 타일에 도달하기 까지의 대각선 이동을 제외한 
	// 가로, 세로로 이동한 총 숫자를 계산해서 구함
	int h;	// 얻어진 타일로부터 최종 목적 지점까지의 예상 이동 비용

	// 세로 또는 가로 이동 10, 대가선 14로 계산
	// 대각선의 길이는 이등변삼각형의 길이비 루트2:1 해서 14 나온거
	int g;  // 시작점으로부터 새로운 타일까지의 이동 비용
};

// 타일 정보
struct tagTile {
	RECT rc;				// rect 이걸로 그냥 사각형 그림
	float x, y;				// 중점 x, y
	float width, height;	// 너비, 높이
	int row, col;			// 행, 열
	bool block;				// 막힌 타일인지 체크하는 변수

	bool dest;				// 목표 타일인지 체크하는 변수
	bool close;				// 닫혀있는 타일인지 체크하는 변수 (a* 에서 사용)
	tagCost f;				// 타일의 가중치 값 (a* 에서 사용)
	tagTile* node;			// 타일의 부모 노드 (a* 에서 사용)
};

// 플레이어 정보
struct tagPlayer {
	RECT rc;				// rect 이걸로 그냥 사각형 타일보다 작게 그림
	float x, y;				// 중점 x, y
	float width, height;	// 너비, 높이
	
							// 각도 타일의 중점과 플레이어 중점과 각도 계산해서
	float angle;			// 움직이는 방향 정하도록 구현 (대각 이동도 할 수 있게)

	float speed;			// 속도

	bool isMove;			// 플레이어가 움직여야 하는지 체크하는 변수
	float destX, destY;		// 목표 타일의 중점 x, y
	int index;				// 플레이어가 있는 타일의 index
};

class TestScene : public GameNode
{
private:
	bool isDebug;

	tagTile m_tile[TILEMAX];
	tagPlayer m_player;

	stack<tagTile> pathStack;	// a* 로 찾은 타일들을 담을 자료구조 (stack)
	stack<tagTile> showStack;	// pathStack과 같음, 옆에 보여주기 위해서 하나 더 쓴거
	stack<tagTile> lineStack;	// pathStack과 같음, 길을 라인으로 이으려고 하나 더 쓴거

	HBRUSH brush;				
	char str[128];
public:
	TestScene();
	~TestScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

								// 플레이어 움직이는 함수
	void Move();				// 목표 타일이 설정되면 목표 타일을 향해 움직이도록 구현
								
	void AStarPathFinding(tagTile& tile);	// a* 알고리즘, 해당 타일로 갈 수 있는 길 찾기
};

