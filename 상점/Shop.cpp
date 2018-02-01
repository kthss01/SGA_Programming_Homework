#include "Shop.h"



Shop::Shop()
{
	Init();
}


Shop::~Shop()
{
	delete[] weapon;
	delete[] armour;
	delete[] potion;
	delete[] acce;
}

void Shop::ShowSelect()
{
	cout << "================ 위치 ================" << endl;
	cout << "1. 상점 2. 인벤토리[판매] 3. 인벤토리[장착]" << endl;
}

void Shop::ShowCategory()
{
	cout << "================ 상점 ================" << endl;
	cout << "1. 방어구 2. 무기 3. 악세서리 4. 포션" << endl;
	cout << "=== 메인화면으로 나가길 원하면 0번 ===" << endl;
}

void Shop::ShowItem(ITEM_TYPE category)
{
	switch (category)
	{
	case WEAPON:
		for (int i=0; i<weaponSize; i++){
			weapon[i].MakeLine();
			cout << "아이템 번호 : " << i + 1 << endl;
			weapon[i].Show();
			weapon[i].MakeLine();
		}
		break;
	case ARMOUR:
		for (int i = 0; i<armourSize; i++) {
			armour[i].MakeLine();
			cout << "아이템 번호 : " << i + 1 << endl;
			armour[i].Show();
			armour[i].MakeLine();
		}
		break;
	case POTION:
		for (int i = 0; i<potionSize; i++) {
			potion[i].MakeLine();
			cout << "아이템 번호 : " << i + 1 << endl;
			potion[i].Show();
			weapon[i].MakeLine();
		}
		break;
	case ACCESSORY:
		for (int i = 0; i<acceSize; i++) {
			acce[i].MakeLine();
			cout << "아이템 번호 : " << i + 1 << endl;
			acce[i].Show();
			acce[i].MakeLine();
		}
		break;
	}
}

