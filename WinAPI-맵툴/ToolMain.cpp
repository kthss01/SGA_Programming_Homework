#include "stdafx.h"
#include "ToolMain.h"


ToolMain::ToolMain()
{
}


ToolMain::~ToolMain()
{
}

HRESULT ToolMain::Init()
{
	isDebug = false;

	img = IMAGE->FindImage("tile_main");

	for (int j = 0; j < MAXWINTILEX; j++) {
		for (int i = 0; i < MAXWINTILEY; i++) {
			ZeroMemory(&tile[i][j], sizeof(tagTileInfo));
			tile[i][j].x = j * img->GetFrameWidth();
			tile[i][j].y = i * img->GetFrameHeight();
			tile[i][j].rc = RectMake(tile[i][j].x, tile[i][j].y,
				img->GetFrameWidth(), img->GetFrameHeight());
		}
	}

	countX = countY = 0;
	currentX = currentY = 0;

	return S_OK;
}

void ToolMain::Release()
{
}

void ToolMain::Update()
{
	for (int j = 0; j < MAXWINTILEX; j++) {
		for (int i = 0; i < MAXWINTILEY; i++) {
			if (PtInRect(&tile[i][j].rc, g_ptMouse)) {
				currentX = j;
				currentY = i;
			}
		}
	}

	if (INPUT->GetKey(VK_LBUTTON)
		&& SUBWIN->GetIsActive() == false) {
		for (int j = 0; j < MAXWINTILEX; j++) {
			for (int i = 0; i < MAXWINTILEY; i++) {
				if (PtInRect(&tile[i][j].rc, g_ptMouse)) {
					//int x = tile[i][j].x;
					//int y = tile[i][j].y;
					//tile[i][j] = currentTile;
					//tile[i][j].x = x;
					//tile[i][j].y = y;
					int cnt = 0;
					if (vCurrentTile.size() != 0) {
						for (int l = j; l <= j + countX; l++) {
							if (l >= MAXWINTILEX) break;
							for (int k = i; k <= i + countY; k++) {
								if (k >= MAXWINTILEY) {
									cnt += currentY + countY - k + 1;
									break;
								}
								int x = tile[k][l].x;
								int y = tile[k][l].y;
								RECT rc = tile[k][l].rc;
								tile[k][l] = vCurrentTile[cnt++];
								tile[k][l].x = x;
								tile[k][l].y = y;
								tile[k][l].rc = rc;
							}
						}
					}
				}
			}
		}
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void ToolMain::Render()
{
	//=================================================
	{
		// test
		//img->FrameRender(GetMemDC(), WINSIZEX / 2, WINSIZEY / 2,
		//	currentTile.tileX, currentTile.tileY);

		for (int j = 0; j < MAXWINTILEX; j++) {
			for (int i = 0; i < MAXWINTILEY; i++) {
				if (tile[i][j].check == false) continue;
				img->FrameRender(GetMemDC(),
					tile[i][j].x, tile[i][j].y,
					tile[i][j].tileX, tile[i][j].tileY);
			}
		}

		if (vCurrentTile.size() != 0) {
			int cnt = 0;
			for (int j = currentX; j <= currentX + countX; j++) {
				if (j >= MAXWINTILEX) break;
				for (int i = currentY; i <= currentY + countY; i++) {
					if (i >= MAXWINTILEY) {
						cnt += currentY + countY - i + 1;
						break;
					}
					img->FrameRender(GetMemDC(),
						tile[i][j].x, tile[i][j].y,
						vCurrentTile[cnt].tileX, vCurrentTile[cnt].tileY, 128);
					cnt++;
				}
			}
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "(%d %d)", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), WINSIZEX - 100, 0, str, strlen(str));

		for (int j = 0; j < MAXWINTILEX; j++) {
			for (int i = 0; i < MAXWINTILEY; i++) {
				RectangleMake(GetMemDC(), tile[i][j].rc);
			}
		}
	}
	//=================================================
}

void ToolMain::SaveTile()
{
	//string temp;
	//char temp[128];
	//TEXTDATA->CreateWriteTextDataHandle((char*)"save/mapInfo.txt");
	TEXTDATA->CreateWriteTextDataFilePointer((char*)"save/mapInfo.txt");

	for (int j = 0; j < MAXWINTILEX; j++) {
		for (int i = 0; i < MAXWINTILEY; i++) {

			//// 포인트 문제로 고생함 결국 IniData 쪽을 바꿈
			//// 모두 저장하려면 엄청 오래 걸림
			//// check된거만 저장하자 -> 읽어올 때 문제됨
			//sprintf_s(str, "tile_%d_%d_x", i, j);
			//INIDATA->AddData("tile", str, to_string(tile[i][j].x).c_str());
			//sprintf_s(str, "tile_%d_%d_y", i, j);
			//INIDATA->AddData("tile", str, to_string(tile[i][j].y).c_str());
			//sprintf_s(str, "tile_%d_%d_tileX", i, j);
			//INIDATA->AddData("tile", str, to_string(tile[i][j].tileX).c_str());
			//sprintf_s(str, "tile_%d_%d_tileY", i, j);
			//INIDATA->AddData("tile", str, to_string(tile[i][j].tileY).c_str());
			//sprintf_s(str, "tile_%d_%d_check", i, j);
			//INIDATA->AddData("tile", str, to_string((int)tile[i][j].check).c_str());

			//sprintf_s(str, "tile_%d_%d_x", i, j);
			//strcpy_s(temp, sizeof(temp), str);
			//INIDATA->AddData("tile", temp, to_string(tile[i][j].x).c_str());
			//sprintf_s(str, "tile_%d_%d_y", i, j);
			//strcpy_s(temp, sizeof(temp), str);
			//INIDATA->AddData("tile", temp, to_string(tile[i][j].y).c_str());
			//sprintf_s(str, "tile_%d_%d_tileX", i, j);
			//strcpy_s(temp, sizeof(temp), str);
			//INIDATA->AddData("tile", temp, to_string(tile[i][j].tileX).c_str());
			//sprintf_s(str, "tile_%d_%d_tileY", i, j);
			//strcpy_s(temp, sizeof(temp), str);
			//INIDATA->AddData("tile", temp, to_string(tile[i][j].tileY).c_str());
			//sprintf_s(str, "tile_%d_%d_check", i, j);
			//strcpy_s(temp, sizeof(temp), str);
			//INIDATA->AddData("tile", temp, to_string((int)tile[i][j].check).c_str());

			//temp = "tile_" + to_string(i) + "_" + to_string(j) + "_x";
			//INIDATA->AddData("tile", temp.c_str(), to_string(tile[i][j].x).c_str());
			//temp = "tile_" + to_string(i) + "_" + to_string(j) + "_y";
			//INIDATA->AddData("tile", temp.c_str(), to_string(tile[i][j].y).c_str());
			//temp = "tile_" + to_string(i) + "_" + to_string(j) + "_tileX";
			//INIDATA->AddData("tile", temp.c_str(), to_string(tile[i][j].tileX).c_str());
			//temp = "tile_" + to_string(i) + "_" + to_string(j) + "_tileY";
			//INIDATA->AddData("tile", temp.c_str(), to_string(tile[i][j].tileY).c_str());
			//temp = "tile_" + to_string(i) + "_" + to_string(j) + "_check";
			//INIDATA->AddData("tile", temp.c_str(), to_string((int)tile[i][j].check).c_str());

			vector<string> vStr;
			sprintf_s(str, "%d", tile[i][j].x);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile[i][j].y);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile[i][j].tileX);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile[i][j].tileY);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile[i][j].check);
			vStr.push_back(str);
			TEXTDATA->TextWrite(vStr);
		}
	}
	//INIDATA->SaveData("MapInfo");

	//TEXTDATA->CloseTextDataHandle();
	TEXTDATA->CloseTextDataFilePointer();
}

