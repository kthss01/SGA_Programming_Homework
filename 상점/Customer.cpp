#include "Customer.h"



Customer::Customer()
{
	gold = 10000;
}


Customer::~Customer()
{
}

void Customer::ShowInventory()
{
	cout << "================ 인벤토리 ================" << endl;
	cout << "현재 gold : " << gold << endl;
	cout << "판매할 아이템의 번호를 입력하세요." << endl;
	cout << "=== 메인화면으로 나가길 원하면 0번 ===" << endl;
	for (int i = 0; i<inventory.size(); i++) {
		inventory[i].MakeLine();
		cout << "아이템 번호 : " << i + 1 << endl;
		inventory[i].Show();
		inventory[i].MakeLine();
	}
}

int Customer::GetGold()
{
	return gold;
}

void Customer::SetGold(int gold)
{
	this->gold = gold;
}

void Customer::InsertItem(Item item)
{
	inventory.push_back(item);
}

void Customer::DeleteItem(int id)
{
	inventory.erase(inventory.begin() + id);
}

Item Customer::GetItem(int index)
{
	return inventory[index];
}

int Customer::GetItemCount()
{
	return inventory.size();
}
