#pragma once

#include "Item.h"

class Inventory3
{
protected:
	vector<tagItemInfo> m_vItem;
	vector<tagItemInfo>::iterator m_viItem;

	tagItemInfo m_equipItem;
public:
	Inventory3();
	~Inventory3();

	void ItemSetup();
	void AddItem(tagItemInfo item);
	tagItemInfo SellItem(int num, int& gold);

	tagItemInfo GetItem(int num) { return m_vItem[num]; }
	int GetItemCount() { return m_vItem.size(); }

	void EquipItem(int num);

	tagItemInfo GetEquipItem() { return m_equipItem; }
	void SetEquipItem(tagItemInfo equipItem) { m_equipItem = equipItem; }
};