void ToolMain::LoadTile()
{
	//TEXTDATA->CreateReadTextDataHandle((char*)"save/mapInfo.txt");
	TEXTDATA->CreateReadTextDataFilePointer((char*)"save/mapInfo.txt");

	vector< vector<string> > vvStr = TEXTDATA->TextRead();

	for (int j = 0; j < MAXWINTILEX; j++) {
		for (int i = 0; i < MAXWINTILEY; i++) {
			//sprintf_s(str, "tile_%d_%d_x", i, j);
			//tile[i][j].x = INIDATA->LoadDataInteger("MapInfo", "tile", str);
			//sprintf_s(str, "tile_%d_%d_y", i, j);
			//tile[i][j].y = INIDATA->LoadDataInteger("MapInfo", "tile", str);
			//sprintf_s(str, "tile_%d_%d_tileX", i, j);
			//tile[i][j].tileX = INIDATA->LoadDataInteger("MapInfo", "tile", str);
			//sprintf_s(str, "tile_%d_%d_tileY", i, j);
			//tile[i][j].tileY = INIDATA->LoadDataInteger("MapInfo", "tile", str);
			//sprintf_s(str, "tile_%d_%d_check", i, j);
			//tile[i][j].check = INIDATA->LoadDataInteger("MapInfo", "tile", str);
			//tile[i][j].rc = RectMake(tile[i][j].x, tile[i][j].y,
			//	img->GetFrameWidth(), img->GetFrameHeight());

			vector<string> vStr = vvStr[j * MAXWINTILEY + i];

			tile[i][j].x = atoi(vStr[0].c_str());
			tile[i][j].y = atoi(vStr[1].c_str());
			tile[i][j].tileX = atoi(vStr[2].c_str());
			tile[i][j].tileY = atoi(vStr[3].c_str());
			tile[i][j].check = atoi(vStr[4].c_str());

			tile[i][j].rc = RectMake(tile[i][j].x, tile[i][j].y,
				img->GetFrameWidth(), img->GetFrameHeight());
		}
	}

	//TEXTDATA->CloseTextDataHandle();
	TEXTDATA->CloseTextDataFilePointer();
}
