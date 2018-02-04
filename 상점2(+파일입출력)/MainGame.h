#pragma once

#include <Windows.h>
#include <conio.h>
#include "Shop2.h"
#include "Player2.h"
#include "Color.h"

// 위치
enum LOCATION {
	LOCATION_SHOP = 1,
	LOCATION_SELL_INVENTORY,
	LOCATION_INVENTORY
};

class MainGame : public Color
{
private:
	Shop2* m_shop;
	Player2* m_player;
	LOCATION m_location;

	int m_gold;
	int m_selectNum;
	int m_selectPtr;
public:
	MainGame();
	~MainGame();
	
	// 상점, 인벤토리 이동
	void SetLocation(int number);
	// 메인 화면
	void SetMainPage();
	// 상점 화면
	void SetShopPage();
	// 상점의 아이템을 보여주는 함수
	void SetShopItemPage(int itemKind);
	// 인벤토리 상점 판매를 보여주는 함수
	void SetSellInventoryPage();
	// 인벤토리 보여주는 함수 (장착용)
	void SetInventoryPage();
};

