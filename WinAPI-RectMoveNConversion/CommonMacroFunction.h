#pragma once

//////////////////////////////////////////////////////////////////
// CommonMacroFunction (필요한 부분은 직접 만들어서 추가 할것)
//////////////////////////////////////////////////////////////////

// POINT -> (LONG x, LONG y) 되어 있는 구조체

// inline 중복선언 피하는거

inline POINT PointMake(int x, int y) {
	POINT pt = { x, y };
	return pt;
}

inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

inline RECT RectMake(int x, int y, int width, int height) {
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

inline void RectangleMake(HDC hdc, int x, int y, int width, int height) {
	Rectangle(hdc, x, y, x + width, y + height);
}

inline void RectangleMake(HDC hdc, RECT rect) {
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

inline bool Collision(RECT rect1, RECT rect2) {

	if ((rect1.left <= rect2.left && rect2.left <= rect1.right)
		|| (rect1.left <= rect2.right && rect2.right <= rect1.right)) {
		if ((rect1.top <= rect2.top && rect2.top <= rect1.bottom)
			|| (rect1.top <= rect2.bottom && rect2.bottom <= rect1.bottom)) {
			return true;
		}
	}
	return false;
}

// 점과 사각형이 충돌하는지 체크
inline bool Collision(int x, int y, RECT rect) {
	if (rect.left <= x && x <= rect.right) {
		if (rect.top <= y && y <= rect.bottom) {
			return true;
		}
	}
	return false;
}

// 점과 사각형이 충돌하는지 체크2 사각형의 변수들로 인자 받기
inline bool Collision(int x, int y, 
	int left, int top, int right, int bottom) {
	if (left <= x && x <= right) {
		if (top <= y && y <= bottom) {
			return true;
		}
	}
	return false;
}

// 스위치문 안에서 내부에서 사각형 충돌이 일어났는지 확인
inline bool InnerCollision(RECT rect1, RECT rect2) {

	if ((rect1.left <= rect2.left && rect2.left <= rect1.right)
		&& (rect1.left <= rect2.right && rect2.right <= rect1.right)) {
		if ((rect1.top <= rect2.top && rect2.top <= rect1.bottom)
			&& (rect1.top <= rect2.bottom && rect2.bottom <= rect1.bottom)) {
			return false;
		}
	}
	return true;
}

// 방향까지 확인하여 스위치문 끝나고 나서 내부에서 사각형 충돌 여부 확인
inline bool InnerCollision(RECT rect1, RECT rect2, int dir) {
	switch (dir) {
	case VK_LEFT:
		if (rect1.right <= rect2.right)
			return true;
		else
			return false;
		break;
	case VK_RIGHT:
		if (rect1.left >= rect2.left)
			return true;
		else
			return false;
		break;
	case VK_UP:
		if (rect1.bottom <= rect2.bottom)
			return true;
		else
			return false;
		break;
	case VK_DOWN:
		if (rect1.top >= rect2.top)
			return true;
		else
			return false;
		break;
	default:
		return false;
		break;
	}
}