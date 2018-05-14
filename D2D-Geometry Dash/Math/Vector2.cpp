#include "stdafx.h"
#include "Vector2.h"
namespace MyDirectX
{
	Vector2::Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2::Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2::Vector2(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	bool Vector2::operator==(const Vector2 & vec)
	{

		if (fabsf(this->x - vec.x) > EPSILON) return false;
		if (fabsf(this->y - vec.y) > EPSILON) return false;
		if (fabsf(this->z - vec.z) > EPSILON) return false;
		return true;
	}
	bool Vector2::operator!=(const Vector2 & vec)
	{
		return !(*this == vec);

	}
	Vector2 Vector2::operator+(const Vector2 & vec)
	{
		return Vector2(x + vec.x, y + vec.y, z + vec.z);
	}
	Vector2 Vector2::operator-(const Vector2 & vec)
	{
		return Vector2(x - vec.x, y - vec.y, z - vec.z);

	}
	Vector2 Vector2::operator*(float f)
	{
		return Vector2(x * f, y * f, z * f);

	}
	float Vector2::Length()
	{
		return sqrtf(x * x + y * y + z * z);

	}
	Vector2 Vector2::Normalize()
	{
		float len = Length();
		return Vector2(x / len, y / len, z / len);
	}
	float Vector2::Dot(Vector2 & v1, Vector2 & v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	}
	Vector2 Vector2::Cross(Vector2 & v1, Vector2 & v2)
	{

		Vector2 temp;

		temp.x = v1.y * v2.z - v1.z * v2.y;
		temp.y = v1.z * v2.x - v1.x * v2.z;
		temp.z = v1.x * v2.y - v1.y * v2.x;

		return temp;
	}
	Vector2 Vector2::TransformCoord(Matrix & mat)
	{
		Vector2 v;
		v.x = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z + mat[3][0];
		v.y = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z + mat[3][1];
		v.z = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z + mat[3][2];
		float w = mat[0][3] * x + mat[1][3] * y + mat[2][3] * z + mat[3][3];
		if (fabs(w) > 0.0001f)
		{
			v.x /= w;
			v.y /= w;
			v.z /= w;
		}
		return v;
	}
	Vector2 Vector2::TransformNormal(Matrix & mat)
	{
		Vector2 v;
		v.x = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z;
		v.y = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z;
		v.z = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z;
		return v;
	}
}