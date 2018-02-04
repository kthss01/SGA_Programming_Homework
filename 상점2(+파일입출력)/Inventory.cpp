#include "Inventory.h"



Inventory::Inventory()
{
	tagItemInfo itemEmpty = { ITEM_EMPTY, "", "", 0,0,0 };
	m_equipArmor = itemEmpty;
	m_equipWeapon = itemEmpty;
}


Inventory::~Inventory()
{
}

int Inventory::ShowInventory(int m_selectNum)
{
	int itemCount = 0;
	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		itemCount++;

		if (itemCount == m_selectNum)
			SetColor(SET_COLOR);
		cout << "==================================" << endl;
		cout << "아이템 번호 : " << itemCount << endl;
		cout << "이름 : " << m_viItem->name << endl;
		cout << "가격 : " << m_viItem->price << endl;
		cout << "능력치 : " << m_viItem->attribute << endl;
		cout << "설명: " << m_viItem->description << endl;
		cout << "==================================" << endl;
		SetDefaultColor();
	}
	return itemCount;
}

void Inventory::AddItem(tagItemInfo item)
{
	// 아이템이 존재하기만 하면 넣기
	if (item.itemKind != ITEM_EMPTY) {
		m_vItem.push_back(item);
	}
}

tagItemInfo Inventory::SellItem(int num, int & gold)
{
	// 인벤에서 판매 아이템 제거
	tagItemInfo sellItem;
	m_viItem = m_vItem.begin() + (num - 1);
	sellItem = *m_viItem;
	m_vItem.erase(m_viItem);

	// 판매 금액
	int sellPrice = sellItem.price / 2;
	cout << sellItem.name << "을 " << sellPrice << "gold에 판매" << endl;
	gold += sellPrice;

	return sellItem;
}

void Inventory::EquipItem(int num)
{
	tagItemInfo beforeItem;
	m_viItem = m_vItem.begin() + (num - 1);

	switch (m_viItem->itemKind)
	{
	case ITEM_ARMOR:
		beforeItem = GetEquipArmor();
		SetEquipArmor(*m_viItem);
		m_vItem.erase(m_viItem);

		if (beforeItem.itemKind == ITEM_ARMOR) {
			m_vItem.push_back(beforeItem);
		}
		break;
	case ITEM_WEAPON:
		beforeItem = GetEquipWeapon();
		SetEquipWeapon(*m_viItem);
		m_vItem.erase(m_viItem);

		if (beforeItem.itemKind == ITEM_WEAPON) {
			m_vItem.push_back(beforeItem);
		}
		break;
	default:
		break;
	}
}
