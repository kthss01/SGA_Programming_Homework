#include "stdafx.h"
#include "TestScene.h"


TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

HRESULT TestScene::Init()
{
	isDebug = false;

	//SOUND->Play("Test", 0.5f);
	//
	//test.push_back("test");
	//test.push_back("test1");
	//test.push_back("test2");
	//test.push_back("test3");
	//
	//TEXTDATA->TextSave((char*)"save/test.txt", test);
	//
	//test = TEXTDATA->TextLoad((char*)"save/test.txt");
	//
	//INIDATA->AddData("Test", "KeyTest", "1000");
	//INIDATA->AddData("Test", "KeyTest2", "3.141592f");
	//INIDATA->AddData("Test2", "KeyTest3", "1234");
	//INIDATA->AddData("Test3", "KeyTest4", "1000");
	//INIDATA->AddData("Test", "KeyTest5", "1000");
	//INIDATA->SaveData("IniSave");

	for (int i = 0; i < TILEY; i++) {
		for (int j = 0; j < TILEX; j++) {
			ZeroMemory(&tiles[j + i * TILEX], sizeof(tagTile));
			tiles[j + i * TILEX].rc = RectMake(
				(WINSIZEX - 175) / TILEX * j + 175, WINSIZEY / TILEY * i + 10,
				(WINSIZEX - 175) / TILEX, WINSIZEY / TILEY);
			tiles[j + i * TILEX].block = false;
		}
	}

	startTile = endTile = -1;

	currentSelect = SELECT_START;

	isFind = false;
	noPath = false;
	startAStar = false;

	temp = 50;

	for (int i = 0; i < 5; i++) {
		rc[i] = RectMake(15 , 300 + i * 25 + i * 10 + temp * 2, 25, 25);
	}

	rc[5] = RectMake(15, 245 + temp, 140, 45);

	return S_OK;
}

void TestScene::Release()
{
}

