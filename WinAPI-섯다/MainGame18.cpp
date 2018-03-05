#include "stdafx.h"
#include "MainGame18.h"


MainGame18::MainGame18()
{
}


MainGame18::~MainGame18()
{
}

HRESULT MainGame18::Init()
{
	GameNode::Init();

	logo = new Image;
	logo->Init("images/logo.bmp", WINSIZEX / 2 , WINSIZEY / 2);

	card = new Image;
	card->Init("images/card.bmp", WINSIZEX / 4 * 3 - 70, WINSIZEY / 4 + 20, 10, 2);


	for (int i = 0; i < BG_SIZE; i++) {
		bg[i] = new Image;
		if (i == 24)
			sprintf_s(str, "images/betting.bmp");
		else
			sprintf_s(str, "images/shuffle%d.bmp", i + 1);
		bg[i]->Init(str, WINSIZEX, WINSIZEY);
	}

	for (int i = 0; i < JOKBO_SIZE; i++) {
		jokbo[i] = new Image;
		// 38 광땡
		if (i == 0)
			sprintf_s(str, "images/jokbo (%d).bmp", 1);
		// 18 13 광땡
		else if (i <= 2)
			sprintf_s(str, "images/jokbo (%d).bmp", 2);
		// 땡
		else if (i <= 12)
			sprintf_s(str, "images/jokbo (%d).bmp", 3);
		// 갑오 전까지
		else if (i <= 19)
			sprintf_s(str, "images/jokbo (%d).bmp", i % 10 + 1);
		// 끗, 망통
		else if(i <= 28)
			sprintf_s(str, "images/jokbo (%d).bmp", 11);
		// 사구, 땡잡이, 암행어사
		else
			sprintf_s(str, "images/jokbo (%d).bmp", i-17);
		jokbo[i]->Init(str, WINSIZEX / 4 - 25, WINSIZEY / 4);
	}

	// logo를 보여줄지 말지
	isLogo = true;
	// logo의 투명성 0이 안보이는거 255 다보이는거
	alpha = 0;

	// 플레이어
	// int 형 범위 넘어가므로 long long (더 큰 정수형 범위 사용)
	// 참고 int 형 2^32 - 1 까지 약 10억 근데 이건 9999억 정도니
	player[0].bet = 999343891;
	player[0].name = (char*)"뭐가뭥 (나)";
	// test
	//player[0].card1 = CARD_1G;
	//player[0].card2 = CARD_7B;

	// test2
	//for (int i = 0; i < 5; i++) {
	//	player[i].card1 = (CARD)RND->GetInt(20);
	//	player[i].card2 = (CARD)RND->GetInt(20);
	//}

	player[1].bet = 1717448900;
	player[1].name = (char*)"천사수호";
	player[2].bet = 4007564530;
	player[2].name = (char*)"토끼와거북님";
	player[3].bet = 295010000;
	player[3].name = (char*)"꿀벌22";
	player[4].bet = 2436894732;
	player[4].name = (char*)"제우스0";

	// 게임 시작
	StartGame();

	// 게임이 끝났는지 확인
	isOver = false;

	nextBtn = RectMakeCenter(712, 732, 80, 50);

	return S_OK;
}

void MainGame18::Release()
{
	GameNode::Release();
	SAFE_DELETE(logo);
	SAFE_DELETE(card);
	for (int i = 0; i < BG_SIZE; i++) {
		SAFE_DELETE(bg[i]);
	}
	for (int i = 0; i < JOKBO_SIZE; i++) {
		SAFE_DELETE(jokbo[i]);
	}
}

void MainGame18::Update()
{
	GameNode::Update();

	// 처음 시작시 logo 부분 보이기 위한 방법
	if (isLogo && alpha != 255)
		alpha += 1;
	else {
		isLogo = false;
	}

	if (isLogo) return;

	// 배경 화면 애니메이션 제어
	if (moveFrame < BG_SIZE - 1)
		moveFrame += 0.1f;
	// 애니메이션이 끝나면 카드 나눠 줌
	else {
		isShuffle = true;
		isJokbo = true;
		if (!isOver)
			EndGame();
	}

	if (INPUT->GetKeyDown(VK_LBUTTON) && isOver)
	{
		if (PtInRect(&nextBtn, g_ptMouse)) {
			StartGame();
		}
	}

	// 애니메이션 test 부분
	{
		// 조작으로 테스트
		//if (INPUT->GetKeyDown(VK_CONTROL)) { isCombi = !isCombi; }

		//if (INPUT->GetKeyDown(VK_SPACE)) { moveFrame += 1.0f; }

		// BG test
		//if(moveFrame < BG_SIZE-1)
		//	moveFrame += 0.1f;

		// COMBI test
		//if (moveFrame < COMBI_SIZE - 1)
		//	moveFrame += 0.1f;

		//// card test
		//if (moveFrame < 20)
		//	moveFrame += 0.1f;
	}
}

