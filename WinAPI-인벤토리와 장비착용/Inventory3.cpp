#include "stdafx.h"
#include "Inventory3.h"


Inventory3::Inventory3()
{
}


Inventory3::~Inventory3()
{
}

void Inventory3::ItemSetup()
{
	tagItemInfo item;
	item.itemKind = ITEM_MONSTERBALL;
	item.name = "몬스터볼";
	item.price = 200;
	item.attribute = 1;
	item.description = "야생 포켓몬에게 던져서 잡기 위한 볼. 캡슐식으로 되어있다.";
	item.count = 5;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_PLUSPOWER;
	item.name = "플러스파워";
	item.price = 500;
	item.attribute = 5;
	item.description = "포켓몬의 공력력이 5 올라간다.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_DEFENDUP;
	item.name = "디펜드업";
	item.price = 550;
	item.attribute = 5;
	item.description = "포켓몬의 방어력이 5 올라간다.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_SPECIALUP;
	item.name = "스폐셜업";
	item.price = 350;
	item.attribute = 5;
	item.description = "포켓몬의 특수 공격력이 5 올라간다.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_SPECIALGUARD;
	item.name = "스폐셜가드";
	item.price = 350;
	item.attribute = 5;
	item.description = "포켓몬의 특수 방어력이 5 올라간다.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_SPEEDUP;
	item.name = "스피드업";
	item.price = 350;
	item.attribute = 5;
	item.description = "포켓몬의 스피드가 5 올라간다.";
	item.count = 1;

	m_vItem.push_back(item);

	// 이미지 없어서 안쓰기로
	//item.itemKind = BOOSTER_ACCURACY;
	//item.name = "잘-맞히기";
	//item.price = 950;
	//item.attribute = 5;
	//item.description = "포켓몬의 명중률이 5 올라간다.";
	//item.count = 1;

	//m_vItem.push_back(item);

	item.itemKind = BOOSTER_CRITICALCUTTER;
	item.name = "크리티컬커터";
	item.price = 350;
	item.attribute = 5;
	item.description = "포켓몬의 크리컬확률이 5 올라간다.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_EFFECTGUARD;
	item.name = "이펙트가드";
	item.price = 700;
	item.attribute = 1;
	item.description = "포켓몬의 능력치가 내려가지 않는다.";
	item.count = 1;

	m_vItem.push_back(item);

	this->m_equipItem.itemKind = ITEM_EMPTY;
}

void Inventory3::AddItem(tagItemInfo item)
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

tagItemInfo Inventory3::SellItem(int num, int & gold)
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

void Inventory3::EquipItem(int num)
{
	if(num == -1){
		tagItemInfo beforeItem;
		beforeItem = GetEquipItem();

		tagItemInfo emptyItem;
		emptyItem.itemKind = ITEM_EMPTY;
		SetEquipItem(emptyItem);

		if (beforeItem.itemKind != ITEM_EMPTY)
			m_vItem.push_back(beforeItem);
	}
	else {
		tagItemInfo beforeItem;
		m_viItem = m_vItem.begin() + num;

		beforeItem = GetEquipItem();
		SetEquipItem(*m_viItem);
		m_vItem.erase(m_viItem);
		if (beforeItem.itemKind != ITEM_EMPTY)
			m_vItem.push_back(beforeItem);
	}
}
