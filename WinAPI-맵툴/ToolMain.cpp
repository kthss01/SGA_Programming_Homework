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

	startX = MAXWINTILEX / 2;
	startY = MAXWINTILEY / 2;
	endX = MAXWINTILEX * 3 / 2;
	endY = MAXWINTILEY * 3 / 2;

	for (int j = 0; j < MAXWINTILEX * 2; j++) {
		for (int i = 0; i < MAXWINTILEY * 2; i++) {
			ZeroMemory(&tile.tile[i][j], sizeof(tagTileInfo));
			if (startX <= j && j < endX && startY <= i && i < endY) {
				tile.tile[i][j].x = j * img->GetFrameWidth() - startX * 25;
				tile.tile[i][j].y = i * img->GetFrameHeight() - startY * 25;
				tile.tile[i][j].rc = RectMake(tile.tile[i][j].x, tile.tile[i][j].y,
					img->GetFrameWidth(), img->GetFrameHeight());

				//// test
				//tile[i][j].check = true;
				//tile[i][j].tileX = RND->GetInt(MAXTILEX);
				//tile[i][j].tileY = RND->GetInt(MAXTILEY);
			}
			// test2
			//tile[i][j].check = true;
			//tile[i][j].tileX = RND->GetInt(MAXTILEX);
			//tile[i][j].tileY = RND->GetInt(MAXTILEY);
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

	//if (INPUT->GetKey('A') && startX > 0) {
	if(g_ptMouse.x < WINSIZEX / 8 && startX > 0
		&& SUBWIN->GetIsActive() == false) {
		startX--;
		endX--;
	}
	//if (INPUT->GetKey('D') && endX < MAXWINTILEX * 2) {
	if (g_ptMouse.x > WINSIZEX - WINSIZEX / 8 && endX < MAXWINTILEX * 2
		&& SUBWIN->GetIsActive() == false) {
		startX++;
		endX++;
	}
	//if (INPUT->GetKey('W') && startY > 0) {
	if (g_ptMouse.y < WINSIZEY / 8 && startY > 0
		&& SUBWIN->GetIsActive() == false) {
		startY--;
		endY--;
	}
	//if (INPUT->GetKey('S') && endY < MAXWINTILEY * 2) {
	if (g_ptMouse.y > WINSIZEY - WINSIZEY / 8 && endY < MAXWINTILEY * 2
		&& SUBWIN->GetIsActive() == false) {
		startY++;
		endY++;
	}

	for (int j = startX; j < endX; j++) {
		for (int i = startY; i < endY; i++) {
			tile.tile[i][j].x = j * img->GetFrameWidth() - startX * 25;
			tile.tile[i][j].y = i * img->GetFrameHeight() - startY * 25;
			tile.tile[i][j].rc = RectMake(tile.tile[i][j].x, tile.tile[i][j].y,
				img->GetFrameWidth(), img->GetFrameHeight());
		}
	}

	for (int j = startX; j < endX; j++) {
		for (int i = startY; i < endY; i++) {
			if (PtInRect(&tile.tile[i][j].rc, g_ptMouse)) {
				currentX = j;
				currentY = i;
			}
		}
	}

	if (INPUT->GetKey(VK_LBUTTON)
		&& SUBWIN->GetIsActive() == false) {
		for (int j = startX; j < endX; j++) {
			for (int i = startY; i < endY; i++) {
				if (PtInRect(&tile.tile[i][j].rc, g_ptMouse)) {
					//int x = tile[i][j].x;
					//int y = tile[i][j].y;
					//tile[i][j] = currentTile;
					//tile[i][j].x = x;
					//tile[i][j].y = y;
					int cnt = 0;
					if (vCurrentTile.size() != 0) {
							stack.push(tile);

						for (int l = j; l <= j + countX; l++) {
							if (l >= MAXWINTILEX * 2) break;
							for (int k = i; k <= i + countY; k++) {
								if (k >= MAXWINTILEY * 2) {
									cnt += currentY + countY - k + 1;
									break;
								}
								int x = tile.tile[k][l].x;
								int y = tile.tile[k][l].y;
								RECT rc = tile.tile[k][l].rc;
								tile.tile[k][l] = vCurrentTile[cnt++];
								tile.tile[k][l].x = x;
								tile.tile[k][l].y = y;
								tile.tile[k][l].rc = rc;
							}
						}
					}
				}
			}
		}
	}

	if (INPUT->GetKey(VK_RBUTTON)
		&& SUBWIN->GetIsActive() == false) {
		for (int j = startX; j < endX; j++) {
			for (int i = startY; i < endY; i++) {
				if (PtInRect(&tile.tile[i][j].rc, g_ptMouse)) {
					tile.tile[i][j].check = false;
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

		for (int j = startX; j < endX; j++) {
			for (int i = startY; i < endY; i++) {
				if (tile.tile[i][j].check == false) continue;
				img->FrameRender(GetMemDC(),
					tile.tile[i][j].x, tile.tile[i][j].y,
					tile.tile[i][j].tileX, tile.tile[i][j].tileY);
			}
		}

		if (vCurrentTile.size() != 0 
			&& SUBWIN->GetIsActive() == false) {
			int cnt = 0;
			for (int j = currentX; j <= currentX + countX; j++) {
				if (j >= MAXWINTILEX * 2) break;
				for (int i = currentY; i <= currentY + countY; i++) {
					if (i >= MAXWINTILEY * 2) {
						cnt += currentY + countY - i + 1;
						break;
					}
					img->FrameRender(GetMemDC(),
						tile.tile[i][j].x, tile.tile[i][j].y,
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

		for (int j = startX; j < endX; j++) {
			for (int i = startY; i < endY; i++) {
				RectangleMake(GetMemDC(), tile.tile[i][j].rc);
			}
		}
	}
	//=================================================
}

void ToolMain::InitTile()
{
	for (int j = 0; j < MAXWINTILEX * 2; j++) {
		for (int i = 0; i < MAXWINTILEY * 2; i++) {
			tile.tile[i][j].check = false;
		}
	}
}

void ToolMain::SaveTile()
{
	//string temp;
	//char temp[128];
	//TEXTDATA->CreateWriteTextDataHandle((char*)"save/mapInfo.txt");
	TEXTDATA->CreateWriteTextDataFilePointer((char*)"save/mapInfo.txt");

	for (int j = 0; j < MAXWINTILEX * 2; j++) {
		for (int i = 0; i < MAXWINTILEY * 2; i++) {

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
			sprintf_s(str, "%d", tile.tile[i][j].x);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile.tile[i][j].y);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile.tile[i][j].tileX);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile.tile[i][j].tileY);
			vStr.push_back(str);
			sprintf_s(str, "%d", tile.tile[i][j].check);
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

	for (int j = 0; j < MAXWINTILEX * 2; j++) {
		for (int i = 0; i < MAXWINTILEY * 2; i++) {
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

			vector<string> vStr = vvStr[j * MAXWINTILEY * 2 + i];

			tile.tile[i][j].x = atoi(vStr[0].c_str());
			tile.tile[i][j].y = atoi(vStr[1].c_str());
			tile.tile[i][j].tileX = atoi(vStr[2].c_str());
			tile.tile[i][j].tileY = atoi(vStr[3].c_str());
			tile.tile[i][j].check = atoi(vStr[4].c_str());

			tile.tile[i][j].rc = RectMake(tile.tile[i][j].x, tile.tile[i][j].y,
				img->GetFrameWidth(), img->GetFrameHeight());
		}
	}

	//TEXTDATA->CloseTextDataHandle();
	TEXTDATA->CloseTextDataFilePointer();
}

void ToolMain::RestoreTile()
{
	if (stack.empty()) return;

	// 마우스 키 입력이라 같은 타일 정보가 들어가는 경우가 있음 
	// 그 경우 다 빼버리려고 하는거
	while (true) {
		bool isSame = true;
		for (int j = 0; j < MAXWINTILEX * 2; j++) {
			for (int i = 0; i < MAXWINTILEY * 2; i++) {
				if (tile.tile[i][j].check != stack.top().tile[i][j].check) {
					isSame = false;
					break;
				}
			}
			if (isSame == false)
				break;
		}

		if (isSame)
			stack.pop();
		else
			break;
	}

	tile = stack.top();
	stack.pop();
}
