#pragma once

// 싱글턴으로 만들 예정
#define CIRCLEGIZMO_SEGMENTS 36

// 0xff808080 컬러값 중 중간값

// 기즈모는 다른 오브젝트(월드)에 영향을 받으면 안됨

class Gizmo
{
private:
	typedef struct tagGIZMOVERTEX {
		Vector2 position;
		DWORD color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	} GIZMOVERTEX, *LPGIZMOVERTEX;

private:
	static Gizmo* instance;

	Gizmo();
	~Gizmo();
public:
	static Gizmo* Get();
	static void Delete();

	// 선 그리기
	void Line(const Vector2& startPos, const Vector2& endPos, DWORD color = 0xff808080) const;

	void Circle(Vector2& centerPos, float radius, DWORD color = 0xff808080) const;

	void AABBBox(const Vector2& minPos, const Vector2& maxPos, DWORD color = 0xff808080) const;

	// 월드 그리드 3D에선 높이값 (높이 확인이 힘듬), 2D에선 얼마만큼 떨어져있는지 
	// 격자무늬로 되있는거 -> 라인을 여러개 그리면 됨 
};

#define GIZMO Gizmo::Get()
