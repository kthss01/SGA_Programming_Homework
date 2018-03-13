#pragma once

#include "Item.h"

class Inventory2
{
protected:
	vector<tagItemInfo> m_vItem;
	vector<tagItemInfo>::iterator m_viItem;

public:
	Inventory2();
	~Inventory2();

	void ItemSetup();
	void AddItem(tagItemInfo item);
	tagItemInfo SellItem(int num, int& gold);

	tagItemInfo GetItem(int num) { return m_vItem[num]; }
	int GetItemCount() { return m_vItem.size(); }
};

