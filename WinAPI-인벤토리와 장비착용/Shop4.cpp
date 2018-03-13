#include "stdafx.h"
#include "Shop4.h"


Shop4::Shop4()
{
}


Shop4::~Shop4()
{
}

void Shop4::ItemSetup()
{
	tagItemInfo item;

	FILE* fp;
	fopen_s(&fp, "Text/item.txt", "r");

	char* str = new char[1024];
	memset(str, 0, 1024);

	while (!feof(fp)) {
		char* szBuff = new char[1024];
		memset(szBuff, 0, 1024);
		fgets(szBuff, 1024, fp);

		if (strlen(szBuff) == 0)
			continue;

		str = strtok_s(szBuff, "[]", &szBuff);
		item.itemKind = (ITEM)atoi(str);

		str = strtok_s(szBuff, "[]", &szBuff);
		item.name = str;

		str = strtok_s(szBuff, "[]", &szBuff);
		item.price = atoi(str);

		str = strtok_s(szBuff, "[]", &szBuff);
		item.attribute = atoi(str);

		str = strtok_s(szBuff, "[]", &szBuff);
		item.description = str;

		str = strtok_s(szBuff, "[]", &szBuff);
		item.count = atoi(str);

		m_vItem.push_back(item);
	}
	fclose(fp);
}

void Shop4::AddItem(tagItemInfo item)
{
	//bool isNewItem = true;

	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		if (m_viItem->itemKind == item.itemKind) {
			m_viItem->count++;
			//isNewItem = false;
			break;
		}
	}

	//if (isNewItem == true) {
	//	m_vItem.push_back(item);
	//}
}

tagItemInfo Shop4::BuyItem(int num, int & gold)
{
	tagItemInfo buyItem;
	int itemCount = 0;
	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		if (itemCount++ == num) {
			if (gold >= m_viItem->price) {
				if (m_viItem->count > 0) {
					gold -= m_viItem->price;
					m_viItem->count--;
					buyItem = *m_viItem;
					buyItem.count = 1;
				}
			}
			break;
		}
	}

	return buyItem;
}
