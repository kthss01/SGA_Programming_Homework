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
	m_selectNum = 0;

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
	cout << "1.상점 2.인벤토리[판매] 3.인벤토리[장착]" << endl;
	cin >> m_selectNum;
	SetLocation(m_selectNum);
}

void MainGame::SetShopPage()
{
	while (true) {
		system("cls");
		cout << "==================== 상점 =================== " << endl;
		cout << "1.방어구 2.무기 3.악세서리 4.포션" << endl;
		cout << "=== 메인 화면으로 나가려면 0번을 ===" << endl;

		cin >> m_selectNum;
		if (m_selectNum == 0) {
			SetMainPage();
			break;
		}
		else {
			SetShopItemPage(m_selectNum);
		}
	}
}

void MainGame::SetShopItemPage(int itemKind)
{
	int itemCount = 0;
	while (true) {
		system("cls");

		// 선택 받은 아이템을 출력
		itemCount = m_shop->ItemOutput(itemKind);

		cout << "현재 gold : " << m_gold << endl;
		cout << "구매할 아이템의 번호를 입력" << endl;
		cout << "itemCount : " << itemCount << endl;
		cout << "===== 상점 메뉴로 나가길 희망하면 0번 =====" << endl;
		cin >> m_selectNum;
		if (m_selectNum == 0)
			break;
		else if ((m_selectNum < 0) || m_selectNum > itemCount) {
			cout << "잘못 입력" << endl;
		}
		else {
			tagItemInfo buyItem
				= m_shop->BuyItem(itemKind, m_selectNum, m_gold);
			m_player->AddItem(buyItem);
			Sleep(1000);
		}
	}
}

void MainGame::SetSellInventoryPage()
{
	int itemCount = 0;
	while (true) {
		system("cls");
		cout << "============ 인벤토리 ============" << endl;
		cout << "현재 gold : " << m_gold << endl;
		cout << "판매할 아이템의 번호를 입력" << endl;
		cout << "======== 메인 화면은 0번 =========" << endl;

		itemCount = m_player->ShowInventory();

		cin >> m_selectNum;

		if (m_selectNum == 0) {
			SetMainPage();
			break;
		}
		else if ((m_selectNum < 0) || m_selectNum > itemCount) {
			cout << "잘못 입력" << endl;
		}
		else {
			m_shop->AddItem(m_player->SellItem(m_selectNum, m_gold));
			Sleep(1000);
		}
	}
}

void MainGame::SetInventoryPage()
{
	int itemCount = 0;

	while (true) {
		system("cls");
		m_player->ShowStatus();
		cout << "=============== 인벤토리 ==============" << endl;
		cout << "장착할 아이템의 번호를 입력" << endl;
		cout << "============ 메인화면은 0번 ===========" << endl;
		// show Inventory
		itemCount = m_player->ShowInventory();

		cin >> m_selectNum;

		if (m_selectNum == 0) {
			SetMainPage();
			break;
		}
		else if ((m_selectNum < 0) || m_selectNum > itemCount) {
			cout << "잘못된 입력" << endl;
		}
		else {
			m_player->EquipItem(m_selectNum);
			Sleep(1000);
		}
	}
}
