#pragma once
inline POINT PointMake(int x, int y) {
	POINT pt = { x,y };
	return pt;
}

inline void LineMake(HDC hdc, POINT p1, POINT p2) {
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);
}

inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

inline RECT RectMake(int x, int y, int width, int height) {
	RECT rc = { x,y,x + width,y + height };
	return rc;
}

inline RECT RectMake(int x, int y, float width, float height) {
	RECT rc = { x,y,x + width,y + height };
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

inline RECT RectMakeCenter(int x, int y, int width, int height) {
	RECT rc = { x - width / 2, y - height / 2,
		x + width / 2, y + height / 2 };
	return rc;
}

inline RECT RectMakeCenter(int x, int y, float width, float height) {
	RECT rc = { x - width / 2, y - height / 2,
		x + width / 2, y + height / 2 };
	return rc;
}

inline void EllipseMakeCenter(HDC hdc, int x, int y, int radius) {
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}
