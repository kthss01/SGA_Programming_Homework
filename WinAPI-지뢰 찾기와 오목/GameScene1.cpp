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
	board->Init("images/minesweeper/board.bmp", WINSIZEX, WINSIZEY, true);

	for (int i = 0; i < 6; i++) {
		number[i] = new Image;
		number[i]->Init("images/minesweeper/number.bmp", 450, 80, 10, 1, true);
	}

	ChangeNumber(true, MINECOUNT);
	flagCount = 0;
	ChangeNumber(false, MINECOUNT - flagCount);

	sizeX = FIELDX / ROW;
	sizeY = FIELDY / COL;

	// 결과 이미지 읽기
	result = new Image;
	result->Init("images/minesweeper/result.bmp", 240, 106, 4, 1, true);

	// 마인 이미지 읽기
	for (int i = 0; i < 9; i++) {
		mine[i] = new Image;
		// 숫자
		if (i < 4) {
			sprintf_s(str, "images/minesweeper/%d.bmp", i + 1);
		}
		else {
			switch (i) {
			// boom
			case 4:
				sprintf_s(str, "images/minesweeper/boom.bmp");
				break;
			case 5:
				sprintf_s(str, "images/minesweeper/empty.bmp");
				break;
			case 6:
				sprintf_s(str, "images/minesweeper/flag.bmp");
				break;
			case 7:
				sprintf_s(str, "images/minesweeper/mine.bmp");
				break;
			case 8:
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
			mineInfo[i][j].x = 25 + sizeX * j;
			mineInfo[i][j].y = 256 + sizeY * i;
			mineInfo[i][j].isFlag = false;
			mineInfo[i][j].nearMineCount = 0;
			mineInfo[i][j].rc = RectMake(
				mineInfo[i][j].x, mineInfo[i][j].y,
				sizeX, sizeY);
		}
	}
	
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

	return S_OK;
}

void GameScene1::Release()
{
	SAFE_DELETE(board);
	for (int i = 0; i < 6; i++)
		SAFE_DELETE(number[i]);
	for (int i = 0; i < 9; i++)
		SAFE_DELETE(mine[i]);
	SAFE_DELETE(result);
}

void GameScene1::Update()
{
	// 마우스 클릭 시
	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++) {
				if (PtInRect(&mineInfo[i][j].rc, g_ptMouse)
					&& mineInfo[i][j].isFlag == false) {
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
	if (INPUT->GetKeyUp(VK_LBUTTON)) {
		result->SetFrameX(0);
	}

	if (INPUT->GetKeyDown(VK_RBUTTON)) {
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (PtInRect(&mineInfo[i][j].rc, g_ptMouse)) {
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
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void GameScene1::Render()
{
	board->Render(GetMemDC());

	// 결과 그리기
	result->FrameRender(GetMemDC(), 226, 75);

	// 숫자 그리기
	//number->Render(GetMemDC());
	for (int i = 0; i < 6; i++) {
		// 화면 왼쪽 실제 지뢰 갯수
		if(i < 3)
		number[i]->FrameRender(GetMemDC(), 
			55 + i * (number[i]->GetFrameWidth() + 5), 90);
		// 화면 오른쪽 지뢰 갯수 - 깃발 꼿은 갯수
		else
		number[i]->FrameRender(GetMemDC(),
			312 + (i-3) * (number[i]->GetFrameWidth() + 5), 90);
	}

	// 지뢰 그리기
	//for (int i = 0; i < 9; i++) {
	//	mine[i]->Render(GetMemDC(), 200, 200 + 50*i);
	//}

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
	}
	//=================================================
}

void GameScene1::ChangeNumber(bool mine, int number)
{
	if (mine) {
		this->number[0]->SetFrameX(number / 100);
		this->number[1]->SetFrameX(number / 10 % 10);
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
		}
	}
}

void GameScene1::GameOver()
{
	result->SetFrameX(2);

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (mineInfo[i][j].isMine && mineInfo[i][j].status == STATUS_NCB)
				mineInfo[i][j].status = STATUS_MINE;
		}
	}

	KillTimer(g_hWnd, 1);
}

void GameScene1::GameClear()
{
	result->SetFrameX(3);
	
	KillTimer(g_hWnd, 1);
}

void GameScene1::FindMine(int row, int col)
{
	if (mineInfo[row][col].isMine ||
		mineInfo[row][col].status != STATUS_NCB) {
		return;
	}
	else {
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
		}
	}

	if (row > 0) FindMine(row - 1, col);
	if (row < ROW - 1) FindMine(row + 1, col);
	if (col > 0) FindMine(row, col - 1);
	if (col < COL - 1) FindMine(row, col + 1);
}

