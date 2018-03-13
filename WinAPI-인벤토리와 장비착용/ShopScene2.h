#pragma once

#include "GameNode.h"
#include "Shop4.h"
#include "Inventory3.h"
#include "Player3.h"
#include "Item.h"

#define ITEMCOUNT 10

enum PlayerSTATUS {
	DOWN_WALK,
	UP_WALK,
	LEFT_WALK,
	RIGHT_WALK,
};

struct tagPlayer {
	float x, y;
	int gold;
};

struct tagShopInfo {
	RECT shopBox;
	tagItemInfo item;
	float x, y;
};

struct tagInvenInfo {
	RECT invenBox;
	tagItemInfo item;
	float x, y;
};

class ShopScene2 : public GameNode
{
private:
	Image * bg;
	Image * bgPixel;
	Image * character;

	Image * ball;
	Image * potion;
	Image * potion2;
	Image * booster;

	Image * menu;
	Image * inventory;
	Image * status;

	RECT shopRc;
	RECT invenRc;
	RECT shopperBox;
	RECT statusBox;

	Shop4 shop;
	tagShopInfo shopInfo[ITEMCOUNT];
	//Inventory3 inven;
	Player3 player;
	tagInvenInfo invenInfo[ITEMCOUNT];

	tagPlayer playerInfo;
	tagItemInfo currentItem;

	char str[128];
	RECT temp;
	float moveFrame;

	bool isDrag;
	bool isBuy;

	bool isDebug;
	bool showInven;
	bool showShop;
	bool showStatus;

	int width;
	int height;

	HFONT hFont;
	HFONT oldFont;
public:
	ShopScene2();
	~ShopScene2();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void Move();
	void UpdateItem();
};