void TestScene::Update()
{
	SOUND->Update();

	if (INPUT->GetKeyDown('1')) { currentSelect = SELECT_START; }
	if (INPUT->GetKeyDown('2')) { currentSelect = SELECT_END; }
	if (INPUT->GetKeyDown('3')) { currentSelect = SELECT_BLOCK; }
	if (INPUT->GetKeyDown('4')) {
		if (startTile != -1 && endTile != -1) {
			startAStar = true;
			currentTile = startTile;
			// 시작지점 open list에 추가
			openList.push_back(currentTile);
		}
	}
	if (INPUT->GetKeyDown('5')) {
		openList.clear();
		closeList.clear();
		Init();
	}

	if (INPUT->GetKey(VK_LBUTTON)) {
		for (int i = 0; i < TILESIZE; i++) {
			if (PtInRect(&tiles[i].rc, g_ptMouse)) {
				switch (currentSelect)
				{
				case SELECT_START:
					startTile = i;
					break;
				case SELECT_END:
					endTile = i;
					break;
				case SELECT_BLOCK:
					tiles[i].block = true;
					break;
				default:
					break;
				}
			}
		}
	}

	if (INPUT->GetKey(VK_RBUTTON)) {
		for (int i = 0; i < TILESIZE; i++) {
			if (PtInRect(&tiles[i].rc, g_ptMouse)) {
				if (currentSelect == SELECT_BLOCK)
					tiles[i].block = false;
			}
		}
	}

	if (!isFind && !noPath && startAStar)
		AStar();

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
		//for (int i = 0; i < test.size(); i++) {
		//	TextOut(GetMemDC(), WINSIZEX / 2, i * 20,
		//		test[i].c_str(), strlen(test[i].c_str()));
		//}
		//
		//sprintf_s(str, "%d",
		//	INIDATA->LoadDataInteger("IniSave", "Test3", "KeyTest4"));
		//TextOut(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2, str, strlen(str));
		//
		//float data = atof(INIDATA->LoadDataString("IniSave", "Test", "KeyTest2"));
		//sprintf_s(str, "%f", data);
		//TextOut(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2 + 50, str, strlen(str));

		for (int i = 0; i < TILESIZE; i++) {
			if (tiles[i].block) {
				if (i == startTile) startTile = -1;
				if (i == endTile) endTile = -1;
				BeginSolidColor(GetMemDC(), &brush, RGB(0, 0, 255));
			}
			else if (i == startTile) {
				BeginSolidColor(GetMemDC(), &brush, RGB(0, 255, 0));
			}
			else if (i == endTile) {
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 0, 0));
			}
			else if (tiles[i].showState == STATE_OPEN) {
				BeginSolidColor(GetMemDC(), &brush, RGB(128, 255, 255));
			}
			else if (tiles[i].showState == STATE_CLOSE) {
				BeginSolidColor(GetMemDC(), &brush, RGB(128, 255, 0));
			}
			else if (tiles[i].showState == STATE_PATH) {
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 128, 128));
			}
			else
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 255, 255));

			RectangleMake(GetMemDC(), tiles[i].rc);

			DeleteObject(brush);

			if (tiles[i].showState != STATE_NONE 
				&& i != endTile) {
				BeginCreateFont(GetMemDC(), &font, 8);
				oldFont = (HFONT)SelectObject(GetMemDC(), font);
				SetTextColor(GetMemDC(), RGB(0, 0, 0));
				sprintf_s(str, "%d", tiles[i].h);
				TextOut(GetMemDC(),
					tiles[i].rc.left + 2, tiles[i].rc.top + 2, str, strlen(str));
				sprintf_s(str, "%d", tiles[i].g);
				TextOut(GetMemDC(),
					tiles[i].rc.left + 2, tiles[i].rc.bottom - 10, str, strlen(str));
				sprintf_s(str, "%d", tiles[i].h);
				TextOut(GetMemDC(),
					tiles[i].rc.right - 17, tiles[i].rc.bottom - 10, str, strlen(str));
				SelectObject(GetMemDC(), oldFont);
				DeleteObject(font);
			}
		}

		SetTextColor(GetMemDC(), RGB(0, 0, 0));
		sprintf_s(str, "1: Start");
		TextOut(GetMemDC(), 10, 100, str, strlen(str));
		sprintf_s(str, "2: End");
		TextOut(GetMemDC(), 10, 130, str, strlen(str));
		sprintf_s(str, "3: Block (R Btn 제거)");
		TextOut(GetMemDC(), 10, 160, str, strlen(str));
		sprintf_s(str, "4: 시작");
		TextOut(GetMemDC(), 10, 190, str, strlen(str));
		sprintf_s(str, "5: 초기화");
		TextOut(GetMemDC(), 10, 220, str, strlen(str));

		RectangleMake(GetMemDC(), rc[5]);

		if (isFind) {
			BeginSolidColor(GetMemDC(), &brush, RGB(128, 255, 128));
			sprintf_s(str, "길찾기 성공!");
			TextOut(GetMemDC(), 40, 260 + temp, str, strlen(str));
			DeleteObject(brush);
		}
		if (noPath) {
			BeginSolidColor(GetMemDC(), &brush, RGB(255, 128, 128));
			sprintf_s(str, "길찾기 실패!");
			TextOut(GetMemDC(), 40, 260 + temp, str, strlen(str));
			DeleteObject(brush);
		}

		for (int i = 0; i < 5; i++) {
			if (i == 0) {
				BeginSolidColor(GetMemDC(), &brush, RGB(0, 255, 0));
				RectangleMake(GetMemDC(), rc[i]);
				sprintf_s(str, "시작점");
				DeleteObject(brush);
			}
			if (i == 1) {
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 0, 0));
				RectangleMake(GetMemDC(), rc[i]);
				sprintf_s(str, "도착점");
				DeleteObject(brush);
			}
			if (i == 2) {
				BeginSolidColor(GetMemDC(), &brush, RGB(128, 255, 255));
				RectangleMake(GetMemDC(), rc[i]);
				sprintf_s(str, "OpenList");
				DeleteObject(brush);
			}
			if (i == 3) {
				BeginSolidColor(GetMemDC(), &brush, RGB(128, 255, 0));
				RectangleMake(GetMemDC(), rc[i]);
				sprintf_s(str, "CloseList");
				DeleteObject(brush);
			}
			if (i == 4) {
				BeginSolidColor(GetMemDC(), &brush, RGB(255, 128, 128));
				RectangleMake(GetMemDC(), rc[i]);
				sprintf_s(str, "Path");
				DeleteObject(brush);
			}

			TextOut(GetMemDC(), rc[i].right + 15, rc[i].top + 5, str, strlen(str));
		}

	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
}

