#pragma once
class Unit
{
protected:
	int m_atk;
	int m_def;

public:
	Unit();
	~Unit();

	int GetAtk() { return m_atk; }
	void SetAtk(int atk) { m_atk = atk; }

	int GetDef() { return m_def; }
	void SetDef(int def) { m_def = def; }
};

