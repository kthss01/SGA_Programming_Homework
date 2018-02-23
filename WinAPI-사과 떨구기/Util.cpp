#include "stdafx.h"
#include "Util.h"

// namespace 이름 같으면 그 안에꺼 공유 됨
namespace MY_UTIL
{
	float GetAngle(float startX, float startY, float endX, float endY)
	{
		// deltat x , y 구한거
		float x = endX - startX;
		float y = endY - startY;

		float distance = sqrtf(x * x + y * y);
		float angle = acosf(x / distance);

		if (endY > startY) {
			angle = -angle;
			if (angle >= M_PI * 2) angle -= M_PI * 2;
		}
		return angle;
	}
	float GetDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		float distance = sqrtf(x * x + y * y);
		return distance;
	}
}
