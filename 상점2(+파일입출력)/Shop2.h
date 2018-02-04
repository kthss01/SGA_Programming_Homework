#pragma once

#include "Item.h"
#include "Color.h"

#include <stdio.h>
#include <string.h>

class Shop2 : public Color
{
	// 구조체를 담은 벡터
	vector<tagItemInfo> m_vItem;
	vector<tagItemInfo>::iterator m_viItem;

public:
	Shop2();
	~Shop2();

	// 초기 아이템 셋팅
	bool ItemSetup();
	// 아이템 출력
	int ItemOutput(int num, int selectNum);
	// 상점 아이템 구입
	tagItemInfo BuyItem(int itemKind, int num, int& gold);
	// 상점에 아이템 추가
	void AddItem(tagItemInfo item);
};

