#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum ITEM {
	ITEM_EMPTY,
	ITEM_MONSTERBALL,
	ITEM_POTION,
	ITEM_ANTIDOTE,
	ITEM_PARLYZEHEAL,
	ITEM_BURNHEAL,
	ITEM_ICEHEAL,
	ITEM_AWAKENING,
	ITEM_FULLHEAL,

	BOOSTER_CRITICALCUTTER,
	BOOSTER_EFFECTGUARD,
	BOOSTER_SPECIALGUARD,
	BOOSTER_PLUSPOWER,
	BOOSTER_DEFENDUP,
	BOOSTER_SPECIALUP,
	BOOSTER_SPEEDUP,
};

struct tagItemInfo {
	ITEM itemKind;
	string name;
	string description;
	int attribute;
	int price;
	int count;
};