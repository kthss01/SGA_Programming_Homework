#include "stdafx.h"
#include "GameScene1.h"


GameScene1::GameScene1()
{
}


GameScene1::~GameScene1()
{
}

HRESULT GameScene1::Init()
{
	isDebug = false;

	board = new Image;
	board->Init("images/minesweeper/board.bmp", BOARDSIZEX, BOARDSIZEY, true);

	for (int i = 0; i < 6; i++) {
		number[i] = new Image;
		number[i]->Init("images/minesweeper/number.bmp", 350, 55, 10, 1, true);
	}

	// 전광판에 숫자를 넣기 위한 함수
	ChangeNumber(true, MINECOUNT);
	// 깃발 갯수를 샐 수 있는 변수
	flagCount = 0;
	ChangeNumber(false, MINECOUNT - flagCount);

	sizeX = FIELDX / ROW;
	sizeY = FIELDY / COL;

	// 메뉴창 이미지 읽기
	menu = new Image;
	menu->Init("images/minesweeper/menu.bmp", BOARDSIZEX, 50);

	// 결과 이미지 읽기
	result = new Image;
	result->Init("images/minesweeper/result.bmp", 200, 68, 4, 1, true);

	// 마인 이미지 읽기
	for (int i = 0; i < 13; i++) {
		mine[i] = new Image;
		// 숫자
		if (i < 8) {
			sprintf_s(str, "images/minesweeper/%d.bmp", i + 1);
		}
		else {
			switch (i) {
			case 8:
				sprintf_s(str, "images/minesweeper/boom.bmp");
				break;
			case 9:
				sprintf_s(str, "images/minesweeper/empty.bmp");
				break;
			case 10:
				sprintf_s(str, "images/minesweeper/flag.bmp");
				break;
			case 11:
				sprintf_s(str, "images/minesweeper/mine.bmp");
				break;
			case 12:
				sprintf_s(str, "images/minesweeper/ncb.bmp");
				break;
			}
		}
		mine[i]->Init(str, sizeX, sizeY);
	}

	// 처음 10개에 마인 넣어두고 셔플
	for (int i = 0; i < ROW*COL; i++) {
		if (i < MINECOUNT)
			isMine[i] = true;
		else
			isMine[i] = false;
	}
	// 마인 셔플
	MineShuffle();

	// 마인 정보 초기화
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (isMine[i*COL + j]) {
				// test
				//mineInfo[i][j].status = STATUS_MINE;
				mineInfo[i][j].status = STATUS_NCB;
				mineInfo[i][j].isMine = true;
			}
			else
			{
				mineInfo[i][j].isMine = false;
				mineInfo[i][j].status = STATUS_NCB;
			}
			mineInfo[i][j].x = STARTX + sizeX * j;
			mineInfo[i][j].y = STARTY + sizeY * i;
			mineInfo[i][j].isFlag = false;
			mineInfo[i][j].nearMineCount = 0;
			mineInfo[i][j].rc = RectMake(
				mineInfo[i][j].x, mineInfo[i][j].y,
				sizeX, sizeY);
		}
	}
	
	isOver = false;
	isClear = false;

	// 주변 마인 갯수 체크
	MineCheck();
	// test
	/*for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			switch (mineInfo[i][j].nearMineCount) {
			case 0:
				if(!mineInfo[i][j].isMine)
					mineInfo[i][j].status = STATUS_EMPTY;
				break;
			case 1:
				mineInfo[i][j].status = STATUS_ONE;
				break;
			case 2:
				mineInfo[i][j].status = STATUS_TWO;
				break;
			case 3:
				mineInfo[i][j].status = STATUS_THREE;
				break;
			case 4:
				mineInfo[i][j].status = STATUS_FOUR;
				break;
			}
		}
	}*/

	rcMineExit = RectMake(WINSIZEX / 2 + BOARDSIZEX - 17, 50, 17, 25);
	rcMineRestart = RectMake(678, 151, 50, 68);

	return S_OK;
}

void GameScene1::Release()
{
	SAFE_DELETE(board);
	for (int i = 0; i < 6; i++)
		SAFE_DELETE(number[i]);
	for (int i = 0; i < 13; i++)
		SAFE_DELETE(mine[i]);
	SAFE_DELETE(result);
	SAFE_DELETE(menu);
}

