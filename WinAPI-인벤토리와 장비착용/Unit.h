#pragma once

#include <string>

using namespace std;

class Unit {
protected:
	string m_name;
	int m_level;
	int m_maxExp;
	int m_exp;
	int m_maxHp;
	int m_hp;
	int m_atk;
	int m_def;
	int m_spAtk;
	int m_spDef;
	int m_speed;
public:
	Unit() {}
	~Unit() {}

	string GetName() { return m_name; }
	void SetName(string name) { m_name = name; }
	int GetLevel() { return m_level; }
	void SetLevel(int level) { m_level = level; }
	int GetMaxExp() { return m_maxExp; }
	void SetMaxExp(int maxExp) { m_maxExp = maxExp; }
	int GetExp() { return m_exp; }
	void SetExp(int exp) { m_exp = exp; }
	int GetMaxHp() { return m_maxHp; }
	void SetMaxHp(int maxHp) { m_maxHp = maxHp; }
	int GetHp() { return m_hp; }
	void SetHp(int hp) { m_hp = hp; }
	int GetAtk() { return m_atk; }
	void SetAtk(int atk) { m_atk = atk; }
	int GetDef() { return m_def; }
	void SetDef(int def) { m_def = def; }
	int GetSpAtk() { return m_spAtk; }
	void SetSpAtk(int spAtk) { m_spAtk = spAtk; }
	int GetSpDef() { return m_spDef; }
	void SetSpDef(int spDef) { m_spDef = spDef; }
	int GetSpeed() { return m_speed; }
	void SetSpeed(int speed) { m_speed = speed; }
};