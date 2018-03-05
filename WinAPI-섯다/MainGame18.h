#pragma once

#include "GameNode.h"

#define BG_SIZE 25
#define JOKBO_SIZE 32

enum CARD {
	CARD_1G, CARD_2A, CARD_3G, CARD_4A, CARD_5A,
	CARD_6A, CARD_7A, CARD_8G, CARD_9A, CARD_10A,
	CARD_1B, CARD_2B, CARD_3B, CARD_4B, CARD_5B,
	CARD_6B, CARD_7B, CARD_8B, CARD_9B, CARD_10B,
};

enum JOKBO {
	JB_38G, JB_18G, JB_13G, JB_1010, JB_99, JB_88,
	JB_77, JB_66, JB_55, JB_44, JB_33, JB_22, JB_11,
	JB_12, JB_14, JB_91, JB_101, JB_104, JB_46, JB_9,
	JB_8, JB_7, JB_6, JB_5, JB_4, JB_3, JB_2, JB_1, JB_0,
	JB_49, JB_NNC, JB_GC
};

struct tagPlayer {
	char* name;
	CARD card1, card2;
	JOKBO jokbo;
	long long bet;
	bool isWin;
};

class MainGame18 : public GameNode
{
private:
	Image * logo;
	Image * card;
	Image * bg[BG_SIZE];
	Image * jokbo[JOKBO_SIZE];

	CARD cardNum[20];
	tagPlayer player[5];

	RECT nextBtn;

	char str[128];

	int box;
	int winnerCount;

	int tableBet;
	int baseBet;
	int winBet;

	float moveFrame;
	float alpha;

	bool isJokbo;
	bool isLogo;
	bool isShuffle;
	bool isOver;
public:
	MainGame18();
	~MainGame18();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void StartGame();
	void EndGame();
	void Shuffle();
	JOKBO CheckJokbo(CARD card1, CARD card2);
};

