#include "stdafx.h"
#include "TestScene.h"

// 우선순위 큐 pair에서 
// 첫번째 값인 가중치가 같은 경우 
// 두번째 값인 타일을 비교해야 하므로
// 비교 연산자를 오버로딩 해야함
// 간단하게 가중치가 같은 경우 
// 두 타일의 행을 비교하여 행이 큰 값이 먼저 나오도록 하였음
// 작은 행부터 나오게 하려면 부호 반대로
bool operator<(tagTile t1, tagTile t2) {
	return t1.row < t2.row;
}

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

HRESULT TestScene::Init()
{
	isDebug = false;

	// tile 초기화
	for (int i = 0; i < TILEMAX; i++) {
		m_tile[i].width = 50;
		m_tile[i].height = 50;
		m_tile[i].row = i / TILEROW;
		m_tile[i].col = i % TILECOL;
		m_tile[i].x = 100 + m_tile[i].row * m_tile[i].width;
		m_tile[i].y = 100 + m_tile[i].col * m_tile[i].height;
		m_tile[i].block = false;
		m_tile[i].rc = RectMakeCenter(
			m_tile[i].x, m_tile[i].y,
			m_tile[i].width, m_tile[i].height);
	}

	// player 초기화
	ZeroMemory(&m_player, sizeof(tagPlayer));
	m_player.width = 30;
	m_player.height = 30;

	// 플레이어 처음 위치 가장 왼쪽 위 타일부터 시작
	m_player.x = m_tile[0].x;
	m_player.y = m_tile[0].y;
	m_player.index = 0;
	
	m_player.rc = RectMakeCenter(
		m_player.x, m_player.y,
		m_player.width, m_player.height);
	m_player.speed = 2.5f;
	m_player.isMove = false;

	return S_OK;
}

void TestScene::Release()
{
}

