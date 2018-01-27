#pragma once
class Player
{
private:
	int level;		// 받은 화투패를 족보로 바꿨을때 등급 이 레벨로 승패 정함
	int card[2];	// 2장의 화투패
	int betting;	// 배팅금액
	int money;		// 소지금액
public:
	Player();
	~Player();

	int GetLevel();
	int* GetCard();
	int GetMoney();
	int GetBetting();

	void SetLevel(int level);
	void SetCard(int card1, int card2);
	void SetMoney(int money);
	bool SetBetting(int bet);
};

