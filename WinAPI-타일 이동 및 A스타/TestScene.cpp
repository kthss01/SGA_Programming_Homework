#include "stdafx.h"
#include "TestScene.h"

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

	// tile
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

	// player
	ZeroMemory(&m_player, sizeof(tagPlayer));
	m_player.width = 30;
	m_player.height = 30;
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
	if (INPUT->GetKeyDown(VK_LBUTTON) && 
		!m_player.isMove) {

		for (int i = 0; i < TILEMAX; i++) {
			if (PtInRect(&m_tile[i].rc, g_ptMouse)) {
				m_tile[i].dest = true;
				m_player.isMove = true;
			
				AStarPathFinding(m_tile[i]);	
	
				tagTile temp = pathStack.top();
				pathStack.pop();

				m_player.destX = temp.x;
				m_player.destY = temp.y;
				m_player.angle = GetAngle(
					m_player.x, m_player.y, m_player.destX, m_player.destY);

				break;
			}
		}
	}

	if (INPUT->GetKeyDown(VK_RBUTTON)) {
		for (int i = 0; i < TILEMAX; i++) {
			if (PtInRect(&m_tile[i].rc, g_ptMouse))
				m_tile[i].block = !m_tile[i].block;
		}
	}

	Move();

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
		for (tagTile tile : m_tile) {
			if (tile.block) {
				BeginSolidColor(GetMemDC(), &brush, RGB(0, 0, 255));
			}
			else if (tile.dest) {
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 0, 0));
			}
			else {
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 255, 255));
			}
			RectangleMake(GetMemDC(), tile.rc);

			DeleteObject(brush);

			SetBkMode(GetMemDC(), TRANSPARENT);
			sprintf_s(str, "(%d,%d)", tile.row, tile.col);
			TextOut(GetMemDC(),
				tile.rc.left + 3, tile.rc.top, str, strlen(str));			
		}

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
			tagTile start = lineStack.top();
			lineStack.pop();

			MoveToEx(GetMemDC(), start.x, start.y, NULL);

			while (!lineStack.empty()) {
				tagTile temp = lineStack.top();
				lineStack.pop();

				LineTo(GetMemDC(), temp.x, temp.y);
			}
		}

		sprintf_s(str, "<path>");
		TextOut(GetMemDC(), WINSIZEX - 250, 75, str, strlen(str));

		int index = 0;
		while (!showStack.empty()) {
			tagTile temp = showStack.top();
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
	if (m_player.isMove) {
		if (GetDistance(
			m_player.x, m_player.y, m_player.destX, m_player.destY) < 10) {
			if (pathStack.empty()) {
				m_player.isMove = false;
				m_player.x = m_player.destX;
				m_player.y = m_player.destY;
			}
			else {
				tagTile temp = pathStack.top();
				pathStack.pop();

				m_player.destX = temp.x;
				m_player.destY = temp.y;
				m_player.angle = GetAngle(
					m_player.x, m_player.y, m_player.destX, m_player.destY);
				m_player.index = temp.row * TILECOL + temp.col;
				if (m_tile[temp.row * TILECOL + temp.col].dest)
					m_tile[temp.row * TILECOL + temp.col].dest = false;
			}
		}
		else {
			m_player.x += cosf(m_player.angle) * m_player.speed;
			m_player.y -= sinf(m_player.angle) * m_player.speed;
		}
		m_player.rc = RectMakeCenter(
			m_player.x, m_player.y, m_player.width, m_player.height);
	}
}

void TestScene::AStarPathFinding(tagTile& tile)
{
	
	priority_queue< pair<int, tagTile> > pq;

	// Init
	for (int i = 0; i < TILEROW * TILECOL; i++) {
		m_tile[i].close = m_tile[i].block ? true : false;
		m_tile[i].node = NULL;
	}
	while (!pathStack.empty())
		pathStack.pop();

	m_tile[m_player.index].close = true;
	pq.push(make_pair(0, m_tile[m_player.index]));

	// 이동한 위치 위아래좌우 거리와 가중치 계산해서 pq에 넣기
	while (!pq.empty()) {
		tagTile temp = pq.top().second;
		pq.pop();

		if (temp.row == tile.row && temp.col == tile.col)
			break;

		int index = temp.row * TILECOL + temp.col;

		// 플레이어 주변 타일 거리와 가중치 계산
		// up
		if ((index >= TILECOL)
			&& !m_tile[index - TILECOL].close) {
			m_tile[index - TILECOL].close = true;
			m_tile[index - TILECOL].f.g = 10;
			m_tile[index - TILECOL].f.h =
				abs(tile.row - m_tile[index - TILECOL].row)
				+ abs(tile.col - m_tile[index - TILECOL].col);
			m_tile[index - TILECOL].node = &m_tile[index];
			pq.push(
				make_pair(
					-m_tile[index - TILECOL].f.h +
					-m_tile[index - TILECOL].f.g,
					m_tile[index - TILECOL]));
		}
		// down
		if ((index < TILEROW * TILECOL - TILECOL)
			&& !m_tile[index + TILECOL].close) {
			m_tile[index + TILECOL].close = true;
			m_tile[index + TILECOL].f.g = 10;
			m_tile[index + TILECOL].f.h =
				abs(tile.row - m_tile[index + TILECOL].row)
				+ abs(tile.col - m_tile[index + TILECOL].col);
			m_tile[index + TILECOL].node = &m_tile[index];
			pq.push(
				make_pair(
					-m_tile[index + TILECOL].f.h +
					-m_tile[index + TILECOL].f.g,
					m_tile[index + TILECOL]));
		}
		// left
		if ((index % TILECOL > 0)
			&& !m_tile[index - 1].close) {
			m_tile[index - 1].close = true;
			m_tile[index - 1].f.g = 10;
			m_tile[index - 1].f.h =
				abs(tile.row - m_tile[index - 1].row)
				+ abs(tile.col - m_tile[index - 1].col);
			m_tile[index - 1].node = &m_tile[index];
			pq.push(
				make_pair(
					-m_tile[index - 1].f.h +
					-m_tile[index - 1].f.g,
					m_tile[index - 1]));
		}
		// right
		if ((index % TILECOL < TILECOL - 1)
			&& !m_tile[index + 1].close) {
			m_tile[index + 1].close = true;
			m_tile[index + 1].f.g = 10;
			m_tile[index + 1].f.h =
				abs(tile.row - m_tile[index + 1].row)
				+ abs(tile.col - m_tile[index + 1].col);
			m_tile[index + 1].node = &m_tile[index];
			pq.push(
				make_pair(
					-m_tile[index + 1].f.h +
					-m_tile[index + 1].f.g,
					m_tile[index + 1]));
		}
	}

	tagTile temp = tile;

	pathStack.push(temp);

	while ((temp.row != m_tile[m_player.index].row) ||
		(temp.col != m_tile[m_player.index].col)) {
		pathStack.push(*temp.node);
		temp = *temp.node;
	}
}
