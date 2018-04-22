#pragma once

namespace MY_UTIL {
	float GetAngle(float startX, float startY, float endX, float endY);

	float GetDistance(float startX, float startY, float endX, float endY);

	bool IntersectLine(POINT FLineP1, POINT FLineP2,
		POINT SLineP1, POINT SLineP2);

	// Ray 반직선 - 시작지점이 있는 직선 (값만 앞으로 나가는거)
	// 반직선은 라인과 다름 게임 실행될 때는 보여주는건 아님 
	// 개발할때만 사용 (디버그용도로 사용)
	typedef struct tagRay {
		POINT origin;		// 시작 위치
		POINT direction;	// 방향

		tagRay() {
			origin = { 0, 0 };
			direction = { 0, 0 };
		}
		tagRay(POINT origin, POINT direction) {
			this->origin = origin;
			this->direction = direction;
		}
		tagRay(int originX, int originY, int directionX, int directionY) {
			origin = { originX, originY };
			direction = { directionX, directionY };
		}

		void DrawRay(HDC hdc);
		bool CollisionRect(RECT rc);
	} Ray, *LPRay;
}