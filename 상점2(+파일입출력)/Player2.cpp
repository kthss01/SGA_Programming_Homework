#include "Player2.h"



Player2::Player2()
{
}


Player2::~Player2()
{
}

void Player2::InputPlayer()
{
	m_atk = 10;
	m_def = 10;
}

void Player2::ShowStatus()
{
	int addAtk = m_equipWeapon.attribute;
	int addDef = m_equipArmor.attribute;

	cout << "================ 스탯 ===============" << endl;
	cout << "공격력\t\t" << m_atk << " + " << addAtk << endl;
	cout << "방어력\t\t" << m_def << " + " << addDef << endl;
	cout << "================ 장착 ===============" << endl;
	cout << "무기\t\t" << m_equipWeapon.name << endl;
	cout << "방어구\t\t" << m_equipArmor.name << endl;
}
