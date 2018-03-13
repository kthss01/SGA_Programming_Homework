#pragma once

#include "GameNode.h"
#include "Shop3.h"
#include "Inventory2.h"
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

class ShopScene : public GameNode
{
private:
	Image * bg;
	Image * bgPixel;
	Image * player;
	
	Image * ball;
	Image * potion;
	Image * potion2;

	Image * menu;
	Image * inventory;

	RECT shopRc;
	RECT invenRc;
	RECT shopperBox;
	
	Shop3 shop;
	tagShopInfo shopInfo[ITEMCOUNT];
	Inventory2 inven;
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
public:
	ShopScene();
	~ShopScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void Move();
	void UpdateItem();
};

