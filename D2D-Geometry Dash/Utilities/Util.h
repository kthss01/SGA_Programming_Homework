#pragma once

typedef struct tagRay {
	Vector2 origin;
	Vector2 direction;
	tagRay() {
		origin = Vector2(0, 0);
		direction = Vector2(0, 0);
	}
	tagRay(Vector2 origin, Vector2 direction) {
		this->origin = origin;
		this->direction = direction;
	}
} Ray, *LPRay;

class Util
{
public:
	Util();
	~Util();

	static void GetMousePos(Vector2 * out);
	static float GetRandomSeed();
};

