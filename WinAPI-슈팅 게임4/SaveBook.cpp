#include "stdafx.h"
#include "SaveBook.h"


SaveBook::SaveBook()
{
}


SaveBook::~SaveBook()
{
}

HRESULT SaveBook::Init()
{
	book = IMAGE->FindImage("save_book");

	book->SetX(WINSIZEX - book->GetFrameWidth() - 125);
	book->SetY(0);

	ReadData();

	rc = RectMake(WINSIZEX - 110, 0, 110, 110);

	deltaTime = 0;
	currentFrameX = 0;

	return S_OK;
}

void SaveBook::Release()
{
}

void SaveBook::Update()
{
	if (isSave) {
		Animation();
	}
	else
		currentFrameX = 0;
}

void SaveBook::Render(HDC hdc)
{
	RectangleMake(hdc, rc);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));

	for (int i = 0; i < END; i++) {
		switch (i)
		{
		case KILL:
			sprintf_s(str, "Kill : %d", count.kill);
			break;
		case HIT:
			sprintf_s(str, "Hit : %d", count.hit);
			break;
		case GAMEOVER:
			sprintf_s(str, "GameOver : %d", count.gameover);
			break;
		case GAMECLEAR:
			sprintf_s(str, "GameClear : %d", count.ending);
			break;
		}
		TextOut(hdc, WINSIZEX - 100, 10 + i * 25, str, strlen(str));
	}

	book->FrameRender(hdc, book->GetX(), book->GetY(), currentFrameX, 0);
}

void SaveBook::Animation()
{
	deltaTime += FRAME->GetElapsedTime();

	if (deltaTime > 0.3f) {
		currentFrameX++;
		if (currentFrameX == book->GetMaxFrameX()) {
			currentFrameX = 0;
			SaveData();
			isSave = false;
		}
		deltaTime = 0;
	}
}

void SaveBook::ReadData()
{
	vStr = TEXTDATA->TextLoad((char*)"save/countData.txt");
	
	count.kill = atoi(vStr[KILL].c_str());
	count.hit = atoi(vStr[HIT].c_str());
	count.gameover = atoi(vStr[GAMEOVER].c_str());
	count.ending = atoi(vStr[GAMECLEAR].c_str());

	hp = atoi(vStr[4].c_str());
	pos.x = atoi(vStr[5].c_str());
	pos.y = atoi(vStr[6].c_str());
}

void SaveBook::SaveData()
{
	vector<string> str;
	str.push_back(to_string(count.kill));
	str.push_back(to_string(count.hit));
	str.push_back(to_string(count.gameover));
	str.push_back(to_string(count.ending));
	str.push_back(to_string(hp));
	str.push_back(to_string(pos.x));
	str.push_back(to_string(pos.y));

	TEXTDATA->TextSave((char*)"save/countData.txt", str);
}
