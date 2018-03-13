#include "stdafx.h"
#include "Inventory2.h"


Inventory2::Inventory2()
{
}


Inventory2::~Inventory2()
{
}

void Inventory2::ItemSetup()
{
	tagItemInfo item;
	item.itemKind = ITEM_MONSTERBALL;
	item.name = "몬스터볼";
	item.price = 200;
	item.attribute = 1;
	item.description = "야생 포켓몬에게 던져서 잡기 위한 볼. 캡슐식으로 되어있다.";
	item.count = 5;

	m_vItem.push_back(item);
}

void Inventory2::AddItem(tagItemInfo item)
{
	bool isNewItem = true;

	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// 아이템 같은 종류인지 확인
		if (m_viItem->itemKind != item.itemKind) continue;

		// strcmp 같음 string 안에 compare 함수 있음
		// 아이템 이름 같으면 갯수만 증가
		if (m_viItem->name.compare(item.name) == 0) {
			m_viItem->count++;
			isNewItem = false;
			break;
		}
	}

	if (isNewItem == true) {
		m_vItem.push_back(item);
	}
}

tagItemInfo Inventory2::SellItem(int num, int & gold)
{
	tagItemInfo sellItem;
	m_viItem = m_vItem.begin() + num;
	sellItem = *m_viItem;

	if (m_viItem->count > 1)
		m_viItem->count--;
	else
		m_vItem.erase(m_viItem);

	int sellPrice = sellItem.price / 2;
	gold += sellPrice;

	return sellItem;
}
