#include "stdafx.h"
#include "Collision.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 mousePos)
{
	Vector2 cross;	// 각도를 계산하는 기준 vector

	float dotA, dotB;	// 사이 각도

	Vector2 temp[3];	// 계산할 녀석, 방향 벡터임

	temp[0].x = p2.x - p0.x;
	temp[0].y = p2.y - p0.y;

	temp[1].x = p1.x - p0.x;
	temp[1].y = p1.y - p0.y;

	temp[2].x = mousePos.x - p0.x;
	temp[2].y = mousePos.y - p0.y;

	// 2D에서 사용하는 외적 
	// temp[0]에 수직인 벡터가 만들어짐 (노말벡터, 법선벡터)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 둔각이라는 180보다 높다
	// dot이 내적 90보다 작으면 > 0 180보다 크면 음수
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// 이 경우인 경우 삼각형 밖으로 나가게됨
	// 몇몇 경우는 계산 더 해주면 되고
	if (dotA * dotB < 0)
		return false;

	temp[0].x = p0.x - p1.x;
	temp[0].y = p0.y - p1.y;

	temp[1].x = p2.x - p1.x;
	temp[1].y = p2.y - p1.y;

	temp[2].x = mousePos.x - p1.x;
	temp[2].y = mousePos.y - p1.y;

	// 2D에서 사용하는 외적 
	// temp[0]에 수직인 벡터가 만들어짐 (노말벡터, 법선벡터)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 둔각이라는 180보다 높다
	// dot이 내적 90보다 작으면 > 0 180보다 크면 음수
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// 이 경우인 경우 삼각형 밖으로 나가게됨
	// 몇몇 경우는 계산 더 해주면 되고
	if (dotA * dotB < 0)
		return false;

	temp[0].x = p1.x - p2.x;
	temp[0].y = p1.y - p2.y;

	temp[1].x = p0.x - p2.x;
	temp[1].y = p0.y - p2.y;

	temp[2].x = mousePos.x - p2.x;
	temp[2].y = mousePos.y - p2.y;

	// 2D에서 사용하는 외적 
	// temp[0]에 수직인 벡터가 만들어짐 (노말벡터, 법선벡터)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 둔각이라는 180보다 높다
	// dot이 내적 90보다 작으면 > 0 180보다 크면 음수
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// 이 경우인 경우 삼각형 밖으로 나가게됨
	// 몇몇 경우는 계산 더 해주면 되고
	if (dotA * dotB < 0)
		return false;

	return true;
}

bool Collision::IntersectRayToLine(Ray ray, Vector2 start, Vector2 end, Vector2 * outPos, Vector2 * outNormal)
{
	Vector2 normal;
	normal.x = -(end.y - start.y);
	normal.y = (end.x - start.x);

	// 각도로 쓸 수도 있고
	// 벡터의 곱으로도 쓸 수 있음
	// normal 과 꼭지점과의 최단 거리 구한거
	float dist = -Vector2::Dot(normal, end);
	float t1 = -Vector2::Dot(normal, ray.origin);
	float t2 = Vector2::Dot(normal, ray.direction);
	float t = (t1 - dist) / t2;

	// 직교와 예각인 상황일 때 (수평, 직교 둘 다 포함)
	if (t >= 0.0f && t <= 1.0f) {
		if (outPos != NULL) {
			*outPos = ray.origin + ray.direction * t;

			if (outNormal != NULL) {
				*outNormal = normal * -1;
			}
		}
		return true;
	}
	return false;
}
