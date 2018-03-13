#include "stdafx.h"
#include "ShopScene2.h"


ShopScene2::ShopScene2()
{
}


ShopScene2::~ShopScene2()
{
}

HRESULT ShopScene2::Init()
{
	isDebug = false;
	showInven = false;
	showShop = false;
	showStatus = false;
	isDrag = false;
	isBuy = false;

	bg = new Image;
	bg->Init("images/shop.bmp", WINSIZEX, WINSIZEY);

	bgPixel = new Image;
	bgPixel->Init("images/shopPixel.bmp", WINSIZEX, WINSIZEY);

	character = new Image;
	character->Init("images/trainer.bmp", 210, 400, 3, 4, true, RGB(255, 200, 106));

	character->SetFrameX(1);
	character->SetFrameY(DOWN_WALK);

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

	status = new Image;
	status->Init("images/status.bmp", 500, 500);

	booster = new Image;
	booster->Init("images/battleBooster.bmp", 350, 50, 7, 1, true, RGB(0, 120, 192));

	shop.ItemSetup();
	player.InputPlayer();
	player.ItemSetup();
	//inven.ItemSetup();

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

	shopRc = RectMake(50, 100, 400, 500);
	invenRc = RectMake(
		WINSIZEX - inventory->GetWidth() - 50, 100, 400, 500);
	shopperBox = RectMake(480, 265, 50, 50);
	statusBox = RectMake(68, 214, 50, 50);

	width = 10;
	height = 10;

	return S_OK;
}

void ShopScene2::Release()
{
	SAFE_DELETE(bg);
	SAFE_DELETE(bgPixel);
	SAFE_DELETE(character);
	SAFE_DELETE(ball);
	SAFE_DELETE(potion);
	SAFE_DELETE(potion2);
	SAFE_DELETE(menu);
	SAFE_DELETE(inventory);
}

void ShopScene2::Update()
{
	if (showShop == false && showInven == false)
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

	if (INPUT->GetKeyDown('C') && showShop == false) {
		showStatus = !showStatus;
	}


	if (INPUT->GetKeyDown(VK_SPACE) &&
		IntersectRect(&temp, &shopperBox,
			&RectMake(playerInfo.x, playerInfo.y, 70, 100))) {
		playerInfo.x = 480;
		playerInfo.y = 255;
		character->SetFrameX(1);
		character->SetFrameY(LEFT_WALK);
		showStatus = false;
		showInven = true;
		showShop = true;
	}

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		if (showStatus && showInven) {
			if (PtInRect(&statusBox, g_ptMouse)) {
				isDrag = true;
				currentItem = player.GetEquipItem();
			}

			for (int i = 0; i < ITEMCOUNT; i++) {
				if (PtInRect(&invenInfo[i].invenBox, g_ptMouse)) {
					if (invenInfo[i].item.itemKind >= BOOSTER_CRITICALCUTTER) {
						isDrag = true;
						currentItem = invenInfo[i].item;
					}
				}
			}
		}

		if (showShop) {
			for (int i = 0; i < ITEMCOUNT; i++) {
				if (i == ITEMCOUNT - 1 &&
					PtInRect(&shopInfo[i].shopBox, g_ptMouse)) {
					showInven = false;
					showShop = false;
				}
				else if (PtInRect(&shopInfo[i].shopBox, g_ptMouse)) {
					if (shopInfo[i].item.itemKind == ITEM_EMPTY) continue;
					if (shopInfo[i].item.count > 0 &&
						shopInfo[i].item.price <= playerInfo.gold)
						player.AddItem(shop.BuyItem(i, playerInfo.gold));
					UpdateItem();
				}
			}
		}
	}

	if (INPUT->GetKeyUp(VK_LBUTTON) && showStatus && showInven) {
		if (PtInRect(&invenRc, g_ptMouse) &&
			currentItem.name.compare(player.GetEquipItem().name) == 0) {
			player.EquipItem(-1);
			UpdateItem();
		}

		if (PtInRect(&statusBox, g_ptMouse)) {
			for (int i = 0; i < ITEMCOUNT; i++) {
				if (invenInfo[i].item.name.compare(currentItem.name) == 0) {
					player.EquipItem(i);
					UpdateItem();
					break;
				}
			}
		}

		isDrag = false;
	}

	if (INPUT->GetKeyDown(VK_RBUTTON) 
		&& showInven == true && showShop == true) {
		for (int i = 0; i < ITEMCOUNT; i++) {
			if (PtInRect(&invenInfo[i].invenBox, g_ptMouse)) {
				if (invenInfo[i].item.itemKind == ITEM_EMPTY) continue;
				// 부스터는 팔지 못하게
				if (shopInfo[i].item.itemKind >= BOOSTER_CRITICALCUTTER &&
					shopInfo[i].item.itemKind <= BOOSTER_SPEEDUP) continue;
				shop.AddItem(player.SellItem(i, playerInfo.gold));
				UpdateItem();
			}
		}
	}

	//// 마우스 드래그로 구매 및 판매
	//if (INPUT->GetKeyDown(VK_LBUTTON) && showShop) {
	//	// 구매 스타트
	//	for (int i = 0; i < ITEMCOUNT; i++) {
	//		if (i == ITEMCOUNT - 1 &&
	//			PtInRect(&shopInfo[i].shopBox, g_ptMouse)) {
	//			showInven = false;
	//			showShop = false;
	//		}
	//		else if (PtInRect(&shopInfo[i].shopBox, g_ptMouse)) {
	//			if (shopInfo[i].item.itemKind == ITEM_EMPTY) continue;
	//			isDrag = true;
	//			isBuy = true;
	//			currentItem = shopInfo[i].item;
	//			currentItem.count = 1;
	//		}
	//	}

	//	// 판매 스타트
	//	for (int i = 0; i < ITEMCOUNT; i++) {
	//		if (PtInRect(&invenInfo[i].invenBox, g_ptMouse)) {
	//			if (invenInfo[i].item.itemKind == ITEM_EMPTY) continue;
	//			isDrag = true;
	//			isBuy = false;
	//			currentItem = invenInfo[i].item;
	//		}
	//	}
	//}

	//if (INPUT->GetKeyUp(VK_LBUTTON) && showShop) {
	//	// 구매 끝
	//	if (isBuy && PtInRect(&invenRc, g_ptMouse)) {
	//		if (currentItem.count > 0
	//			&& playerInfo.gold >= currentItem.price) {
	//			inven.AddItem(currentItem);
	//			for (int i = 0; i < ITEMCOUNT; i++) {
	//				if (currentItem.name.compare(shopInfo[i].item.name) == 0) {
	//					shop.BuyItem(i, playerInfo.gold);
	//					break;
	//				}
	//			}
	//			UpdateItem();
	//		}
	//	}
	//	// 판매 끝
	//	else if (!isBuy && PtInRect(&shopRc, g_ptMouse)) {
	//		for (int i = 0; i < ITEMCOUNT; i++) {
	//			if (currentItem.name.compare(invenInfo[i].item.name) == 0) {
	//				shop.AddItem(inven.SellItem(i, playerInfo.gold));
	//				break;
	//			}
	//		}
	//		UpdateItem();
	//	}
	//	isDrag = false;
	//}



	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
}