void GameScene1::Update()
{
	// 마우스 클릭 시
	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		// 종료 창 누르면 씬 끄기
		if (PtInRect(&rcMineExit, g_ptMouse)) {
			SCENE->ChangeScene("None");
			return;
		}

		// 재시작 버튼 누를시 새로 시작
		if (PtInRect(&rcMineRestart, g_ptMouse)) {
			this->Init();
			return;
		}

		// 지뢰 찾기
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++) {
				if (PtInRect(&mineInfo[i][j].rc, g_ptMouse)
					&& mineInfo[i][j].isFlag == false) {
					// 지뢰 찾을 때마다 결과 애니메이션 변화
					result->SetFrameX(1);
					if (mineInfo[i][j].isMine) {
						mineInfo[i][j].status = STATUS_BOOM;
						GameOver();
					}
					else
						FindMine(i, j);
				}
			}
		}
	}

	if (isOver || isClear) return;

	// 마우스 때면 원상태로 애니메이션 변화
	if (INPUT->GetKeyUp(VK_LBUTTON)) {
		result->SetFrameX(0);
	}

	// 깃발 꼽기
	if (INPUT->GetKeyDown(VK_RBUTTON)) {
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (PtInRect(&mineInfo[i][j].rc, g_ptMouse)) {
					// 클릭되지 않은 상태고 깃발 갯수 초과되지 않으면 깃발꼽기
					if (mineInfo[i][j].status == STATUS_NCB
						&& flagCount < MINECOUNT) {
						mineInfo[i][j].isFlag = true;
						mineInfo[i][j].status = STATUS_FLAG;
						flagCount++;
						ChangeNumber(false, MINECOUNT - flagCount);
					}
					else if (mineInfo[i][j].status == STATUS_FLAG) {
						mineInfo[i][j].isFlag = false;
						mineInfo[i][j].status = STATUS_NCB;
						flagCount--;
						ChangeNumber(false, MINECOUNT - flagCount);
					}
				}
			}
		}
	}

	// 게임 클리어를 위한 체크
	// 클릭되지 않은 타일이 없으면 클리어
	isClear = true;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (!mineInfo[i][j].isMine &&
				mineInfo[i][j].status == STATUS_NCB)
				isClear = false;
		}
	}

	//test
	//if (INPUT->GetKeyDown(VK_SPACE)) {
	//	MineCheck();
	//	for (int i = 0; i < ROW; i++) {
	//		for (int j = 0; j < COL; j++) {
	//			switch (mineInfo[i][j].nearMineCount) {
	//			case 0:
	//				if (!mineInfo[i][j].isMine)
	//					mineInfo[i][j].status = STATUS_EMPTY;
	//				break;
	//			case 1:
	//				mineInfo[i][j].status = STATUS_ONE;
	//				break;
	//			case 2:
	//				mineInfo[i][j].status = STATUS_TWO;
	//				break;
	//			case 3:
	//				mineInfo[i][j].status = STATUS_THREE;
	//				break;
	//			case 4:
	//				mineInfo[i][j].status = STATUS_FOUR;
	//				break;
	//			}
	//		}
	//	}
	//}

	if (isClear) {
		GameClear();
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void GameScene1::Render()
{
	// 보드 그리기
	board->Render(GetMemDC(), WINSIZEX/2, 100);

	// 메뉴 그리기
	menu->Render(GetMemDC() , WINSIZEX/2, 100 - menu->GetHeight());

	// 결과 그리기
	result->FrameRender(GetMemDC(), 678, 151);

	// 숫자 그리기
	//number->Render(GetMemDC());
	for (int i = 0; i < 6; i++) {
		// 화면 왼쪽 실제 지뢰 갯수
		if(i < 3)
		number[i]->FrameRender(GetMemDC(), 
			550 + i * (number[i]->GetFrameWidth() + 5), 157);
		// 화면 오른쪽 지뢰 갯수 - 깃발 꼿은 갯수
		else
		number[i]->FrameRender(GetMemDC(),
			742 + (i-3) * (number[i]->GetFrameWidth() + 5), 157);
	}

	// 지뢰 그리기

	// test
	//for (int i = 0; i < 9; i++) {
	//	mine[i]->Render(GetMemDC(), 200, 200 + 50*i);
	//}

	// 상태에 따라 지뢰 그리기
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			mine[mineInfo[i][j].status]->Render(GetMemDC(),
				mineInfo[i][j].x, mineInfo[i][j].y);
		}
	}
	//// test
	//for (int i = 0; i < ROW; i++) {
	//	for (int j = 0; j < COL; j++)
	//		RectangleMake(GetMemDC(), mineInfo[i][j].rc);
	//}

	//==================   Debug   ====================
	if (isDebug)
	{
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (mineInfo[i][j].isMine) {
					RectangleMake(GetMemDC(), mineInfo[i][j].rc);
				}
			}
		}

		RectangleMake(GetMemDC(), rcMineExit);
		RectangleMake(GetMemDC(), rcMineRestart);

		// test
		/*for (int i = 0; i < 13; i++)
			mine[i]->Render(GetMemDC(), i*sizeX, 0);*/
	}
	//=================================================
}

