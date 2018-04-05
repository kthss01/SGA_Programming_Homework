#pragma once

#include "SIngletonBase.h"

enum CountOrder {
	KILL,
	HIT,
	GAMEOVER,
	GAMECLEAR,
	END
};

struct tagCount {
	int kill;
	int hit;
	int gameover;
	int ending;
};

class SaveBook : public SingletonBase<SaveBook>
{
private:
	Image * book;

	vector<string> vStr;
	char str[128];

	float deltaTime;
	int currentFrameX;

	tagCount count;

	int hp;
	POINT pos;

	bool isSave;
	RECT rc;
public:
	SaveBook();
	~SaveBook();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Animation();
	void ReadData();
	void SaveData();

	void AddKill() { count.kill++; }
	void AddHit() { count.hit++; }
	void AddGameOver() { count.gameover++; }
	void AddEnding() { count.ending++; }

	RECT GetRect() { 
		return RectMake(
			book->GetX(), book->GetY(), 
			book->GetFrameWidth(), book->GetFrameHeight()); 
	}

	void SetIsSave(bool isSave) { this->isSave = isSave; }
	bool GetIsSave() { return isSave; }

	void SetHp(int hp) { this->hp = hp; }
	int GetHp() { return hp; }
	void SetPos(POINT pos) { this->pos = pos; }
	POINT GetPos() { return pos; }
};

#define SAVEBOOK SaveBook::GetSingleton()
