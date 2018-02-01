#include <iostream>
#include <Windows.h>

#include "Shop.h"
#include "Customer.h"

#define SLEEP_TIME 1500

using namespace std;

bool buyItem(ITEM_TYPE category, Shop& shop, Customer& customer);
bool sellItem(Shop& shop, Customer& customer);

void main()
{
	Shop shop;
	Customer customer;
	int selectNumber;

	while (true) {
		system("cls");

		shop.ShowSelect();
		cin >> selectNumber;

		switch (selectNumber) {
		// 상점 구매
		case 1:
			while (true) {
				system("cls");
				shop.ShowCategory();
				cin >> selectNumber;


				if (selectNumber == 0)
					break; 
				switch (selectNumber) {
				case 1:
					while (buyItem(ARMOUR, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				case 2:
					while (buyItem(WEAPON, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				case 3:
					while (buyItem(ACCESSORY, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				case 4:
					while (buyItem(POTION, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				}

			}
			break;
		// 인벤토리[판매]
		case 2:
			while (sellItem(shop, customer)) { Sleep(SLEEP_TIME); };
			break;
		// 인벤토리[장착]
		case 3:
			break;
		}
	}
}

bool buyItem(ITEM_TYPE category, Shop& shop, Customer& customer) {
	Item* items;
	Item* item;
	int gold;
	int selectNumber;
	int itemCount;

	system("cls");
	shop.ShowItem(category);
	items = shop.GetItems(category);

	itemCount = shop.GetItemCount(category);

	cout << "현재 gold :" << customer.GetGold() << endl;
	cout << "구매할 아이템의 번호를 입력하세요." << endl;
	cout << "itmeCount : " << itemCount << endl;
	cout << "=== 상점메뉴로 나가길 원하면 0번 ===" << endl;
	cin >> selectNumber;

	if (selectNumber == 0)
		return false;
	else if (selectNumber > 0 && selectNumber <= itemCount) {
		item = &items[selectNumber - 1];
		Item newItem;
		newItem.Init(
			item->GetItemType(), item->GetName(), item->GetPrice(),
			item->GetStat(), item->GetComment(), 1);
		// 사게 되는 경우 금액 감소, 아이템 전달
		if (shop.Buy(category, selectNumber - 1, customer.GetGold())) {
			gold = customer.GetGold();
			customer.SetGold(gold - item->GetPrice());
			customer.InsertItem(newItem);
		}
	}
	return true;
}

bool sellItem(Shop& shop, Customer& customer) {
	int selectNumber;
	int gold;

	system("cls");
	customer.ShowInventory();

	cin >> selectNumber;

	if (selectNumber == 0)
		return false;
	else if(selectNumber > 0 && selectNumber <= customer.GetItemCount()) {
		Item item = customer.GetItem(selectNumber - 1);
		customer.DeleteItem(selectNumber - 1);
		gold = customer.GetGold();
		customer.SetGold(gold + item.GetPrice() / 2);	// 절반값 반환
		int id = shop.GetItemIndex(item.GetItemType(), item.GetName());
		shop.Sell(item.GetItemType(), id);
	}
}