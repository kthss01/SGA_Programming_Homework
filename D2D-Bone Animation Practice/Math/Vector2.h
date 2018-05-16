#pragma once

namespace MyDirectX
{
	struct Vector2
	{
		float x, y, z = 0; 

		Vector2();
		Vector2(float x, float y);
		Vector2(float x, float y, float z);

		bool operator==(const Vector2& vec);
		bool operator!=(const Vector2& vec);

		Vector2 operator+(const Vector2& vec);
		Vector2 operator-(const Vector2& vec);
		Vector2 operator*(float f); //½ºÄ®¶ó °ö

		float Length();
		Vector2 Normalize();
		static float Dot(Vector2& v1, Vector2& v2);
		static Vector2 Cross(Vector2& v1, Vector2& v2);

		Vector2 TransformCoord(struct Matrix& mat);
		Vector2 TransformNormal(struct Matrix& mat);
	};
}
