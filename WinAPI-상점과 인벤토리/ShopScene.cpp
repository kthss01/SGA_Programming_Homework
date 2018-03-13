#include "stdafx.h"
#include "ShopScene.h"


ShopScene::ShopScene()
{
}


ShopScene::~ShopScene()
{
}

HRESULT ShopScene::Init()
{
	isDebug = false;
	showInven = false;
	showShop = false;

	bg = new Image;
	bg->Init("images/shop.bmp", WINSIZEX, WINSIZEY);

	bgPixel = new Image;
	bgPixel->Init("images/shopPixel.bmp", WINSIZEX, WINSIZEY);

	player = new Image;
	player->Init("images/trainer.bmp", 210, 400, 3, 4, true, RGB(255, 200, 106));

	player->SetFrameX(1);
	player->SetFrameY(DOWN_WALK);

	ball = new Image;
	ball->Init("images/ball.bmp", 600, 50, 12, 1, true, RGB(0, 120, 192));

	potion = new Image;
	potion->Init("images/potion.bmp", 150, 50, 3, 1, true, RGB(0, 120, 192));

	potion2 = new Image;
	potion2->Init("images/potion2.bmp", 240, 50, 6, 1, true, RGB(0, 120, 192));

	menu = new Image;
	menu->Init("images/menu.bmp", 400, 500);

	inventory = new Image;
	inventory->Init("images/inventory.bmp", 400, 500);

	shop.ItemSetup();
	inven.ItemSetup();

	for (int i = 0; i < ITEMCOUNT; i++) {
		shopInfo[i].x = 90;
		shopInfo[i].y = 155 + 40 * i;
		shopInfo[i].shopBox = RectMake(
			shopInfo[i].x, shopInfo[i].y, 325, 40);
		shopInfo[i].item.itemKind = ITEM_EMPTY;

		invenInfo[i].x = 610;
		invenInfo[i].y = 155 + 40 * i;
		invenInfo[i].invenBox = RectMake(
			invenInfo[i].x, invenInfo[i].y, 325, 40);
		invenInfo[i].item.itemKind = ITEM_EMPTY;
	}

	UpdateItem();

	moveFrame = 1;

	playerInfo.x = 627;
	playerInfo.y = 258;
	playerInfo.gold = 10000;

	shopperBox = RectMake(480, 265, 50, 50);

	return S_OK;
}

void ShopScene::Release()
{
	SAFE_DELETE(bg);
	SAFE_DELETE(bgPixel);
	SAFE_DELETE(player);
	SAFE_DELETE(ball);
	SAFE_DELETE(potion);
	SAFE_DELETE(potion2);
	SAFE_DELETE(menu);
	SAFE_DELETE(inventory);
}

void ShopScene::Update()
{
	if(showShop == false && showInven == false)
		Move();
	
	// test
	//if (INPUT->GetKeyDown(VK_SPACE)) {
	//	//ball->SetFrameX(ball->GetFrameX() + 1);
	//	//potion->SetFrameX(potion->GetFrameX() + 1);
	//	//potion2->SetFrameX(potion2->GetFrameX() + 1);
	//}

	if (INPUT->GetKeyDown('I') && showShop == false) {
		showInven = !showInven;
	}


	if(INPUT->GetKeyDown(VK_SPACE) && 
		IntersectRect(&temp, &shopperBox, 
			&RectMake(playerInfo.x, playerInfo.y, 70, 100))) {
		playerInfo.x = 480;
		playerInfo.y = 255;
		player->SetFrameX(1);
		player->SetFrameY(LEFT_WALK);
		showInven = true;
		showShop = true;
	}

	if (INPUT->GetKeyDown(VK_LBUTTON) && showShop == true) {
		for (int i = 0; i < ITEMCOUNT; i++) {
			if (i == ITEMCOUNT - 1 &&
				PtInRect(&shopInfo[i].shopBox, g_ptMouse)) {
				showInven = false;
				showShop = false;
			}
			else if (PtInRect(&shopInfo[i].shopBox, g_ptMouse)) {
				if (shopInfo[i].item.itemKind == ITEM_EMPTY) continue;
				if(shopInfo[i].item.count > 0 && 
					shopInfo[i].item.price <= playerInfo.gold)
					inven.AddItem(shop.BuyItem(i, playerInfo.gold));
				UpdateItem();
			}

		}
	}

	if (INPUT->GetKeyDown(VK_RBUTTON) 
		&& showInven == true && showShop == true) {
		for (int i = 0; i < ITEMCOUNT; i++) {
			if (PtInRect(&invenInfo[i].invenBox, g_ptMouse)) {
				if (invenInfo[i].item.itemKind == ITEM_EMPTY) continue;
				inven.SellItem(i, playerInfo.gold);
				UpdateItem();
			}
		}
	}

	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
}

