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
	// value-계산할 값, 계산할 값이 min 보다 작으면 min으로 max보다 크면 max로 해주는
	// min max 사이값으로 반환
	static float Clamp(float value, float min, float max);
	// value값 무조건 0, 1 사이
	static float Clamp01(float value);

};

