#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

int Player::GetLevel()
{
	return level;
}

int * Player::GetCard()
{
	return card;
}

int Player::GetMoney()
{
	return money;
}

void Player::SetLevel(int level)
{
	this->level = level;
}

void Player::SetCard(int card1, int card2)
{
	// 첫번째 카드를 작은 카드로 하도록 하기 위해서
	if (card1 % 10 <= card2 % 10) {
		card[0] = card1;
		card[1] = card2;
	}
	else {
		card[0] = card2;
		card[1] = card1;
	}
}

void Player::SetMoney(int money)
{
	this->money = money;
}
