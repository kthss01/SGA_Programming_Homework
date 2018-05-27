#include "stdafx.h"
#include "Collision.h"

#include "./Common/Transform.h"
#include "./Physics/RectCollider.h"

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

bool Collision::IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Transform* transform, Vector2 mousePos)
{
	Vector2 cross;	// 각도를 계산하는 기준 vector

	float dotA, dotB;	// 사이 각도

	Vector2 temp[3];	// 계산할 녀석, 방향 벡터임

						// 내부 계산이므로 view projection 필요
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;

	D2D::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D2D::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);

	Matrix view = Matrix(matView);
	Matrix proj = Matrix(matProjection);
	Matrix world = transform->GetFinalMatrix();

	Vector2 point[3];
	point[0] = p0.TransformCoord(world);
	point[1] = p1.TransformCoord(world);
	point[2] = p2.TransformCoord(world);

	for (int i = 0; i < 3; i++) {
		point[i] = point[i].TransformCoord(view);
		point[i] = point[i].TransformCoord(proj);
	}

	temp[0].x = point[2].x - point[0].x;
	temp[0].y = point[2].y - point[0].y;

	temp[1].x = point[1].x - point[0].x;
	temp[1].y = point[1].y - point[0].y;

	temp[2].x = mousePos.x - point[0].x;
	temp[2].y = mousePos.y - point[0].y;

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

	temp[0].x = point[0].x - point[1].x;
	temp[0].y = point[0].y - point[1].y;

	temp[1].x = point[2].x - point[1].x;
	temp[1].y = point[2].y - point[1].y;

	temp[2].x = mousePos.x - point[1].x;
	temp[2].y = mousePos.y - point[1].y;

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

	temp[0].x = point[1].x - point[2].x;
	temp[0].y = point[1].y - point[2].y;

	temp[1].x = point[0].x - point[2].x;
	temp[1].y = point[0].y - point[2].y;

	temp[2].x = mousePos.x - point[2].x;
	temp[2].y = mousePos.y - point[2].y;

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

bool Collision::IsOverlap(Transform * pTransA, CircleCollider * pBoundA, Transform * pTransB, CircleCollider * pBoundB)
{
	Vector2 centerA;
	float radiusA;
	Vector2 centerB;
	float radiusB;

	// 같은 공간에 있어서 따로 처리 안해도 됨
	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	Vector2 dirTo = centerB - centerA;

	float dist = dirTo.Length();

	float distPow = (dist * dist);
	float sumRadiusPow = radiusA + radiusB;
	sumRadiusPow *= sumRadiusPow;

	if (distPow > sumRadiusPow) return false;
	return true;
}

bool Collision::IsOverlap(Transform * pTransA, RectCollider * pBoundA, Transform * pTransB, RectCollider * pBoundB)
{
	// Rect를 감싸고 있는 원끼리 충돌 먼저 체크
	if (!Collision::IsOverlap(
		pTransA, (CircleCollider*)pBoundA,
		pTransB, (CircleCollider*)pBoundB)) return false;

	Vector2 centerA;
	float radiusA;
	Vector2 centerB;
	float radiusB;

	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	struct OBB {
		Vector2 center;
		Vector2 axis[2];
		float halfLength[2];
	};

	OBB obbA;
	obbA.axis[0] = pTransA->GetRight();
	obbA.axis[1] = pTransA->GetUp();

	obbA.center = centerA;
	Vector2 scaleA = pTransA->GetScale();
	obbA.halfLength[0] = pBoundA->GetHalfSize().x * scaleA.x;
	obbA.halfLength[1] = pBoundA->GetHalfSize().y * scaleA.y;

	OBB obbB;
	obbB.axis[0] = pTransB->GetRight();
	obbB.axis[1] = pTransB->GetUp();

	obbB.center = centerB;
	Vector2 scaleB = pTransB->GetScale();
	obbB.halfLength[0] = pBoundB->GetHalfSize().x * scaleB.x;
	obbB.halfLength[1] = pBoundB->GetHalfSize().y * scaleB.y;

	// OBB 충돌
	float cos[2][2]; // 각축 차에 대한 코사인 값 [A축][B축]
	float absCos[2][2]; // 절대값
	float dist[2]; // 중심점을 투영한 길이값 

				   // A에서 B의 방향 벡터
	Vector2 D = obbB.center - obbA.center;

	float r, r0, r1;
	// r D를 투영한 값 
	// r0 A에서 투영한 값, r1 B에서 투영한 값
	// r > r0 + r1 이면 충돌 실패

	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			cos[a][b] = Vector2::Dot(obbA.axis[a], obbB.axis[b]);
			absCos[a][b] = abs(cos[a][b]);
		}

		// 센터끼리의 방향벡터를 A바운드
		// Axis으 투영한 길이
		dist[a] = Vector2::Dot(obbA.axis[a], D);
		// Dot 내적으로 사용하거나(벡터 두개 곱해주거나) 
		// cos 값을 구하거나 할 수 있음
	}

	// 중심점에 대해서 충돌 되었는지 확인

	r = abs(dist[0]);
	r0 = obbA.halfLength[0];
	/*
	풀어서 설명하면 이렇게 되는데 미리 계산해둠
	r1 = abs(Vector2::Dot(obbA.axis[0], obbB.axis[0] * obbB.halfLength[0]))
	+ abs(Vector2::Dot(obbA.axis[0], obbB.axis[1] * obbB.halfLength[1]))
	//+ abs(Vector2::Dot(obbA.axis[0], obbB.axis[0] * obbB.halfLength[2]))
	*/
	r1 = absCos[0][0] * obbB.halfLength[0] +
		absCos[0][1] * obbB.halfLength[1];

	if (r > r0 + r1) return false;

	// 다른 축도 해주어야함
	r = abs(dist[1]);
	r0 = obbA.halfLength[1];
	r1 = absCos[1][0] * obbB.halfLength[0] +
		absCos[1][1] * obbB.halfLength[1];

	if (r > r0 + r1) return false;

	// 반대 쪽도 해주어야함
	r = abs(Vector2::Dot(obbB.axis[0], D));
	r0 = absCos[0][0] * obbA.halfLength[0] +
		absCos[1][0] * obbA.halfLength[1];
	r1 = obbB.halfLength[0];

	if (r > r0 + r1) return false;

	r = abs(Vector2::Dot(obbB.axis[1], D));
	r0 = absCos[0][1] * obbA.halfLength[0] +
		absCos[1][1] * obbA.halfLength[1];
	r1 = obbB.halfLength[1];

	if (r > r0 + r1) return false;

	// 외적으로도 확인해봐야 함 (충돌이 아닌데 충돌된다고 나오는 경우가 있음)
	// 3D에선 중심점은 길어지는데 겹치지 않는 경우가 생긴다고 하심
	// 2D에서는 z축이 없어서 문제가 안된다고 하심

	return true;
}