bool Shop::Buy(ITEM_TYPE category, int id, int gold)
{
	int count;
	string name;
	int price;
	// 상점에 수량이 있어서 살 수 있는지 확인하는 변수
	bool isCount = true;
	bool isMoney = true;
	switch (category)
	{
	case WEAPON:
		count = weapon[id].GetCount();
		price = weapon[id].GetPrice();
		name = weapon[id].GetName();
		if (count > 0 && gold >= price) {
			weapon[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	case ARMOUR:
		count = armour[id].GetCount();
		name = armour[id].GetName();
		price = armour[id].GetPrice();
		if (count > 0 && gold >= price) {
			armour[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	case POTION:
		count = potion[id].GetCount();
		name = potion[id].GetName();
		price = potion[id].GetPrice();
		if (count > 0 && gold >= price) {
			potion[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	case ACCESSORY:
		count = acce[id].GetCount();
		name = acce[id].GetName();
		price = acce[id].GetPrice();
		if (count > 0 && gold >= price) {
			acce[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	}

	if (isCount == true && isMoney) {
		cout << name << "을 " << price << "gold" << "에 구매하였습니다" << endl;
		return true;
	}
	else if (isCount == false) {
		cout << "재고가 없어서 구매 할 수 없습니다." << endl;
		return false;
	}
	else if (isMoney == false) {
		cout << "돈이 부족합니다." << endl;
		return false;
	}
}

void Shop::Sell(ITEM_TYPE category, int id)
{
	int count;
	string name;
	int price;

	switch (category)
	{
	case WEAPON:
		count = weapon[id].GetCount();
		name = weapon[id].GetName();
		price = weapon[id].GetPrice();
		weapon[id].SetCount(count + 1);
		break;
	case ARMOUR:
		count = armour[id].GetCount();
		name = armour[id].GetName();
		price = armour[id].GetPrice();
		armour[id].SetCount(count + 1);
		break;
	case POTION:
		count = potion[id].GetCount();
		name = potion[id].GetName();
		price = potion[id].GetPrice();
		potion[id].SetCount(count + 1);
		break;
	case ACCESSORY:
		count = acce[id].GetCount();
		name = acce[id].GetName();
		price = acce[id].GetPrice();
		acce[id].SetCount(count + 1);
		break;
	}

	cout << name << "을 " << price / 2 << "gold에 판매하였습니다." << endl;
}

void Shop::MakeWeapon()
{
	weaponSize = 5;
	weapon = new Weapon[weaponSize];
	weapon[0].Init(
		WEAPON, "연습용 목도", 500, 4,
		"잘 다듬어진 나무칼. 무게가 잘 맞춰져있어 초보자가 다루기 쉽다.", 1
	);
	weapon[1].Init(
		WEAPON, "숏 소드", 2000, 7,
		"가장 다루기 쉽고 안정적인 공격을 할 수 있는 무기", 1
	);
	weapon[2].Init(
		WEAPON, "롱 소드", 3000, 9,
		"길이가 길면서도 안정적인 공격력을 자랑하는 무기.", 1
	);
	weapon[3].Init(
		WEAPON, "투 핸디드 소드", 7000, 25,
		"보통 사람의 키보다 클 정도의 장검으로 한손으로는 사용 할 수 없다.", 1
	);
	weapon[4].Init(
		WEAPON, "클레이모어", 8000, 28,
		"두께가 얇고 탄력성이 있는 예리한 날로 적을 베기 위해 만들어진 양손검.", 1
	);
}

void Shop::MakeArmour()
{
	armourSize = 5;
	armour = new Armour[armourSize];
	armour[0].Init(
		ARMOUR, "초보자용 바지", 50, 1,
		"초보자가 보통 입는 간단한 옷이다.", 1
	);
	armour[1].Init(
		ARMOUR, "사냥꾼옷", 200, 2,
		"가죽소재의 상의와 튼튼한 천으로 된 바지로 이루어져있다.", 1
	);
	armour[2].Init(
		ARMOUR, "라이트 레더메일", 600, 4,
		"통짜 가죽을 묶어서 만든 가죽갑옷", 1
	);
	armour[3].Init(
		ARMOUR, "체인메일", 2000, 8,
		"쇠고리를 엮은 사슬로 만든 갑옷이다.", 1
	);
	armour[4].Init(
		ARMOUR, "플레이트메일", 4000, 14,
		"철판을 덧대어 만든 튼튼한 갑옷이다.", 1
	);
}

void Shop::MakePotion()
{
	potionSize = 5;
	string hp[5] = { "10", "30", "50", "100", "300" };
	int price[5] = { 50,200,400,1000,4000 };
	potion = new Potion[potionSize];
	for (int i = 0; i < potionSize; i++) {
		potion[i].Init(
			POTION, "생명력 " + hp[i] + " 포션", price[i], atoi(hp[i].c_str()),
			"생명력을 " + hp[i] + " 회복할 수 있는 포션", 1
		);
	}
}

void Shop::MakeAcce()
{
	acceSize = 5;
	acce = new Accessory[acceSize];
	acce[0].Init(
		ACCESSORY, "녹슨 반지", 50, 1,
		"녹이 슬어 빛을 잃어버린 반지", 1
	);
	acce[1].Init(
		ACCESSORY, "반지", 100, 2,
		"심플하고 밝은 빛깔의 반지", 1
	);
	acce[2].Init(
		ACCESSORY, "은반지", 500, 3,
		"은으로 만들어진 반지", 1
	);
	acce[3].Init(
		ACCESSORY, "금반지", 1000, 4,
		"금으로 만들어진 반지", 1
	);
	acce[4].Init(
		ACCESSORY, "다이아반지", 2000, 5,
		"다이아몬드가 박혀있는 반지", 1
	);
}

void Shop::ReadItem(ITEM_TYPE category)
{
	switch (category)
	{
	case WEAPON:
		MakeWeapon();
		break;
	case ARMOUR:
		MakeArmour();
		break;
	case POTION:
		MakePotion();
		break;
	case ACCESSORY:
		MakeAcce();
		break;
	}
}

Item* Shop::GetItems(ITEM_TYPE category)
{
	switch (category)
	{
	case WEAPON:
		return weapon;
		break;
	case ARMOUR:
		return armour;
		break;
	case POTION:
		return potion;
		break;
	case ACCESSORY:
		return acce;
		break;
	}
}

int Shop::GetItemIndex(ITEM_TYPE category, string name)
{
	int index;

	switch (category)
	{
	case WEAPON:
		for (int i = 0; i < weaponSize; i++) {
			if (weapon[i].GetName() == name)
				index = i;
		}
		break;
	case ARMOUR:
		for (int i = 0; i < armourSize; i++) {
			if (armour[i].GetName() == name)
				index = i;
		}
		break;
	case POTION:
		for (int i = 0; i < potionSize; i++) {
			if (potion[i].GetName() == name)
				index = i;
		}
		break;
	case ACCESSORY:
		for (int i = 0; i < acceSize; i++) {
			if (acce[i].GetName() == name)
				index = i;
		}
		break;
	}

	return index;
}

int Shop::GetItemCount(ITEM_TYPE category)
{
	int count;

	switch (category)
	{
	case WEAPON:
		count = weaponSize;
			break;
	case ARMOUR:
		count = armourSize;
		break;
	case POTION:
		count = potionSize;
		break;
	case ACCESSORY:
		count = acceSize;
		break;
	}

	return count;
}

void Shop::Init()
{
	ReadItem(WEAPON);
	ReadItem(ARMOUR);
	ReadItem(POTION);
	ReadItem(ACCESSORY);

}

//void Shop::Update()
//{
//}
//
//void Shop::Render()
//{
//}