void MainGame18::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	// 애니메이션 렌더 test
	{
		//logo->Render(memDC, WINSIZEX / 4, WINSIZEY / 4);
		// logo test
		//if(isLogo)
		//	logo->AlphaRender(memDC, WINSIZEX / 4, WINSIZEY / 4, alpha);
		//else
		//	bg[24]->Render(memDC);

		//card->Render(memDC, WINSIZEX / 4, WINSIZEY / 4);
		//bg[24]->Render(memDC);
		//// card 1 test
		//card->FrameRender(memDC, 
		//	WINSIZEX / 2 - WINSIZEX / 8 - 6, WINSIZEY - WINSIZEY / 6 - 13,
		//	(int)moveFrame % 10, (int) moveFrame / 10);
		//// card 2 test
		//card->FrameRender(memDC,
		//	WINSIZEX / 2 - WINSIZEX / 8 - 6 + card->GetFrameWidth(), WINSIZEY - WINSIZEY / 6 - 13,
		//	(int)moveFrame % 10, (int)moveFrame / 10);
		//bg[(int)moveFrame]->Render(memDC);
		//if(isCombi)
		//	combi[(int)moveFrame]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);

		// mouse x y
		//sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		//TextOut(memDC, 10, 10, str, strlen(str));

		// other card test
		/*card->FrameRender(memDC,
			38, 216 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			38 + card->GetFrameWidth(), 216 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			8, 417 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			8 + card->GetFrameWidth(), 417 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			864, 217 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			864 + card->GetFrameWidth(), 217 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);

		card->FrameRender(memDC,
			1017 - card->GetFrameWidth() * 2, 415 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);
		card->FrameRender(memDC,
			1017 - card->GetFrameWidth(), 415 - card->GetFrameHeight(),
			(int)moveFrame % 10, (int)moveFrame / 10);*/

		// money text test
		// player
		//sprintf_s(str, "%4d억 %4d만 %4d냥", 1234, 5678, 9999);
		//TextOut(memDC, 324, 161, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 58, 237, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 29, 440, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 832, 237, str, strlen(str));

		//sprintf_s(str, "9999억 9999만 9999냥");
		//TextOut(memDC, 862, 439, str, strlen(str));

		//// betting text test
		//sprintf_s(str, "%d만냥", 1234);
		//TextOut(memDC, 485, 501, str, strlen(str));

		//sprintf_s(str, "기본머니 %d만냥", 1000);
		//TextOut(memDC, 448, 550, str, strlen(str));

		// text test
		/*sprintf_s(str, "삼팔광땡");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 5,
			str, strlen(str));

		sprintf_s(str, " 여 덜 끗 ");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 65,
			str, strlen(str));
		
		sprintf_s(str, "   독  사   ");
		TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 115,
			str, strlen(str));*/
	}

	{
		// 로고 부분 그려주기
		if(isLogo)
			logo->AlphaRender(memDC, WINSIZEX / 4, WINSIZEY / 4, alpha);
		// 로고 부분 끝나고 나서 게임 부분 그리기
		else {
			// 배경 화면 그리기
			bg[(int)moveFrame]->Render(memDC);
			
			// 배팅액 그리기
			{
				// player들 배팅액
				// 본인
				sprintf_s(str, "%4d억 %4d만 %4d냥",
					// 억 자리
					player[0].bet / 100000000,
					// 만 자리
					(player[0].bet / 10000) % 10000,
					// 냥 자리
					player[0].bet % 10000);
				TextOut(memDC, 324, 161, str, strlen(str));

				sprintf_s(str, "%4d억 %4d만 %4d냥",
					// 억 자리
					player[1].bet / 100000000,
					// 만 자리
					(player[1].bet / 10000) % 10000,
					// 냥 자리
					player[1].bet % 10000);
				TextOut(memDC, 58, 237, str, strlen(str));

				sprintf_s(str, "%4d억 %4d만 %4d냥",
					// 억 자리
					player[2].bet / 100000000,
					// 만 자리
					(player[2].bet / 10000) % 10000,
					// 냥 자리
					player[2].bet % 10000);
				TextOut(memDC, 29, 440, str, strlen(str));

				sprintf_s(str, "%4d억 %4d만 %4d냥",
					// 억 자리
					player[3].bet / 100000000,
					// 만 자리
					(player[3].bet / 10000) % 10000,
					// 냥 자리
					player[3].bet % 10000);
				TextOut(memDC, 832, 237, str, strlen(str));

				sprintf_s(str, "%4d억 %4d만 %4d냥",
					// 억 자리
					player[4].bet / 100000000,
					// 만 자리
					(player[4].bet / 10000) % 10000,
					// 냥 자리
					player[4].bet % 10000);
				TextOut(memDC, 862, 439, str, strlen(str));

				// 배팅액 부분
				sprintf_s(str, "%d만냥", tableBet);
				TextOut(memDC, 485, 501, str, strlen(str));

				sprintf_s(str, "기본머니 %d만냥", baseBet);
				TextOut(memDC, 448, 550, str, strlen(str));
			}

			// 카드 그리기
			if (isShuffle) {
				// 본인 카드
				// card 1
				card->FrameRender(memDC, 
					WINSIZEX / 2 - WINSIZEX / 8 - 6, WINSIZEY - WINSIZEY / 6 - 13,
					player[0].card1 % 10, player[0].card1 / 10);
				// card 2
				card->FrameRender(memDC,
					WINSIZEX / 2 - WINSIZEX / 8 - 6 + card->GetFrameWidth(), WINSIZEY - WINSIZEY / 6 - 13,
					player[0].card2 % 10, player[0].card2 / 10);

				// 나머지 플레이어
				card->FrameRender(memDC,
				38, 216 - card->GetFrameHeight(),
					player[1].card1 % 10, player[1].card1 / 10);
				card->FrameRender(memDC,
				38 + card->GetFrameWidth(), 216 - card->GetFrameHeight(),
					player[1].card2 % 10, player[1].card2 / 10);

				card->FrameRender(memDC,
				8, 417 - card->GetFrameHeight(),
					player[2].card1 % 10, player[2].card1 / 10);
				card->FrameRender(memDC,
				8 + card->GetFrameWidth(), 417 - card->GetFrameHeight(),
					player[2].card2 % 10, player[2].card2 / 10);

				card->FrameRender(memDC,
				864, 217 - card->GetFrameHeight(),
					player[3].card1 % 10, player[3].card1 / 10);
				card->FrameRender(memDC,
				864 + card->GetFrameWidth(), 217 - card->GetFrameHeight(),
					player[3].card2 % 10, player[3].card2 / 10);

				card->FrameRender(memDC,
				1017 - card->GetFrameWidth() * 2, 415 - card->GetFrameHeight(),
					player[4].card1 % 10, player[4].card1 / 10);
				card->FrameRender(memDC,
				1017 - card->GetFrameWidth(), 415 - card->GetFrameHeight(),
					player[4].card2 % 10, player[4].card2 / 10);
			}

			// 족보 그리기
			if (isJokbo) {
				// 본인 족보
				switch (player[0].jokbo)
				{
				case JB_38G:
					sprintf_s(str, "삼팔광땡");
					jokbo[0]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_18G:
					sprintf_s(str, "일팔광땡");
					jokbo[1]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_13G:
					sprintf_s(str, "일삼광땡");
					jokbo[1]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_1010:
					sprintf_s(str, "   장  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_99:
					sprintf_s(str, "   구  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_88:
					sprintf_s(str, "   팔  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_77:
					sprintf_s(str, "   칠  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_66:
					sprintf_s(str, "   육  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_55:
					sprintf_s(str, "   오  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_44:
					sprintf_s(str, "   사  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_33:
					sprintf_s(str, "   삼  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_22:
					sprintf_s(str, "   이  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_11:
					sprintf_s(str, "   삥  땡   ");
					jokbo[2]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_12:
					sprintf_s(str, "   알  리   ");
					jokbo[3]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_14:
					sprintf_s(str, "   독  사   ");
					jokbo[4]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_91:
					sprintf_s(str, "   구  삥   ");
					jokbo[5]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_101:
					sprintf_s(str, "   장  삥   ");
					jokbo[6]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_104:
					sprintf_s(str, "   장  사   ");
					jokbo[7]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_46:
					sprintf_s(str, "   세  륙   ");
					jokbo[8]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_9:
					sprintf_s(str, "   갑  오   ");
					jokbo[9]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_8:
					sprintf_s(str, " 여 덜 끗 ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_7:
					sprintf_s(str, " 일 곱 끗 ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_6:
					sprintf_s(str, " 여 섯 끗 ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_5:
					sprintf_s(str, " 다 섯 끗 ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_4:
					sprintf_s(str, "   네  끗   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_3:
					sprintf_s(str, "   세  끗   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_2:
					sprintf_s(str, "   두  끗   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_1:
					sprintf_s(str, "   한  끗   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_0:
					sprintf_s(str, "   망  통   ");
					jokbo[10]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_49:
					sprintf_s(str, "   사  구   ");
					jokbo[11]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_NNC:
					sprintf_s(str, " 땡 잡 이 ");
					jokbo[12]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				case JB_GC:
					sprintf_s(str, "암행어사");
					jokbo[13]->Render(memDC, 0, WINSIZEY - WINSIZEY / 4);
					break;
				}

				TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 8 - 6 + 35, WINSIZEY - WINSIZEY / 6 - 13 + 115,
					str, strlen(str));
			}

			// 승패 그리기
			if (isOver) {
				HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
					VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
				HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

				int pad = 25;

				if (winnerCount > 1) {
					sprintf_s(str, "무승부");
					TextOut(memDC, WINSIZEX / 2 - 35, WINSIZEY / 2, str, strlen(str));

					sprintf_s(str, "+%d만냥", baseBet);
					TextOut(memDC, 324, 161 + pad, str, strlen(str));
					TextOut(memDC, 58, 237 + pad, str, strlen(str));
					TextOut(memDC, 29, 440 + pad, str, strlen(str));
					TextOut(memDC, 832, 237 + pad, str, strlen(str));
					TextOut(memDC, 862, 439 + pad, str, strlen(str));
				}
				else {
					for (int i = 0; i < 5; i++) {
						if (player[i].isWin) {
							sprintf_s(str, "%s 승리", player[i].name);
							TextOut(memDC, WINSIZEX / 2 - WINSIZEX / 12, WINSIZEY / 2, 
								str, strlen(str));
							switch (i) {
							case 0:
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d만냥", tableBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 1:
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d만냥", tableBet);
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 2:
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d만냥", tableBet);
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 3:
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d만냥", tableBet);
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(255, 0, 0));
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							case 4:
								sprintf_s(str, "-%d만냥", baseBet);
								TextOut(memDC, 324, 161 + pad, str, strlen(str));
								TextOut(memDC, 58, 237 + pad, str, strlen(str));
								TextOut(memDC, 29, 440 + pad, str, strlen(str));
								TextOut(memDC, 832, 237 + pad, str, strlen(str));
								SetTextColor(memDC, RGB(0, 0, 255));
								sprintf_s(str, "+%d만냥", tableBet);
								TextOut(memDC, 862, 439 + pad, str, strlen(str));
								break;
							}
						}
					}
				}
				
				// 다시 원래대로
				SetTextColor(memDC, RGB(0, 0, 0));

				// 게임이 끝나면 Next 버튼 그리기
				RectangleMake(memDC, nextBtn);

				sprintf_s(str, "Next");
				TextOut(memDC, 685, 720, str, strlen(str));

				SelectObject(memDC, OldFont);
				DeleteObject(hFont);
			}
		}
	
		// test
		//sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		//TextOut(memDC, 10, 10, str, strlen(str));
	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame18::StartGame()
{

	// 프레임 제어 변수
	moveFrame = 0;
	// 카드가 셔플 되었는지 확인하는 변수 (카드가 다 나눠졌는지)
	isShuffle = false;
	// 해당 족보를 보일지 말지
	isJokbo = false;
	// 게임 오버도 초기화
	isOver = false;

	// 모두 패배로 시작
	for (int i = 0; i < 5; i++) {
		player[i].isWin = false;
	}

	// 배팅액
	baseBet = 1000;
	tableBet = 0;
	for (int i = 0; i < 5; i++) {
		player[i].bet -= baseBet;
		tableBet += baseBet;
	}

	// 카드 섞기
	Shuffle();
	// 섞은 카드 나눠주기
	// 카드 테스트
	//player[0].card1 = CARD_4A;
	//player[0].card2 = CARD_4B;
	//for (int i = 1; i < 5; i++) {
	//	player[i].card1 = cardNum[i];
	//	player[i].card2 = cardNum[i + 5];
	//}

	//// 카드 테스트 2 땡잡이
	//player[0].card1 = CARD_4A;
	//player[0].card2 = CARD_4B;
	//player[1].card1 = CARD_3B;
	//player[1].card2 = CARD_7A;
	//for (int i = 2; i < 5; i++) {
	//	player[i].card1 = cardNum[i];
	//	player[i].card2 = cardNum[i + 5];
	//}

	// 섞은 카드 나눠주기
	for (int i = 0; i < 5; i++) {
		player[i].card1 = cardNum[i];
		player[i].card2 = cardNum[i + 5];
	}

	// 족보 체크
	for (int i = 0; i < 5; i++) {
		player[i].jokbo = CheckJokbo(player[i].card1, player[i].card2);
	}
}

void MainGame18::EndGame()
{
	// 승패 계산
	// 특수 족보 계산
	for (int i = 0; i < 5; i++) {
		// 사구
		if (player[i].jokbo == JB_49) {
			// 무승부로 처리 무승부는 모두 win으로 표시
			for (int j = 0; j < 5; j++) {
				player[j].isWin = true;
			}
			break;
		}
		// 땡잡이
		if (player[i].jokbo == JB_NNC) {
			for (int j = 0; j < 5; j++) {
				// 자신 제외
				if (i == j) continue;
				// 땡이 있는 경우
				if (player[j].jokbo >= JB_1010 && player[j].jokbo <= JB_11) {
					// 이김
					player[i].isWin = true;
					break;
				}
			}
			// 땡을 못찾은 경우 망통으로 변함
			if (player[i].isWin == false) {
				player[i].jokbo = JB_0;
			}
			else
				break;
		}
		// 암행어사
		if (player[i].jokbo == JB_GC) {
			for (int j = 0; j < 5; j++) {
				// 자신 제외
				if (i == j) continue;
				// 광땡이 있는 경우 38광땡 제외
				if (player[j].jokbo > JB_38G && player[j].jokbo <= JB_13G) {
					// 이김
					player[i].isWin = true;
					break;
				}
			}
			// 광땡을 못찾은 경우 한끗으로 변함
			if (player[i].isWin == false) {
				player[i].jokbo = JB_1;
			}
			else
				break;
		}
	}

	// 특수 족보에 의해 이겼는지 확인
	isOver = false;
	for (int i = 0; i < 5; i++) {
		if (player[i].isWin == true) {
			isOver = true;
			break;
		}
	}
	// 끝나지 않은 경우 족보 계산
	if (!isOver) {
		// 족보 계산되므로 끝남
		isOver = true;
		box = JB_0;
		for (int i = 0; i < 5; i++) {
			if (box > player[i].jokbo) {
				box = player[i].jokbo;
			}
		}
		for (int i = 0; i < 5; i++) {
			if (box == player[i].jokbo) {
				player[i].isWin = true;
			}
		}
	}
	
	// 끝난 경우 배팅액 계산
	// 무승부 체크
	winnerCount = 0;;
	// 우승한 사람이 둘 이상이면 무승부 처리 
	for (int i = 0; i < 5; i++) {
		if (player[i].isWin == true) {
			winnerCount++;
		}
	}
	// 무승부인 경우 배팅액 돌려주기
	if (winnerCount > 1) {
		for (int i = 0; i < 5; i++) {
			player[i].bet += baseBet;
			tableBet -= baseBet;
		}
	}
	// 승부가 난 경우 승자에게 배팅액 몰아주기
	else {
		for (int i = 0; i < 5; i++) {
			if (player[i].isWin == true) {
				player[i].bet += tableBet;
				//tableBet = 0;
			}
		}
	}
}

void MainGame18::Shuffle()
{
	int dest, sour;
	CARD temp;

	// 초기화
	for (int i = 0; i < 20; i++)
		cardNum[i] = CARD(i);

	for (int i = 0; i < 1000; i++) {
		dest = RND->GetInt(20);
		sour = RND->GetInt(20);

		temp = cardNum[dest];
		cardNum[dest] = cardNum[sour];
		cardNum[sour] = temp;
	}
}

JOKBO MainGame18::CheckJokbo(CARD card1, CARD card2)
{
	CARD temp;
	// 작은 카드숫자가 card1로 오게 끔 swap -> 결론 : card1 < card2
	if (card1 > card2) {
		temp = card1;
		card1 = card2;
		card2 = temp;
	}
	if (card1 == CARD_3G && card2 == CARD_8G) {
		return JB_38G;
	}
	else if (card1 == CARD_1G && card2 == CARD_8G) {
		return JB_18G;
	}
	else if (card1 == CARD_1G && card2 == CARD_3G) {
		return JB_13G;
	}
	else if (card1 == CARD_10A && card2 == CARD_10B) {
		return JB_1010;
	}
	else if (card1 == CARD_9A && card2 == CARD_9B) {
		return JB_99;
	}
	else if (card1 == CARD_8G && card2 == CARD_8B) {
		return JB_88;
	}
	else if (card1 == CARD_7A && card2 == CARD_7B) {
		return JB_77;
	}
	else if (card1 == CARD_6A && card2 == CARD_6B) {
		return JB_66;
	}
	else if (card1 == CARD_5A && card2 == CARD_5B) {
		return JB_55;
	}
	else if (card1 == CARD_4A && card2 == CARD_4B) {
		return JB_44;
	}
	else if (card1 == CARD_3G && card2 == CARD_3B) {
		return JB_33;
	}
	else if (card1 == CARD_2A && card2 == CARD_2B) {
		return JB_22;
	}
	else if (card1 == CARD_1G && card2 == CARD_1B) {
		return JB_11;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_2A ||
		card1 == CARD_1G && card2 == CARD_2B ||
		card1 == CARD_2A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_2B) {
		return JB_12;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_4A ||
		card1 == CARD_1G && card2 == CARD_4B ||
		card1 == CARD_4A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_4B) {
		return JB_14;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_9A ||
		card1 == CARD_1G && card2 == CARD_9B ||
		card1 == CARD_9A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_9B) {
		return JB_91;
	}
	else if (
		card1 == CARD_1G && card2 == CARD_10A ||
		card1 == CARD_1G && card2 == CARD_10B ||
		card1 == CARD_10A && card2 == CARD_1B ||
		card1 == CARD_1B && card2 == CARD_10B) {
		return JB_101;
	}
	else if (
		card1 == CARD_4A && card2 == CARD_10A ||
		card1 == CARD_4A && card2 == CARD_10B ||
		card1 == CARD_10A && card2 == CARD_4B ||
		card1 == CARD_4B && card2 == CARD_10B) {
		return JB_104;
	}
	else if (
		card1 == CARD_4A && card2 == CARD_6A ||
		card1 == CARD_4A && card2 == CARD_6B ||
		card1 == CARD_6A && card2 == CARD_4B ||
		card1 == CARD_4B && card2 == CARD_6B) {
		return JB_46;
	}
	// 사구
	else if (card1 == CARD_4A && card2 == CARD_9A) {
		return JB_49;
	}
	// 땡잡이
	else if (
		card1 == CARD_3G && card2 == CARD_7A ||
		card1 == CARD_3G && card2 == CARD_7B ||
		card1 == CARD_7A && card2 == CARD_3B ||
		card1 == CARD_7A && card2 == CARD_3B) {
		return JB_NNC;
	}
	// 암행어사
	else if (card1 == CARD_4A && card2 == CARD_7A) {
		return JB_GC;
	}
	else {
		int a, b, c;
		JOKBO jok;
		a = card1 + 1;
		b = card2 + 1;
		jok = JOKBO(JB_0 - (a + b) % 10);
		return jok;
	}
}