void ShopScene2::Render()
{
	bg->Render(GetMemDC());
	//bg->Render(GetMemDC(), 0, 0, 0, 0, width, height);
	//bgPixel->Render(GetMemDC());
	//player->Render(GetMemDC());
	character->FrameRender(GetMemDC(), playerInfo.x, playerInfo.y);

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

	if (showStatus) {
		BeginCreateFont(GetMemDC(), &hFont, 30);
		oldFont = (HFONT)SelectObject(GetMemDC(), hFont);

		SetTextColor(GetMemDC(), RGB(255, 255, 255));
		status->AlphaRender(GetMemDC(), 50, 100, 225);

		sprintf_s(str, "%d", player.GetLevel());
		TextOut(GetMemDC(), 85, 162, str, strlen(str));

		sprintf_s(str, "%s", player.GetName().c_str());
		TextOut(GetMemDC(), 140, 162, str, strlen(str));

		SetTextColor(GetMemDC(), RGB(0, 0, 0));
		sprintf_s(str, "%d / %d", 
			player.GetHp(), player.GetMaxHp());
		TextOut(GetMemDC(), 430, 165, str, strlen(str));

		sprintf_s(str, "%d", player.GetAtk());
		TextOut(GetMemDC(), 495, 225, str, strlen(str));

		sprintf_s(str, "%d", player.GetDef());
		TextOut(GetMemDC(), 495, 265, str, strlen(str));

		sprintf_s(str, "%d", player.GetSpAtk());
		TextOut(GetMemDC(), 495, 305, str, strlen(str));

		sprintf_s(str, "%d", player.GetSpDef());
		TextOut(GetMemDC(), 495, 345, str, strlen(str));

		sprintf_s(str, "%d", player.GetSpeed());
		TextOut(GetMemDC(), 495, 385, str, strlen(str));

		sprintf_s(str, "%8d", player.GetExp());
		TextOut(GetMemDC(), 430, 425, str, strlen(str));

		sprintf_s(str, "%8d", player.GetMaxExp());
		TextOut(GetMemDC(), 430, 465, str, strlen(str));

		tagItemInfo tempItem = player.GetEquipItem();

		SetTextColor(GetMemDC(), RGB(255, 0, 0));

		if (tempItem.itemKind != ITEM_EMPTY) {
			switch (tempItem.itemKind) {
			case BOOSTER_PLUSPOWER:
				sprintf_s(str, "+ %d", tempItem.attribute);
				TextOut(GetMemDC(), 405, 225, str, strlen(str));
				booster->FrameRender(GetMemDC(), 68, 214,
					tempItem.itemKind - 9, 0);
				break;
			case BOOSTER_DEFENDUP:
				sprintf_s(str, "+ %d", tempItem.attribute);
				TextOut(GetMemDC(), 405, 265, str, strlen(str));
				booster->FrameRender(GetMemDC(), 68, 214,
					tempItem.itemKind - 9, 0);
				break;
			case BOOSTER_SPECIALUP:
				sprintf_s(str, "+ %d", tempItem.attribute);
				TextOut(GetMemDC(), 405, 305, str, strlen(str));
				booster->FrameRender(GetMemDC(), 68, 214,
					tempItem.itemKind - 9, 0);
				break;
			case BOOSTER_SPECIALGUARD:
				sprintf_s(str, "+ %d", tempItem.attribute);
				TextOut(GetMemDC(), 405, 345, str, strlen(str));
				booster->FrameRender(GetMemDC(), 68, 214,
					tempItem.itemKind - 9, 0);
				break;
			case BOOSTER_SPEEDUP:
				sprintf_s(str, "+ %d", tempItem.attribute);
				TextOut(GetMemDC(), 405, 385, str, strlen(str));
				booster->FrameRender(GetMemDC(), 68, 214,
					tempItem.itemKind - 9, 0);
				break;
			case BOOSTER_CRITICALCUTTER:
			case BOOSTER_EFFECTGUARD:
				booster->FrameRender(GetMemDC(), 68, 214,
					tempItem.itemKind - 9, 0);
				break;
			}
		}

		SelectObject(GetMemDC(), oldFont);
		DeleteObject(hFont);
	}

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
			case BOOSTER_PLUSPOWER:
			case BOOSTER_DEFENDUP:
			case BOOSTER_SPECIALUP:
			case BOOSTER_SPEEDUP:
			case BOOSTER_SPECIALGUARD:
			case BOOSTER_CRITICALCUTTER:
			case BOOSTER_EFFECTGUARD:
				booster->FrameRender(GetMemDC(), shopInfo[i].x, shopInfo[i].y,
					shopInfo[i].item.itemKind - 9, 0);
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
			case BOOSTER_PLUSPOWER:
			case BOOSTER_DEFENDUP:
			case BOOSTER_SPECIALUP:
			case BOOSTER_SPEEDUP:
			case BOOSTER_SPECIALGUARD:
			case BOOSTER_CRITICALCUTTER:
			case BOOSTER_EFFECTGUARD:
				booster->FrameRender(GetMemDC(), invenInfo[i].x, invenInfo[i].y,
					invenInfo[i].item.itemKind - 9, 0);
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

	if (isDrag) {
		switch (currentItem.itemKind)
		{
		case ITEM_MONSTERBALL:
			ball->FrameRender(GetMemDC(), g_ptMouse.x - 25, g_ptMouse.y - 25);
			break;
		case ITEM_POTION:
			potion->FrameRender(GetMemDC(), g_ptMouse.x - 25, g_ptMouse.y - 25);
			break;
		case ITEM_ANTIDOTE:
		case ITEM_PARLYZEHEAL:
		case ITEM_BURNHEAL:
		case ITEM_ICEHEAL:
		case ITEM_AWAKENING:
		case ITEM_FULLHEAL:
			potion2->FrameRender(GetMemDC(), g_ptMouse.x - 25, g_ptMouse.y - 25,
				currentItem.itemKind - 3, 0);
			break;
		case BOOSTER_PLUSPOWER:
		case BOOSTER_DEFENDUP:
		case BOOSTER_SPECIALUP:
		case BOOSTER_SPECIALGUARD:
		case BOOSTER_SPEEDUP:
		case BOOSTER_CRITICALCUTTER:
		case BOOSTER_EFFECTGUARD:
			booster->FrameRender(GetMemDC(), g_ptMouse.x - 25, g_ptMouse.y - 25,
				currentItem.itemKind - 9, 0);
			break;
		}
	}

	if (isDebug) {
		for (int i = 0; i < ITEMCOUNT; i++) {
			RectangleMake(GetMemDC(), shopInfo[i].shopBox);
			RectangleMake(GetMemDC(), invenInfo[i].invenBox);
		}
		RectangleMake(GetMemDC(), shopperBox);

		RectangleMake(GetMemDC(), shopRc);
		RectangleMake(GetMemDC(), invenRc);
		RectangleMake(GetMemDC(), statusBox);
	}
}

void ShopScene2::Move()
{
	// left
	if (INPUT->GetKeyDown(VK_LEFT)) {
		moveFrame = 1;
		character->SetFrameY(LEFT_WALK);
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
		character->SetFrameY(RIGHT_WALK);
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
		character->SetFrameY(UP_WALK);
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
		character->SetFrameY(DOWN_WALK);
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

	character->SetFrameX((int)moveFrame);
}

void ShopScene2::UpdateItem()
{
	for (int i = 0; i < shop.GetItemCount(); i++) {
		shopInfo[i].item = shop.GetItem(i);
	}

	for (int i = 0; i < ITEMCOUNT; i++) {
		if (i < player.GetItemCount()) {
			invenInfo[i].item = player.GetItem(i);
		}
		else {
			invenInfo[i].item.itemKind = ITEM_EMPTY;
		}
	}
}