void TestScene::Update()
{
	// 마우스 왼쪽 버튼 클릭 시 (플레이어가 움직이지 않을 때)
	if (INPUT->GetKeyDown(VK_LBUTTON) && 
		!m_player.isMove) {

		// 타일 검색
		for (int i = 0; i < TILEMAX; i++) {
			// 타일 클릭 시
			if (PtInRect(&m_tile[i].rc, g_ptMouse)) {
				// 해당 타일의 목표 타일로 설정
				m_tile[i].dest = true;
				// 플레이어 움직이도록 설정
				m_player.isMove = true;
			
				// a*로 플레이어의 위치로부터 해당 타일까지의 길찾기
				AStarPathFinding(m_tile[i]);	
				
				// 찾아진 길에서 타일 하나 꺼냄 (스택에 맨위에 있는거)
				tagTile temp = pathStack.top();
				// 꺼낸 타일 제거
				pathStack.pop();	

				// 첫 목표 타일 설정
				m_player.destX = temp.x;	// 목표 타일의 중점 x 
				m_player.destY = temp.y;	// 목표 타일의 중점 y
				// 목표 타일과 플레이어와의 각도 계산
				m_player.angle = GetAngle(
					m_player.x, m_player.y, m_player.destX, m_player.destY);

				break;
			}
		}
	}

	// 마우스 오른쪽 버튼으로 장애물 생성 및 제거 
	if (INPUT->GetKeyDown(VK_RBUTTON)) {
		// 타일 검색
		for (int i = 0; i < TILEMAX; i++) {
			// 해당 타일 클릭 시
			if (PtInRect(&m_tile[i].rc, g_ptMouse))
				// 타일의 장애물 상태 토글 (false -> true, true -> false);
				m_tile[i].block = !m_tile[i].block;
		}
	}

	// 플레이어 움직이는 함수
	Move();

	// path 스택에 내용 line과 show stack에 넣기
	showStack = lineStack = pathStack;

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void TestScene::Render()
{
	//=================================================
	{
		// 타일 그리기
		for (tagTile tile : m_tile) {
			// 장애물 타일이면
			if (tile.block) {
				// 파란색
				BeginSolidColor(GetMemDC(), &brush, RGB(0, 0, 255));
			}
			// 목표 타일
			else if (tile.dest) {
				// 빨간색
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 0, 0));
			}
			// 나머지 타일
			else {
				// 흰색
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 255, 255));
			}
			// 타일 Rect로 그리기
			RectangleMake(GetMemDC(), tile.rc);

			DeleteObject(brush);

			// 타일 위에 타일의 행, 열 쓰기
			SetBkMode(GetMemDC(), TRANSPARENT);
			sprintf_s(str, "(%d,%d)", tile.row, tile.col);
			TextOut(GetMemDC(),
				tile.rc.left + 3, tile.rc.top, str, strlen(str));			
		}

		// 플레이어 그리기
		BeginSolidColor(GetMemDC(), &brush, RGB(255, 255, 255));
		RectangleMake(GetMemDC(), m_player.rc);
		DeleteObject(brush);

		// 길 그리기 (한 선으로 안그려짐)
		//while (lineStack.size() > 1) {
		//	tagTile t1 = lineStack.top();
		//	lineStack.pop();
		//	tagTile t2 = lineStack.top();
		//	lineStack.pop();
		//	LineMake(GetMemDC(), PointMake(t1.x, t1.y), PointMake(t2.x, t2.y));
		//}

		// 길 그리기 한 선으로
		if (lineStack.size() > 1) {
			// 시작 타일 꺼내기 (스택에 맨위에 있는거)
			tagTile start = lineStack.top();
			// 꺼낸 타일은 스택에서 제거
			lineStack.pop();

			// 시작 타일의 중점으로 라인 그리기 시작
			MoveToEx(GetMemDC(), start.x, start.y, NULL);

			// 스택이 빌 때 까지
			while (!lineStack.empty()) {
				// 타일 꺼내기 (스택에 맨위에 있는거)
				tagTile temp = lineStack.top();
				// 꺼낸 타일 제거
				lineStack.pop();

				// 해당 타일까지 라인 그리기
				LineTo(GetMemDC(), temp.x, temp.y);
			}
		}

		// 오른쪽 빈 공간에 이동할 타일들 쓰기
		sprintf_s(str, "<path>");
		TextOut(GetMemDC(), WINSIZEX - 250, 75, str, strlen(str));

		int index = 0;
		// show 스택이 빌 때까지
		while (!showStack.empty()) {
			// 타일 하나 꺼내기 (스택에 맨위에 있는거)
			tagTile temp = showStack.top();
			// 꺼낸 타일 제거
			showStack.pop();
			sprintf_s(str, "%d's tile pos = (%d, %d)", 
				index + 1, temp.row, temp.col);
			TextOut(GetMemDC(), WINSIZEX - 300, 100 + index * 25, str, strlen(str));
			index++;
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		// 각 타일들의 a* 알고리즘에서의 가중치 그리기 
		for (tagTile tile : m_tile) {
			sprintf_s(str, "f=%d", tile.f.h + tile.f.g);
			TextOut(GetMemDC(),
				tile.rc.left + 3, tile.rc.bottom - 15, str, strlen(str));
		}
	}
	//=================================================
}

void TestScene::Move()
{
	// 플레이어가 움직이도록 설정된 경우
	if (m_player.isMove) {
		// 해당 플레이어의 중점과 목표 중점과의 거리 계산하여
		// 일정 거리 (10) 이하인 경우
		if (GetDistance(
			m_player.x, m_player.y, m_player.destX, m_player.destY) < 10) {
			// path 스택이 비어있으면 이동 멈추기
			if (pathStack.empty()) {
				m_player.isMove = false;
				m_player.x = m_player.destX;
				m_player.y = m_player.destY;
			}
			// path 스택이 남아있으면 이동
			else {
				// 타일 하나 꺼내기 (스택 맨위에 있는거)
				tagTile temp = pathStack.top();
				// 꺼낸 타일 제거
				pathStack.pop();

				// 해당 타일로 목표 설정
				m_player.destX = temp.x;
				m_player.destY = temp.y;
				m_player.angle = GetAngle(
					m_player.x, m_player.y, m_player.destX, m_player.destY);
				m_player.index = temp.row * TILECOL + temp.col;
				// 꺼낸 타일이 목표 타일인 경우 목표 타일 설정 false로 
				// 빨간색으로 그리던거 그만 하려고
				if (m_tile[temp.row * TILECOL + temp.col].dest)
					m_tile[temp.row * TILECOL + temp.col].dest = false;
			}
		}
		// 해당 플레이어의 중점과 목표 중점과의 거리 계산하여
		// 일정 거리 (10) 이상인 경우 이동 
		else {
			m_player.x += cosf(m_player.angle) * m_player.speed;
			m_player.y -= sinf(m_player.angle) * m_player.speed;
		}
		// 이동한 위치로 rect 다시 만들기
		m_player.rc = RectMakeCenter(
			m_player.x, m_player.y, m_player.width, m_player.height);
	}
}