void ShopScene::Render()
{
	bg->Render(GetMemDC());
	//bgPixel->Render(GetMemDC());
	//player->Render(GetMemDC());
	player->FrameRender(GetMemDC(), playerInfo.x, playerInfo.y);

	// test
	//ball->Render(GetMemDC());
	//ball->FrameRender(GetMemDC(),0, 0);
	//potion->Render(GetMemDC(), 0, 50);
	//potion->FrameRender(GetMemDC(), 0, 50);
	//potion2->Render(GetMemDC());
	//potion2->FrameRender(GetMemDC(), 0, 0);
	//menu->Render(GetMemDC(),0, 0);
	//inventory->Render(GetMemDC(), 
	//	WINSIZEX - inventory->GetWidth(), 0);

	SetBkMode(GetMemDC(), TRANSPARENT);
	SetTextColor(GetMemDC(), RGB(0, 0, 0));

	if (showShop) {
		menu->AlphaRender(GetMemDC(), 50, 100, 225);
		sprintf_s(str, "상점");
		TextOut(GetMemDC(), 120, 110, str, strlen(str));
		sprintf_s(str, "구매/판매 마우스 L/R버튼");
		TextOut(GetMemDC(), 200, 110, str, strlen(str));
		for (int i = 0; i < ITEMCOUNT; i++) {
			if (shopInfo[i].item.itemKind == ITEM_EMPTY) continue;
			switch (shopInfo[i].item.itemKind)
			{
			case ITEM_MONSTERBALL:
				ball->FrameRender(GetMemDC(), shopInfo[i].x, shopInfo[i].y);
				break;
			case ITEM_POTION:
				potion->FrameRender(GetMemDC(), shopInfo[i].x, shopInfo[i].y);
				break;
			case ITEM_ANTIDOTE:
			case ITEM_PARLYZEHEAL:
			case ITEM_BURNHEAL:
			case ITEM_ICEHEAL:
			case ITEM_AWAKENING:
			case ITEM_FULLHEAL:
				potion2->FrameRender(GetMemDC(), shopInfo[i].x, shopInfo[i].y,
					shopInfo[i].item.itemKind - 3, 0);
				break;
			}
			TextOut(GetMemDC(), shopInfo[i].x + 50, shopInfo[i].y + 10,
				shopInfo[i].item.name.c_str(), strlen(shopInfo[i].item.name.c_str()));
			sprintf_s(str, "%d원", shopInfo[i].item.price);
			TextOut(GetMemDC(), shopInfo[i].x + 200, shopInfo[i].y + 10,
				str, strlen(str));
			if (shopInfo[i].item.count != 0)
				sprintf_s(str, "%d개", shopInfo[i].item.count);
			else
				sprintf_s(str, "매진");
			TextOut(GetMemDC(), shopInfo[i].x + 275, shopInfo[i].y + 10,
				str, strlen(str));
		}
	}

	if (showInven) {
		inventory->AlphaRender(GetMemDC(),
			WINSIZEX - inventory->GetWidth() - 50, 100, 225);

		sprintf_s(str, "인벤토리");
		TextOut(GetMemDC(), 670, 110, str, strlen(str));

		sprintf_s(str, "Gold : %d원", playerInfo.gold);
		TextOut(GetMemDC(), 820, 110, str, strlen(str));

		for (int i = 0; i < ITEMCOUNT; i++) {
			if (invenInfo[i].item.itemKind == ITEM_EMPTY) continue;
			switch (invenInfo[i].item.itemKind)
			{
			case ITEM_MONSTERBALL:
				ball->FrameRender(GetMemDC(), invenInfo[i].x, invenInfo[i].y);
				break;
			case ITEM_POTION:
				potion->FrameRender(GetMemDC(), invenInfo[i].x, invenInfo[i].y);
				break;
			case ITEM_ANTIDOTE:
			case ITEM_PARLYZEHEAL:
			case ITEM_BURNHEAL:
			case ITEM_ICEHEAL:
			case ITEM_AWAKENING:
			case ITEM_FULLHEAL:
				potion2->FrameRender(GetMemDC(), invenInfo[i].x, invenInfo[i].y,
					invenInfo[i].item.itemKind - 3, 0);
				break;
			}
			TextOut(GetMemDC(), invenInfo[i].x + 50, invenInfo[i].y + 10,
				invenInfo[i].item.name.c_str(), strlen(invenInfo[i].item.name.c_str()));
			sprintf_s(str, "%d원", invenInfo[i].item.price);
			TextOut(GetMemDC(), invenInfo[i].x + 200, invenInfo[i].y + 10,
				str, strlen(str));
			if (invenInfo[i].item.count != 0)
				sprintf_s(str, "%d개", invenInfo[i].item.count);
			TextOut(GetMemDC(), invenInfo[i].x + 275, invenInfo[i].y + 10,
				str, strlen(str));
		}
	}

	if (isDebug) {
		for (int i = 0; i < ITEMCOUNT; i++) {
			RectangleMake(GetMemDC(), shopInfo[i].shopBox);
			RectangleMake(GetMemDC(), invenInfo[i].invenBox);
		}
		RectangleMake(GetMemDC(), shopperBox);
	}
}

