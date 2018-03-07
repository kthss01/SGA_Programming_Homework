#include "stdafx.h"
#include "Util.h"

namespace MY_UTIL
{
	float GetAngle(float startX, float startY, float endX, float endY) {
		float x = endX - startX;
		float y = endY - startY;

		float distance = sqrtf(x * x + y * y);
		float angle = acosf(x / distance);

		if (endY > startY) {
			angle = -angle;
			if (angle >= PI * 2) angle -= PI * 2;
		}
		return angle;
	}
	float GetDistance(float startX, float startY, float endX, float endY) {
		float x = endX - startX;
		float y = endY - startY;

		float distance = sqrtf(x * x + y * y);
		return distance;
	}
}