void TestScene::AStarPathFinding(tagTile& tile)
{
	// a* 에서 사용할 우선순위 큐
	// 가중치와 해당 타일을 pair로 묶어서 같이 넣음
	// 여기다 넣는게 구글링 설명에서의 열린 목록의 넣는거라고 보면 됨
	// 우선 순위 큐는 max힙으로 기본 설정 되어 있어서
	// 넣고 뺄 때 가장 큰게 나오나 가장 작은 값 (최저 비용) 이 나오길 원하므로
	// 다양한 방법이 있지만 가중치에 - 를 붙이는 방식을 사용할 예정
	// 50 30 10 -> -50 -30 -10 이렇게 넣으면 -10 이 나오므로 (가장 크니)

	priority_queue< pair<int, tagTile> > pq;

	// a* 시작하기 위해 타일들 초기화
	for (int i = 0; i < TILEROW * TILECOL; i++) {
		// 장애물인 경우 닫힌 목록으로 설정
		m_tile[i].close = m_tile[i].block ? true : false;
		// 부모 노드 다 초기화
		m_tile[i].node = NULL;
	}
	// path 스택 다 비우기
	while (!pathStack.empty())
		pathStack.pop();

	// 플레이어가 있는 타일 닫힌 목록으로 설정
	m_tile[m_player.index].close = true;
	// 플레이어의 타일 우선순위 큐에 넣기 (가중치 가장 크게 해서 먼저 나오게 (0))
	pq.push(make_pair(0, m_tile[m_player.index]));

	// 이동한 위치 위아래좌우 거리와 가중치 계산해서 pq에 넣기
	// 대각선은 뺏음 타일 이동이므로 필요시 대각선만 추가하면 됨
	while (!pq.empty()) {
		// 우선 순위 큐에서 가장 큰 값 꺼내기 
		// 가중치 - 이므로 가중치가 가장 작은 타일 꺼내기
		tagTile temp = pq.top().second;
		// 꺼낸 타일 제거
		pq.pop();
		
		// 꺼낸 타일이 목표 타일의 행과 열과 같으면 종료
		if (temp.row == tile.row && temp.col == tile.col)
			break;

		// 인덱스 계산 타일의 행 * 열 갯수 + 타일의 열 하면 됨
		int index = temp.row * TILECOL + temp.col;

		// 플레이어 주변 타일 거리와 가중치 계산
		// up
		// 타일이 젤 아래 행보다 크고 해당 타일이 닫힌 목록이 아니면
		if ((index >= TILECOL)
			&& !m_tile[index - TILECOL].close) {
			// 해당 타일 닫힌 목록으로 변경
			m_tile[index - TILECOL].close = true;
			// 이동비용 세로 이므로 10
			m_tile[index - TILECOL].f.g = 10;
			// 멘하탄 방식으로 예상이동비용 계산
			m_tile[index - TILECOL].f.h =
				abs(tile.row - m_tile[index - TILECOL].row)
				+ abs(tile.col - m_tile[index - TILECOL].col);
			// 해당 타일의 부모 노드는 현재 선택된 타일로 설정
			m_tile[index - TILECOL].node = &m_tile[index];
			// 우선순위 큐에 가중치 값 계산하여 타일과 묶어서 넣기
			pq.push(
				make_pair(
					-m_tile[index - TILECOL].f.h +
					-m_tile[index - TILECOL].f.g,
					m_tile[index - TILECOL]));
		}
		// down
		// 타일이 젤 위 행보다 작고 해당 타일이 닫힌 목록이 아니면
		if ((index < TILEROW * TILECOL - TILECOL)
			&& !m_tile[index + TILECOL].close) {
			// 해당 타일 닫힌 목록으로 변경
			m_tile[index + TILECOL].close = true;
			// 이동비용 세로 이므로 10
			m_tile[index + TILECOL].f.g = 10;
			// 멘하탄 방식으로 예상이동비용 계산
			m_tile[index + TILECOL].f.h =
				abs(tile.row - m_tile[index + TILECOL].row)
				+ abs(tile.col - m_tile[index + TILECOL].col);
			// 해당 타일의 부모 노드는 현재 선택된 타일로 설정
			m_tile[index + TILECOL].node = &m_tile[index];
			// 우선순위 큐에 가중치 값 계산하여 타일과 묶어서 넣기
			pq.push(
				make_pair(
					-m_tile[index + TILECOL].f.h +
					-m_tile[index + TILECOL].f.g,
					m_tile[index + TILECOL]));
		}
		// left
		// 타일이 젤 왼쪽 열보다 크고 해당 타일이 닫힌 목록이 아니면
		if ((index % TILECOL > 0)
			&& !m_tile[index - 1].close) {
			// 해당 타일 닫힌 목록으로 변경
			m_tile[index - 1].close = true;
			// 이동비용 가로 이므로 10
			m_tile[index - 1].f.g = 10;
			// 멘하탄 방식으로 예상이동비용 계산
			m_tile[index - 1].f.h =
				abs(tile.row - m_tile[index - 1].row)
				+ abs(tile.col - m_tile[index - 1].col);
			// 해당 타일의 부모 노드는 현재 선택된 타일로 설정
			m_tile[index - 1].node = &m_tile[index];
			// 우선순위 큐에 가중치 값 계산하여 타일과 묶어서 넣기
			pq.push(
				make_pair(
					-m_tile[index - 1].f.h +
					-m_tile[index - 1].f.g,
					m_tile[index - 1]));
		}
		// right
		// 타일이 젤 오른쪽 열보다 작고 해당 타일이 닫힌 목록이 아니면
		if ((index % TILECOL < TILECOL - 1)
			&& !m_tile[index + 1].close) {
			// 해당 타일 닫힌 목록으로 변경
			m_tile[index + 1].close = true;
			// 이동비용 가로 이므로 10
			m_tile[index + 1].f.g = 10;
			// 멘하탄 방식으로 예상이동비용 계산
			m_tile[index + 1].f.h =
				abs(tile.row - m_tile[index + 1].row)
				+ abs(tile.col - m_tile[index + 1].col);
			// 해당 타일의 부모 노드는 현재 선택된 타일로 설정
			m_tile[index + 1].node = &m_tile[index];
			// 우선순위 큐에 가중치 값 계산하여 타일과 묶어서 넣기
			pq.push(
				make_pair(
					-m_tile[index + 1].f.h +
					-m_tile[index + 1].f.g,
					m_tile[index + 1]));
		}
	}

	// 목표 타일 임시 타일의 넣기
	tagTile temp = tile;

	// 목표 타일을 path 스택에 넣기 (목표 타일에도 도달해야 하므로)
	pathStack.push(temp);

	// 목표타일의 행, 열과 임시타일의 행, 열이 같을 때 까지 반복
	while ((temp.row != m_tile[m_player.index].row) ||
		(temp.col != m_tile[m_player.index].col)) {
		// 임시 타일의 부모 노드를 path 스택에 넣기
		pathStack.push(*temp.node);
		// 임시 타일을 부모 타일로 변경
		temp = *temp.node;
	}
}
