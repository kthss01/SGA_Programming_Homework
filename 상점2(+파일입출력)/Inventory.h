#pragma once

#include "Item.h"
#include "Color.h"

class Inventory : public Color
{
protected:
	vector<tagItemInfo> m_vItem;
	vector<tagItemInfo>::iterator m_viItem;

	tagItemInfo m_equipArmor;
	tagItemInfo m_equipWeapon;

public:
	Inventory();
	~Inventory();

	// 인벤토리 보여주기
	int ShowInventory(int selectNum);
	// 아이템 추가
	void AddItem(tagItemInfo item);
	// 아이템 판매
	tagItemInfo SellItem(int num, int& gold);

	// 아이템 장착
	void EquipItem(int num);

	tagItemInfo GetEquipArmor() { return m_equipArmor; }
	void SetEquipArmor(tagItemInfo equipArmor) {
		m_equipArmor = equipArmor;
	}

	tagItemInfo GetEquipWeapon() { return m_equipWeapon; }
	void SetEquipWeapon(tagItemInfo equipWeapon) {
		m_equipWeapon = equipWeapon;
	}
};

