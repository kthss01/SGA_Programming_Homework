#include "MainGame.h"

MainGame::MainGame()
{
	bool isRead = true;

	// 동적 할당 후 초기화
	m_shop = new Shop2;
	isRead = m_shop->ItemSetup();

	m_player = new Player2;
	m_player->InputPlayer();

	m_gold = 10000;
	m_selectNum = 1;
	m_selectPtr = 0;

	if (isRead == true) {
		Sleep(1000);
		SetMainPage();
	}
}


MainGame::~MainGame()
{
	delete m_shop;
	delete m_player;
}

void MainGame::SetLocation(int number)
{
	switch (number) {
	case LOCATION_SHOP:
		SetShopPage();
		break;
	case LOCATION_SELL_INVENTORY:
		SetSellInventoryPage();
		break;
	case LOCATION_INVENTORY:
		SetInventoryPage();
		break;
	default:
		SetMainPage();
		break;
	}
}

void MainGame::SetMainPage()
{
	system("cls");
	cout << "================== 위치 ==================" << endl;
	if (m_selectNum == 1) { SetColor(SET_COLOR); }
	cout << "상점";
	SetDefaultColor();
	cout << " ";
	if (m_selectNum == 2) { SetColor(SET_COLOR); }
	cout << "인벤토리[판매]";
	SetDefaultColor();
	cout << " ";
	if (m_selectNum == 3) { SetColor(SET_COLOR); }
	cout << "인벤토리[장착]" << endl;
	SetDefaultColor();
	//cin >> m_selectNum;

	cout << "============ 종료하시려면 ESC ============" << endl;

	m_selectPtr = getch();
	// test
	//printf("%d\n", m_selectPtr);
	switch (m_selectPtr) {
		// 선택
	case ENTER:
	case SPACE:
		SetLocation(m_selectNum);
		break;
		// 종료
	case ESC:
		break;
	case LEFT_ARROW:
		m_selectNum--;
		if (m_selectNum < 1)
			m_selectNum = 1;
		SetMainPage();
		break;
	case RIGHT_ARROW:
		m_selectNum++;
		if (m_selectNum > 3)
			m_selectNum = 3;
		SetMainPage();
		break;
	default:
		SetMainPage();
		break;
	}
}

void MainGame::SetShopPage()
{
	m_selectNum = 1;
	bool isDone = true;
	while (isDone) {
		system("cls");
		cout << "==================== 상점 =================== " << endl;
		if (m_selectNum == 1) { SetColor(SET_COLOR); }
		cout << "방어구";
		SetDefaultColor();
		cout << " ";
		if (m_selectNum == 2) { SetColor(SET_COLOR); }
		cout << "무기";
		SetDefaultColor();
		cout << " ";
		if (m_selectNum == 3) { SetColor(SET_COLOR); }
		cout << "악세서리";
		SetDefaultColor();
		cout << " ";
		if (m_selectNum == 4) { SetColor(SET_COLOR); }
		cout << "포션" << endl;
		SetDefaultColor();
		cout << "======== 메인 화면으로 나가려면 ESC ========" << endl;

		//cin >> m_selectNum;
		//if (m_selectNum == 0) {
		//	SetMainPage();
		//	break;
		//}
		//else {
		//	SetShopItemPage(m_selectNum);
		//}

		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// 선택
		case ENTER:
		case SPACE:
			SetShopItemPage(m_selectNum);
			break;
			// 종료
		case ESC:
			m_selectNum = 1;
			SetMainPage();
			isDone = false;
			break;
		case LEFT_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case RIGHT_ARROW:
			m_selectNum++;
			if (m_selectNum > 4)
				m_selectNum = 4;
			break;
		default:
			break;
		}
	}
}