void TestScene::AStar()
{
	//int currentTile = startTile;
	int endX = endTile % TILEX;
	int endY = endTile / TILEX;

	//while (true) {

	int currentX = currentTile % TILEX;
	int currentY = currentTile / TILEX;
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool tempBlock[8];


	for (int i = 0; i < 8; i++) {
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		if (0 <= x && x < TILEX && 0 <= y && y < TILEY) {
			bool isOpen;
			if (tiles[y * TILEX + x].block) tempBlock[i] = true;
			else {
				// check closeList
				bool isClose = false;
				for (int j = 0; j < closeList.size(); j++) {
					if (closeList[j] == y * TILEX + x) {
						isClose = true;
						break;
					}
				}
				if (isClose) continue;

				switch (i) {
					// left
				case DIRECTION_LEFT:
					// path scoring
					tiles[y * TILEX + x].g = 10;
					tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
					tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

					// 오픈리스트에 있으면 g 비용 비교 후 처리
					isOpen = false;
					for (int i = 0; i < openList.size(); i++) {
						if (openList[i] == y * TILEX + x) {
							isOpen = true;
							if (tiles[openList[i]].g >
								tiles[y * TILEX + x].g) {
								tiles[openList[i]].h = tiles[y * TILEX + x].h;
								tiles[openList[i]].g = tiles[y * TILEX + x].g;
								tiles[openList[i]].f = tiles[y * TILEX + x].f;
								tiles[openList[i]].node = currentTile;
							}
						}
					}
					// 없으면 그냥 넣고 부모 설정
					if (!isOpen) {
						openList.push_back(y * TILEX + x);
						tiles[y * TILEX + x].node = currentTile;
					}

					// find
					if (y * TILEX + x == endTile)
						isFind = true;
					break;
					// right
				case DIRECTION_RIGHT:
					// path scoring
					tiles[y * TILEX + x].g = 10;
					tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
					tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;
					// 오픈리스트에 있으면 g 비용 비교 후 처리
					isOpen = false;
					for (int i = 0; i < openList.size(); i++) {
						if (openList[i] == y * TILEX + x) {
							isOpen = true;
							if (tiles[openList[i]].g >
								tiles[y * TILEX + x].g) {
								tiles[openList[i]].h = tiles[y * TILEX + x].h;
								tiles[openList[i]].g = tiles[y * TILEX + x].g;
								tiles[openList[i]].f = tiles[y * TILEX + x].f;
								tiles[openList[i]].node = currentTile;
							}
						}
					}
					// 없으면 그냥 넣고 부모 설정
					if (!isOpen) {
						openList.push_back(y * TILEX + x);
						tiles[y * TILEX + x].node = currentTile;
					}

					// find
					if (y * TILEX + x == endTile)
						isFind = true;
					break;
					// up
				case DIRECTION_UP:
					// path scoring
					tiles[y * TILEX + x].g = 10;
					tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
					tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

					// 오픈리스트에 있으면 g 비용 비교 후 처리
					isOpen = false;
					for (int i = 0; i < openList.size(); i++) {
						if (openList[i] == y * TILEX + x) {
							isOpen = true;
							if (tiles[openList[i]].g >
								tiles[y * TILEX + x].g) {
								tiles[openList[i]].h = tiles[y * TILEX + x].h;
								tiles[openList[i]].g = tiles[y * TILEX + x].g;
								tiles[openList[i]].f = tiles[y * TILEX + x].f;
								tiles[openList[i]].node = currentTile;
							}
						}
					}
					// 없으면 그냥 넣고 부모 설정
					if (!isOpen) {
						openList.push_back(y * TILEX + x);
						tiles[y * TILEX + x].node = currentTile;
					}

					// find
					if (y * TILEX + x == endTile)
						isFind = true;
					break;
					// down
				case DIRECTION_DOWN:
					// path scoring
					tiles[y * TILEX + x].g = 10;
					tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
					tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

					// 오픈리스트에 있으면 g 비용 비교 후 처리
					isOpen = false;
					for (int i = 0; i < openList.size(); i++) {
						if (openList[i] == y * TILEX + x) {
							isOpen = true;
							if (tiles[openList[i]].g >
								tiles[y * TILEX + x].g) {
								tiles[openList[i]].h = tiles[y * TILEX + x].h;
								tiles[openList[i]].g = tiles[y * TILEX + x].g;
								tiles[openList[i]].f = tiles[y * TILEX + x].f;
								tiles[openList[i]].node = currentTile;
							}
						}
					}
					// 없으면 그냥 넣고 부모 설정
					if (!isOpen) {
						openList.push_back(y * TILEX + x);
						tiles[y * TILEX + x].node = currentTile;
					}

					// find
					if (y * TILEX + x == endTile)
						isFind = true;
					break;
					// leftup 인 경우 left나 up 블락 체크
				case DIRECTION_LEFTUP:
					if (!tempBlock[DIRECTION_LEFT] &&
						!tempBlock[DIRECTION_UP]) {
						// path scoring
						tiles[y * TILEX + x].g = 14;
						tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
						tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

						// 오픈리스트에 있으면 g 비용 비교 후 처리
						isOpen = false;
						for (int i = 0; i < openList.size(); i++) {
							if (openList[i] == y * TILEX + x) {
								isOpen = true;
								if (tiles[openList[i]].g >
									tiles[y * TILEX + x].g) {
									tiles[openList[i]].h = tiles[y * TILEX + x].h;
									tiles[openList[i]].g = tiles[y * TILEX + x].g;
									tiles[openList[i]].f = tiles[y * TILEX + x].f;
									tiles[openList[i]].node = currentTile;
								}
							}
						}
						// 없으면 그냥 넣고 부모 설정
						if (!isOpen) {
							openList.push_back(y * TILEX + x);
							tiles[y * TILEX + x].node = currentTile;
						}

						// find
						if (y * TILEX + x == endTile)
							isFind = true;
					}
					break;
					// rightdown 경우 right나 down 블락 체크
				case DIRECTION_RIGHTDOWN:
					if (!tempBlock[DIRECTION_RIGHT] &&
						!tempBlock[DIRECTION_DOWN]) {
						// path scoring
						tiles[y * TILEX + x].g = 14;
						tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
						tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

						// 오픈리스트에 있으면 g 비용 비교 후 처리
						isOpen = false;
						for (int i = 0; i < openList.size(); i++) {
							if (openList[i] == y * TILEX + x) {
								isOpen = true;
								if (tiles[openList[i]].g >
									tiles[y * TILEX + x].g) {
									tiles[openList[i]].h = tiles[y * TILEX + x].h;
									tiles[openList[i]].g = tiles[y * TILEX + x].g;
									tiles[openList[i]].f = tiles[y * TILEX + x].f;
									tiles[openList[i]].node = currentTile;
								}
							}
						}
						// 없으면 그냥 넣고 부모 설정
						if (!isOpen) {
							openList.push_back(y * TILEX + x);
							tiles[y * TILEX + x].node = currentTile;
						}

						// find
						if (y * TILEX + x == endTile)
							isFind = true;
					}
					break;
					// leftdown 경우 left나 down 블락 체크
				case DIRECTION_LEFTDOWN:
					if (!tempBlock[DIRECTION_LEFT] &&
						!tempBlock[DIRECTION_DOWN]) {
						// path scoring
						tiles[y * TILEX + x].g = 14;
						tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
						tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

						// 오픈리스트에 있으면 g 비용 비교 후 처리
						isOpen = false;
						for (int i = 0; i < openList.size(); i++) {
							if (openList[i] == y * TILEX + x) {
								isOpen = true;
								if (tiles[openList[i]].g >
									tiles[y * TILEX + x].g) {
									tiles[openList[i]].h = tiles[y * TILEX + x].h;
									tiles[openList[i]].g = tiles[y * TILEX + x].g;
									tiles[openList[i]].f = tiles[y * TILEX + x].f;
									tiles[openList[i]].node = currentTile;
								}
							}
						}
						// 없으면 그냥 넣고 부모 설정
						if (!isOpen) {
							openList.push_back(y * TILEX + x);
							tiles[y * TILEX + x].node = currentTile;
						}

						// find
						if (y * TILEX + x == endTile)
							isFind = true;
					}
					break;
					// rightup 경우 right나 up 블락 체크
				case DIRECTION_RIGHTUP:
					if (!tempBlock[DIRECTION_RIGHT] &&
						!tempBlock[DIRECTION_UP]) {
						// path scoring
						tiles[y * TILEX + x].g = 14;
						tiles[y * TILEX + x].h = (abs(endX - x) + abs(endY - y)) * 10;
						tiles[y * TILEX + x].f = tiles[y * TILEX + x].g + tiles[y * TILEX + x].h;

						// 오픈리스트에 있으면 g 비용 비교 후 처리
						isOpen = false;
						for (int i = 0; i < openList.size(); i++) {
							if (openList[i] == y * TILEX + x) {
								isOpen = true;
								if (tiles[openList[i]].g >
									tiles[y * TILEX + x].g) {
									tiles[openList[i]].h = tiles[y * TILEX + x].h;
									tiles[openList[i]].g = tiles[y * TILEX + x].g;
									tiles[openList[i]].f = tiles[y * TILEX + x].f;
									tiles[openList[i]].node = currentTile;
								}
							}
						}
						// 없으면 그냥 넣고 부모 설정
						if (!isOpen) {
							openList.push_back(y * TILEX + x);
							tiles[y * TILEX + x].node = currentTile;
						}

						// find
						if (y * TILEX + x == endTile)
							isFind = true;
					}
					break;
				}
			}
		}
	}

	//// Find
	//if (isFind)
	//	break;

	// 선택 지점 열린목록에서 빼기
	for (iter = openList.begin(); iter != openList.end(); ++iter) {
		if ((*iter) == currentTile) {
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0) {
		//isFind = false;
		noPath = true;
		//break;
	}

	closeList.push_back(currentTile);

	if (openList.size() != 0) {
		// find minimum f cost in openList
		int min = tiles[*openList.begin()].h;
		currentTile = *openList.begin();
		for (iter = openList.begin(); iter != openList.end(); ++iter) {
			if (min > tiles[(*iter)].h) {
				min = tiles[(*iter)].h;
				currentTile = *iter;
			}
		}
	}
	//}

	for (int i = 0; i < openList.size(); i++) {
		tiles[openList[i]].showState = STATE_OPEN;
	}
	for (int i = 0; i < closeList.size(); i++) {
		tiles[closeList[i]].showState = STATE_CLOSE;
	}
	int tempTile = endTile;
	while (tiles[tempTile].node != startTile
		&& isFind) {
		tempTile = tiles[tempTile].node;
		tiles[tempTile].showState = STATE_PATH;
	}
}
