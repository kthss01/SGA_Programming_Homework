#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Weapon.h"
#include "Armour.h"
#include "Potion.h"
#include "Accessory.h"

using namespace std;

class Shop
{
private:
	Item* weapon;
	Item* armour;
	Item* potion;
	Item* acce;

	int weaponSize;
	int armourSize;
	int potionSize;
	int acceSize;
public:
	Shop();
	~Shop();

	// 구매, 판매 선택을 콘솔에 보여주는 함수
	void ShowSelect();
	// 방어구, 무기, 악세서리, 포션 목록 선택을 콘솔에 보여주는 함수
	void ShowCategory();

	// 특정 목록에 아이템을 보여주는 함수
	void ShowItem(ITEM_TYPE category);
	// 특정 목록에 아이템을 사는 함수 사게 되는지 아닌지 true/false 반환
	bool Buy(ITEM_TYPE category, int id, int gold);
	// 특정 목록에 아이템을 파는 함수
	void Sell(ITEM_TYPE category, int id);

	// 아이템 만드는 함수 (하드코딩 == 노가다)
	void MakeWeapon();
	void MakeArmour();
	void MakePotion();
	void MakeAcce();
	// 아이템을 읽어오는 함수 현재는 하드코딩한 아이템 읽어옴
	void ReadItem(ITEM_TYPE category);

	// 특정 목록에 아이템이 담긴 vector를 반환하는 함수
	Item* GetItems(ITEM_TYPE category);
	// 특정 목록에 담긴 아이템의 Index를 반환하는 함수
	int GetItemIndex(ITEM_TYPE category, string name);
	// 특정 목록에 담긴 아이템의 갯수 반환하는 함수
	int GetItemCount(ITEM_TYPE category);

	void Init();	// Shop 클래스 초기화 (아이템 읽어옴)
	//void Release();
	//void Update();
	//void Render();
};

