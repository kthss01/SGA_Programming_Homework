#include "stdafx.h"
#include "TestScene.h"


TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

HRESULT TestScene::Init()
{
	isDebug = false;

	for (int i = 0; i < POKEMON_MAX; i++) {
		pokemon[i].img = IMAGE->FindImage("pokemon");
		pokemon[i].row = RND->GetFromInto(0, 12);
		if (pokemon[i].row != 12)
			pokemon[i].col = RND->GetFromInto(0, 11);
		else
			pokemon[i].col = RND->GetFromInto(0, 6);
		pokemon[i].radius = 50;
		pokemon[i].x = RND->GetFromInto(
			pokemon[i].radius, WINSIZEX - pokemon[i].radius - PADDINGX);
		pokemon[i].y = RND->GetFromInto(
			pokemon[i].radius, WINSIZEY - pokemon[i].radius);
		pokemon[i].speed = 1.0f;
		pokemon[i].angle = RND->GetFromInto(0, 360) * PI / 180;
		pokemon[i].rc =
			RectMakeCenter(pokemon[i].x, pokemon[i].y,
				pokemon[i].img->GetFrameWidth(), pokemon[i].img->GetFrameHeight());
		pokemon[i].isLive = true;
	}

	gun.x = (WINSIZEX - PADDINGX) / 2;
	gun.y = WINSIZEY;
	gun.radius = 50;
	gun.rc = RectMakeCenter(gun.x, gun.y, gun.radius, gun.radius);
	gun.angle = 90 * PI / 180;
	gun.length = 50.0f;

	bullet = new Bullet;
	bullet->Init((char*)"bullet", 100, 1000);

	count = 0;

	SOUND->Play("bgm", 0.5f);

	return S_OK;
}

void TestScene::Release()
{
	SAFE_DELETE(bullet);
}

void TestScene::Update()
{
	if (INPUT->GetKey(VK_LEFT) 
		&& gun.angle < PI - 10 * PI / 180) { gun.angle += 2.0f * PI / 180; }
	if (INPUT->GetKey(VK_RIGHT)
		&& gun.angle > 10 * PI / 180) { gun.angle -= 2.0f * PI / 180; }

	count++;

	if (INPUT->GetKey(VK_SPACE) && count % 5 == 0) {
		SOUND->Play("fire", 0.5f);
		bullet->Fire(gun.x + cosf(gun.angle) * gun.length, 
			gun.y - sinf(gun.angle) * gun.length, gun.angle, 2.5f);
	}

	Move();
	bullet->Update();

	for (int i = 0; i < POKEMON_MAX; i++) {
		if (pokemon[i].isLive == true &&
			bullet->CheckCollision(pokemon[i].rc)) {
			pokemon[i].isLive = false;
			pokemon[i].diedTime = FRAME->GetWorldTime();
			SOUND->Play("die", 1.0f);
		}

		if (pokemon[i].isLive == false &&
			FRAME->GetWorldTime() - pokemon[i].diedTime >= RESPAWNTIME) {
			pokemon[i].isLive = true;
			pokemon[i].diedTime = 0;

			pokemon[i].x = RND->GetFromInto(
				pokemon[i].radius, WINSIZEX - pokemon[i].radius - PADDINGX);
			pokemon[i].y = RND->GetFromInto(
				pokemon[i].radius, WINSIZEY - pokemon[i].radius);
			pokemon[i].speed = 1.0f;
			pokemon[i].angle = RND->GetFromInto(0, 360) * PI / 180;
			pokemon[i].rc =
				RectMakeCenter(pokemon[i].x, pokemon[i].y,
					pokemon[i].img->GetFrameWidth(), pokemon[i].img->GetFrameHeight());
		}
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void TestScene::Render()
{
	//=================================================
	{
		IMAGE->FindImage("bg")->Render(GetMemDC());
		
		LineMake(GetMemDC(), gun.x, gun.y,
			gun.x + cosf(gun.angle) * gun.length,
			gun.y - sinf(gun.angle) * gun.length);
		EllipseMake(GetMemDC(), gun.rc);

		for (int i = 0; i < POKEMON_MAX; i++) {
			if (pokemon[i].isLive == false) continue;

			pokemon[i].img->FrameRender(GetMemDC(),
				pokemon[i].rc.left, pokemon[i].rc.top, 
				pokemon[i].col, pokemon[i].row);
		}
		
		bullet->Render();

		for (int i = 0; i < POKEMON_MAX; i++) {
			float alpha = 255;
			if(pokemon[i].isLive)
				pokemon[i].img->FrameRender(GetMemDC(),
					WINSIZEX - PADDINGX + 10, i * 75,
					pokemon[i].col, pokemon[i].row, alpha);
			else {
				alpha = 128;
				pokemon[i].img->FrameRender(GetMemDC(),
					WINSIZEX - PADDINGX + 10, i * 75,
					pokemon[i].col, pokemon[i].row, alpha);

			}
			
			SetTextColor(GetMemDC(), RGB(0, 0, 0));

			sprintf_s(str, "상태 : %s", pokemon[i].isLive ? "Live" : "Die");
			TextOut(GetMemDC(), WINSIZEX - PADDINGX + 110, i * 75 + 25, str, strlen(str));

			if (!pokemon[i].isLive) {
				sprintf_s(str, "리스폰 시간 : %.2fs",
					RESPAWNTIME - (FRAME->GetWorldTime() - pokemon[i].diedTime));
				TextOut(GetMemDC(), WINSIZEX - PADDINGX + 110, i * 75 + 50, str, strlen(str));
			}
		}
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
}

void TestScene::Move() {
	for (int i = 0; i < POKEMON_MAX; i++) {
		if (pokemon[i].isLive == false) continue;

		pokemon[i].x += cosf(pokemon[i].angle) * pokemon[i].speed;
		pokemon[i].y -= sinf(pokemon[i].angle) * pokemon[i].speed;

		// 벽 충돌
		// left
		if (pokemon[i].x - pokemon[i].radius < 0) {
			pokemon[i].x = pokemon[i].radius;
			pokemon[i].angle = PI - pokemon[i].angle;
		}
		// right
		if (pokemon[i].x + pokemon[i].radius > WINSIZEX - PADDINGX) {
			pokemon[i].x = WINSIZEX - pokemon[i].radius - PADDINGX;
			pokemon[i].angle = PI - pokemon[i].angle;
		}
		// up
		if (pokemon[i].y - pokemon[i].radius < 0) {
			pokemon[i].y = pokemon[i].radius;
			pokemon[i].angle = 2 * PI - pokemon[i].angle;
		}
		// bottom
		if (pokemon[i].y + pokemon[i].radius > WINSIZEY) {
			pokemon[i].y = WINSIZEY - pokemon[i].radius;
			pokemon[i].angle = 2 * PI - pokemon[i].angle;
		}

		for (int j = 0; j < POKEMON_MAX; j++) {
			if (i == j) continue;

			float length = GetDistance(
				pokemon[i].x, pokemon[i].y, pokemon[j].x, pokemon[j].y);

			if (length > pokemon[i].radius + pokemon[j].radius) continue;

			pokemon[i].angle = GetAngle(
				pokemon[j].x, pokemon[j].y, pokemon[i].x, pokemon[i].y);
			pokemon[j].angle = GetAngle(
				pokemon[i].x, pokemon[i].y, pokemon[j].x, pokemon[j].y);
		}

		pokemon[i].rc =
			RectMakeCenter(pokemon[i].x, pokemon[i].y,
				pokemon[i].img->GetFrameWidth(), pokemon[i].img->GetFrameHeight());
	}
}
