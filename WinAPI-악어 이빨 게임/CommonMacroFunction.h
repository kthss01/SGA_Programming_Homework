#pragma once
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

inline void RectangleMake(HDC hdc, RECT rect) {
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

inline void RectangleMake(HDC hdc, int left, int top, int right, int bottom) {
	Rectangle(hdc, left, top, right, bottom);
}

inline void RectangleMake2(HDC hdc, int x, int y, int width, int height) {
	Rectangle(hdc, x, y, x + width, y + height);
}

// x,y 가 중점으로해서 거기서 반지름만큼 위아래 좌우로 사각형 만드는거
inline RECT RectMakeCenter(int x, int y, int width, int height) {
	RECT rc = { x - width / 2, y - height / 2,
		x + width / 2, y + height / 2 };

	return rc;
}