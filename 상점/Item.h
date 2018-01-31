#pragma once
#include <iostream>
#include <string>

using namespace std;

#define MAX_LINE 50					// 줄 긋기 위해 표시할 갯수

enum ITEM_TYPE { WEAPON, ARMOUR, POTION, ACCESSORY }; // 아이템 종류 결정

enum STAT_TYPE { NONE, HP, MP, ATK, DEF };	// 능력치 어떤 부분을 상승시킬지 결정

class Item
{
protected:
	ITEM_TYPE itemType;
	string name;
	int price;
	STAT_TYPE type;
	int stat;
	string comment;
	int count;

public:
	Item();
	~Item();

	// Item 멤버변수 초기화 시키는 함수
	virtual void Init(
		ITEM_TYPE itemType, string name, int price, 
		int stat, string comment, int count);

	// 상승하는 능력치에 따라 조금은(능력치부분만) 다르게 출력하기 위해
	// 가상함수로 선언하여 재정의 할 수 있게함
	virtual void Show();			// Item 정보를 보여주는 함수

	// 상속된 클래스마다 다르게 재정의 해야하고 무조건 재정의해야해서
	// 순수 가상함수로 선언, 사실 굳이 이렇게 할 필요는 없음
	// 순수 가상함수로 선언하려 했으나 추상 클래스가 되버려서 
	// Shop 클래스에서 Item으로 접근하기 힘듬 -> 안하기로 변경
	virtual void InitType();	// 능력치 상승 타입 결정

	void MakeLine();				// 줄 긋는 함수

	// 능력치 타입 변경 필요한 경우를 위해 함수 추가
	void SetType(STAT_TYPE type);		
	void SetCount(int count);

	string GetName();
	int GetPrice();
	int GetStat();
	string GetComment();
	int GetCount();
	STAT_TYPE GetStatType();
	ITEM_TYPE GetItemType();
};

