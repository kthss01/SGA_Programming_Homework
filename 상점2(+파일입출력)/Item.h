#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum ITEM {
	ITEM_EMPTY,		// 아이템 x
	ITEM_ARMOR,		// 방어구
	ITEM_WEAPON,	// 무기
	ITEM_RING,		// 반지
	ITEM_POTION,	// 포션
	ITEM_SOLDOUT	// 판매 완료
};

// 아이템 정보 관련 구조체
struct tagItemInfo{
	ITEM itemKind;			// 아이템의 종류
	string name;			// 아이템의 이름
	string description;		// 아이템의 설명
	int attribute;			// 아이템의 특성
	int price;				// 아이템의 가격
	int count;				// 아이템의 갯수
};