void GameScene1::ChangeNumber(bool mine, int number)
{
	if (mine) {
		// 백의 자리
		this->number[0]->SetFrameX(number / 100);
		// 십의 자리
		this->number[1]->SetFrameX(number / 10 % 10);
		// 일의 자리
		this->number[2]->SetFrameX(number % 10);
	}
	else {
		this->number[3]->SetFrameX(number / 100);
		this->number[4]->SetFrameX(number / 10 % 10);
		this->number[5]->SetFrameX(number % 10);
	}
}

void GameScene1::MineShuffle()
{
	for (int i = 0; i < 1000; i++) {
		int sour = RND->GetInt(ROW*COL);
		int dest = RND->GetInt(ROW*COL);

		bool temp = isMine[dest];
		isMine[dest] = isMine[sour];
		isMine[sour] = temp;
	}
}

void GameScene1::MineCheck()
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			// 해당 타일이 지뢰면 무시
			if (mineInfo[i][j].isMine) continue;
			// 주변의 위치에 마인 있으면 주변 마인 갯수 1 증가
			// 위
			if (i > 0 && mineInfo[i-1][j].isMine) 
				mineInfo[i][j].nearMineCount++;
			// 왼
			if (j > 0 && mineInfo[i][j - 1].isMine) 
				mineInfo[i][j].nearMineCount++;
			// 아래
			if (i < ROW - 1 && mineInfo[i + 1][j].isMine) 
				mineInfo[i][j].nearMineCount++;
			// 오른
			if (j < COL - 1 && mineInfo[i][j + 1].isMine) 
				mineInfo[i][j].nearMineCount++;
			// 왼 위
			if (j > 0 && i > 0 && mineInfo[i - 1][j - 1].isMine)
				mineInfo[i][j].nearMineCount++;
			// 왼 아래
			if (j > 0 && i < ROW - 1 && mineInfo[i + 1][j - 1].isMine)
				mineInfo[i][j].nearMineCount++;
			// 오른 위
			if (i > 0 && j < COL - 1 && mineInfo[i - 1][j + 1].isMine)
				mineInfo[i][j].nearMineCount++;
			// 오른 아래
			if (i < ROW - 1 && j < COL - 1 && mineInfo[i + 1][j + 1].isMine)
				mineInfo[i][j].nearMineCount++;
		}
	}
}

void GameScene1::GameOver()
{
	result->SetFrameX(2);

	// 클릭되지 않은 지뢰 다 보여주기
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (mineInfo[i][j].isMine && mineInfo[i][j].status == STATUS_NCB)
				mineInfo[i][j].status = STATUS_MINE;
		}
	}

	//KillTimer(g_hWnd, 1);
	isOver = true;
}

void GameScene1::GameClear()
{
	result->SetFrameX(3);
	
	//KillTimer(g_hWnd, 1);
	isClear = true;
}

// 재귀함수로 지뢰 찾기
void GameScene1::FindMine(int row, int col)
{
	// 해당 위치가 지뢰이거나 클릭되지 않은 타일이 아니면 리턴(반환)
	if (mineInfo[row][col].isMine ||
		mineInfo[row][col].status != STATUS_NCB) {
		return;
	}
	else {
		// 주변 지뢰 갯수에 따라 타일 변경 (타일의 상태 변경)
		switch (mineInfo[row][col].nearMineCount)
		{
		case 0:
			mineInfo[row][col].status = STATUS_EMPTY;
			break;
		case 1:
			mineInfo[row][col].status = STATUS_ONE;
			break;
		case 2:
			mineInfo[row][col].status = STATUS_TWO;
			break;
		case 3:
			mineInfo[row][col].status = STATUS_THREE;
			break;
		case 4:
			mineInfo[row][col].status = STATUS_FOUR;
			break;
		case 5:
			mineInfo[row][col].status = STATUS_FIVE;
			break;
		case 6:
			mineInfo[row][col].status = STATUS_SIX;
			break;
		case 7:
			mineInfo[row][col].status = STATUS_SEVEN;
			break;
		case 8:
			mineInfo[row][col].status = STATUS_EIGHT;
			break;
		}
	}

	// 왼쪽으로 재귀 호출
	if (col > 0) FindMine(row, col - 1);
	// 오른쪽으로 재귀 호출
	if (col < COL - 1) FindMine(row, col + 1);
	// 위쪽으로 재귀 호출
	if (row > 0) FindMine(row - 1, col);
	// 아래쪽으로 재귀 호출
	if (row < ROW - 1) FindMine(row + 1, col);
	// 왼 위쪽으로 재귀 호출
	if (row > 0 && col > 0) FindMine(row - 1, col - 1);
	// 왼 아래쪽으로 재귀 호출
	if (row < ROW - 1 && col > 0) FindMine(row + 1, col - 1);
	// 오른 위쪽으로 재귀 호출
	if (row > 0 && col < COL -1) FindMine(row - 1, col + 1);
	// 오른 아래쪽으로 재귀 호출
	if (row < ROW -1 && col < COL - 1) FindMine(row + 1, col + 1);
}