void ShopScene::Move()
{
	// left
	if (INPUT->GetKeyDown(VK_LEFT)) {
		moveFrame = 1;
		player->SetFrameY(LEFT_WALK);
	}
	if (INPUT->GetKey(VK_LEFT)) {
		moveFrame += 0.1f;
		playerInfo.x -= 2.5f;
		if ((int)moveFrame > 2)
			moveFrame = 0;
	}
	if (INPUT->GetKeyUp(VK_LEFT)) {
		moveFrame = 1;
	}

	// right
	if (INPUT->GetKeyDown(VK_RIGHT)) {
		moveFrame = 1;
		player->SetFrameY(RIGHT_WALK);
	}
	if (INPUT->GetKey(VK_RIGHT)) {
		moveFrame += 0.1f;
		playerInfo.x += 2.5f;
		if ((int)moveFrame > 2)
			moveFrame = 0;
	}
	if (INPUT->GetKeyUp(VK_RIGHT)) {
		moveFrame = 1;
	}

	// up
	if (INPUT->GetKeyDown(VK_UP)) {
		moveFrame = 1;
		player->SetFrameY(UP_WALK);
	}
	if (INPUT->GetKey(VK_UP)) {
		moveFrame += 0.1f;
		playerInfo.y -= 2.5f;
		if ((int)moveFrame > 2)
			moveFrame = 0;
	}
	if (INPUT->GetKeyUp(VK_UP)) {
		moveFrame = 1;
	}

	// down
	if (INPUT->GetKeyDown(VK_DOWN)) {
		moveFrame = 1;
		player->SetFrameY(DOWN_WALK);
	}
	if (INPUT->GetKey(VK_DOWN)) {
		moveFrame += 0.1f;
		playerInfo.y += 2.5f;
		if ((int)moveFrame > 2)
			moveFrame = 0;
	}
	if (INPUT->GetKeyUp(VK_DOWN)) {
		moveFrame = 1;
	}

	player->SetFrameX((int)moveFrame);
}

void ShopScene::UpdateItem()
{
	for (int i = 0; i < shop.GetItemCount(); i++) {
		shopInfo[i].item = shop.GetItem(i);
	}

	for (int i = 0; i < ITEMCOUNT; i++) {
		if (i < inven.GetItemCount()) {
			invenInfo[i].item = inven.GetItem(i);
		}
		else {
			invenInfo[i].item.itemKind = ITEM_EMPTY;
		}
	}
	
}