void MainGame::SetShopItemPage(int itemKind)
{
	int itemCount = 0;
	m_selectNum = 1;
	bool isDone = true;
	while (isDone) {
		system("cls");

		// 선택 받은 아이템을 출력
		itemCount = m_shop->ItemOutput(itemKind, m_selectNum);

		cout << "현재 gold : " << m_gold << endl;
		cout << "구매할 아이템에서 ENTER or SPACE" << endl;
		cout << "itemCount : " << itemCount << endl;
		cout << "===== 상점 메뉴로 나가길 희망하면 ESC =====" << endl;
		//cin >> m_selectNum;
		//if (m_selectNum == 0)
		//	break;
		//else if ((m_selectNum < 0) || m_selectNum > itemCount) {
		//	cout << "잘못 입력" << endl;
		//}
		//else {
		//	tagItemInfo buyItem
		//		= m_shop->BuyItem(itemKind, m_selectNum, m_gold);
		//	m_player->AddItem(buyItem);
		//	Sleep(1000);
		//}
		tagItemInfo buyItem;
		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// 선택
		case ENTER:
		case SPACE:
			buyItem = m_shop->BuyItem(itemKind, m_selectNum, m_gold);
			m_player->AddItem(buyItem);
			Sleep(1000);
			break;
			// 종료
		case ESC:
			m_selectNum = 1;
			isDone = false;
			break;
		case UP_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case DOWN_ARROW:
			m_selectNum++;
			if (m_selectNum > itemCount)
				m_selectNum = itemCount;
			break;
		default:
			cout << "잘못 입력" << endl;
			break;
		}
	}
}

void MainGame::SetSellInventoryPage()
{
	int itemCount = 0;
	m_selectNum = 1;
	bool isDone = true;
	while (isDone) {
		system("cls");
		cout << "================= 인벤토리 ===============" << endl;
		cout << "현재 gold : " << m_gold << endl;
		cout << "판매할 아이템에서 ENTER or SPACE" << endl;
		cout << "============= 메인 화면은 ESC ============" << endl;

		itemCount = m_player->ShowInventory(m_selectNum);

		//cin >> m_selectNum;

		//if (m_selectNum == 0) {
		//	SetMainPage();
		//	break;
		//}
		//else if ((m_selectNum < 0) || m_selectNum > itemCount) {
		//	cout << "잘못 입력" << endl;
		//}
		//else {
		//	m_shop->AddItem(m_player->SellItem(m_selectNum, m_gold));
		//	Sleep(1000);
		//}

		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// 선택
		case ENTER:
		case SPACE:
			m_shop->AddItem(m_player->SellItem(m_selectNum, m_gold));
			Sleep(1000);
			break;
			// 종료
		case ESC:
			m_selectNum = 1;
			SetMainPage();
			isDone = false;
			break;
		case UP_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case DOWN_ARROW:
			m_selectNum++;
			if (m_selectNum > itemCount)
				m_selectNum = itemCount;
			break;
		default:
			cout << "잘못 입력" << endl;
			break;
		}
	}
}

void MainGame::SetInventoryPage()
{
	int itemCount = 0;
	m_selectNum = 1;
	bool isDone = true;

	while (isDone) {
		system("cls");
		m_player->ShowStatus();
		cout << "=============== 인벤토리 ==============" << endl;
		cout << "장착할 아이템에서 ENTER or SPACE" << endl;
		cout << "============ 메인화면은 ESC ===========" << endl;
		// show Inventory
		itemCount = m_player->ShowInventory(m_selectNum);

		//cin >> m_selectNum;

		//if (m_selectNum == 0) {
		//	SetMainPage();
		//	break;
		//}
		//else if ((m_selectNum < 0) || m_selectNum > itemCount) {
		//	cout << "잘못된 입력" << endl;
		//}
		//else {
		//	m_player->EquipItem(m_selectNum);
		//	Sleep(1000);
		//}

		m_selectPtr = getch();
		// test
		//printf("%d\n", m_selectPtr);
		switch (m_selectPtr) {
			// 선택
		case ENTER:
		case SPACE:
			m_player->EquipItem(m_selectNum);
			Sleep(1000);
			break;
			// 종료
		case ESC:
			m_selectNum = 1;
			SetMainPage();
			isDone = false;
			break;
		case UP_ARROW:
			m_selectNum--;
			if (m_selectNum < 1)
				m_selectNum = 1;
			break;
		case DOWN_ARROW:
			m_selectNum++;
			if (m_selectNum > itemCount)
				m_selectNum = itemCount;
			break;
		default:
			cout << "잘못 입력" << endl;
			break;
		}
	}
}
