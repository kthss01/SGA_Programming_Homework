#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
	:pParent(NULL), pFirstChild(NULL), pNextSibling(NULL)
{
	bAutoUpdate = true;
	this->Reset();

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

Transform::~Transform()
{
}

void Transform::AddChild(Transform * pNewChild)
{
	// 새로들어간 자식의 부모가 나면 리턴
	if (pNewChild->pParent == this)
		return;

	// 이전 부모와 연결을 끊는다.
	pNewChild->ReleaseParent();

	// 부모의 상대적인 좌표값으로 갱신하기 위해서 
	// 부모의 final 역행렬 값
	Matrix matInvFinal;
	float temp;

	matInvFinal = this->matFinal.Inverse(temp);
	pNewChild->position =
		pNewChild->position.TransformCoord(matInvFinal);

	for (int i = 0; i < 2; i++)
		pNewChild->axis[i] =
		pNewChild->axis[i].TransformNormal(matInvFinal);

	pNewChild->scale.x = pNewChild->right.Length();
	pNewChild->scale.y = pNewChild->up.Length();

	pNewChild->pParent = this;

	Transform* pChild = this->pFirstChild;

	if (pChild == NULL) {
		this->pFirstChild = pNewChild;
	}
	else {
		while (pChild != NULL) {
			if (pChild->pNextSibling == NULL) {
				pChild->pNextSibling = pNewChild;
				break;
			}
			pChild = pChild->pNextSibling;
		}
	}
	this->UpdateTransform();
}

void Transform::AttachTo(Transform * pParent)
{
}

void Transform::ReleaseParent()
{
}

void Transform::Reset(int resetFlag)
{
	if (resetFlag & RESET_POSITION) {
		this->position.x = 0;
		this->position.y = 0;
	}
	if (resetFlag & RESET_ROTATION) {
		this->right = Vector2(1, 0);
		this->up = Vector2(0, 1);
	}
	if (resetFlag & RESET_SCALE) {
		this->scale = Vector2(1, 1);
	}
}

void Transform::SetWorldPosition(Vector2 position)
{
	// 부모 존재하면 부모 기준으로 설정해야됨
	if (this->pParent != NULL) {
		// 부모의 최종행렬 뒤집은거 
		Matrix matInvParentFinal;
		float temp;
		matInvParentFinal = pParent->matFinal.Inverse(temp);

		// 자식의 matFinal은 자식 월드 * 부모의 월드
		// 따라서 부모이 역행렬로 제거 해야됨
		// 계산 후 다시 부모 월드 곱하는거

		// 부모의 역행렬값만큼 이동
		position = position.TransformCoord(matInvParentFinal);
	}
	this->position.x = position.x;
	this->position.y = position.y;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetLocalPosition(Vector2 position)
{
	this->position.x = position.x;
	this->position.y = position.y;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void Transform::MovePositionSelf(Vector2 delta)
{
	// 이동 벡터
	Vector2 move;

	// 자신의 이동 축을 얻는다
	Vector2 moveAxis[2];
	this->GetUnitAxis(moveAxis);

	move = move + moveAxis[0] * delta.x;
	move = move + moveAxis[1] * delta.y;

	Vector2 nowWorldPos = this->GetWorldPosition();
	this->SetWorldPosition(nowWorldPos + move);
}

void Transform::MovePositionWorld(Vector2 delta)
{
	Vector2 nowWorldPos = this->GetWorldPosition();

	this->SetWorldPosition(nowWorldPos + delta);
}

void Transform::MovePositionLocal(Vector2 delta)
{
}

void Transform::SetScale(Vector2 scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetScaling(Vector2 deltaScale)
{
	this->scale.x += deltaScale.x;
	this->scale.y += deltaScale.y;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

// axis를 world 축으로 설정해주어햐함
void Transform::RotateWorld(float angle)
{
	if (this->pParent) {
		Vector2 worldAxis[2];
		this->GetUnitAxis(worldAxis);

		Matrix matRotateZ;
		matRotateZ = matRotateZ.Rotate(angle);

		Matrix matInvParentFinal;
		float temp;
		matInvParentFinal = this->pParent->matFinal.Inverse(temp);

		Matrix matRot = matRotateZ * matInvParentFinal;

		for (int i = 0; i < 2; i++)
			this->axis[i] = this->axis[i].TransformNormal(matRot);

		if (this->bAutoUpdate)
			this->UpdateTransform();
	}
	else {
		RotateLocal(angle);
	}
}

void Transform::RotateSelf(float angle)
{
	// 2D 라서 한방향만 회전하면 됨 z축으로
	Matrix matRotateZ;
	matRotateZ = matRotateZ.Rotate(angle);

	for (int i = 0; i < 2; i++)
		this->axis[i] = this->axis[i].TransformNormal(matRotateZ);

	if (this->bAutoUpdate)
		this->UpdateTransform();

}

void Transform::RotateLocal(float angle)
{
	// 2D 라서 한방향만 회전하면 됨 z축으로
	Matrix matRotateZ;
	matRotateZ = matRotateZ.Rotate(angle);

	for (int i = 0; i < 2; i++)
		this->axis[i] = this->axis[i].TransformNormal(matRotateZ);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateWorld(const Matrix matWorldRotate)
{

}

void Transform::SetRotateWorld(D3DXQUATERNION & worldRotate)
{
	D3DXQUATERNION quatRot = worldRotate;

	D3DXMATRIX matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	Matrix matRot = Matrix(matRotate);
	if (this->pParent) {
		Matrix matInvParentFinal;
		float temp;
		matInvParentFinal = this->pParent->matFinal.Inverse(temp);

		matRot = matRot * matInvParentFinal;
	}

	this->right = Vector2(1, 0);
	this->up = Vector2(0, 1);

	for (int i = 0; i < 2; i++)
		this->axis[i] = this->axis[i].TransformNormal(matRot);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetRotateLocal(const Matrix matLocalRotate)
{
}

void Transform::LookPosition(Vector2 pos, Vector2 up)
{
}

void Transform::ScaleLerp(Transform * from, Transform * to, float t)
{

	t = Util::Clamp01(t);

	D3DXVECTOR3 fromScale = from->scale.ToDXVector3();
	D3DXVECTOR3 toScale = to->scale.ToDXVector3();

	if (FLOATZERO(t)) {
		this->SetScale(Vector2(from->scale.x, from->scale.y));
	}
	else if (FLOATEQUAL(t, 1.0f)) {
		this->SetScale(Vector2(to->scale.x, to->scale.y));
	}
	else {
		D3DXVECTOR3 result;
		D3DXVec3Lerp(&result, &fromScale, &toScale, t);

		Vector2 vec = Vector2(result.x, result.y);
		this->SetScale(vec);
	}
}

void Transform::RotateSlerp(Transform * from, Transform * to, float t)
{
	// t 보간값 
	// 0 <= t <= 1
	// 0에 가까우면 from에 가깝고 1에 가까우면 to에 가까움
	// 0에 가까우면 많이 짤리게 되고
	// 1에 가까우면 적게 짤리게 된다는거 
	// t 짤리는 비율

	t = Util::Clamp01(t);
	D3DXQUATERNION fromQuat = from->GetWorldRotateQuaternion();
	D3DXQUATERNION toQuat = to->GetWorldRotateQuaternion();

	if (FLOATZERO(t)) {
		this->SetRotateWorld(fromQuat);
	}
	else if (FLOATEQUAL(t, 1.0f)) {
		this->SetRotateWorld(toQuat);
	}
	else {
		D3DXQUATERNION result;
		D3DXQuaternionSlerp(&result, &fromQuat, &toQuat, t);

		this->SetRotateWorld(result);
	}
}

void Transform::PositionLerp(Transform * from, Transform * to, float t)
{
	// 선형보간 

	t = Util::Clamp01(t);

	D3DXVECTOR3 fromWorldPos = from->GetWorldPosition().ToDXVector3();
	D3DXVECTOR3 toWorldPos = to->GetWorldPosition().ToDXVector3();

	if (FLOATZERO(t)) {
		this->SetWorldPosition(from->GetWorldPosition());
	}
	else if (FLOATEQUAL(t, 1.0f)) {
		this->SetWorldPosition(to->GetWorldPosition());
	}
	else {
		D3DXVECTOR3 result;
		D3DXVec3Lerp(&result, &fromWorldPos, &toWorldPos, t);

		Vector2 vec = Vector2(result.x, result.y);
		this->SetWorldPosition(vec);
	}
}

void Transform::Interpolate(Transform * from, Transform * to, float t)
{
	// 과제
	// scale은 선형보간
	// position은 선형보간
	// rotate 구면보간

	// auto 업데이트 끄고 값을 집어넣고 auto 업데이트 해줘야함
	bool bPrevAutoUpdate = this->bAutoUpdate;
	bAutoUpdate = false;

	PositionLerp(from, to, t);
	RotateSlerp(from, to, t);
	ScaleLerp(from, to, t);

	// -> SetScale
	// -> SetWorldPosition
	// -> SetRotateWorld

	this->bAutoUpdate = bPrevAutoUpdate;

	if (bAutoUpdate)
		this->UpdateTransform();
}

void Transform::DefaultControl2()
{
	float deltaTime = (float)Frame::Get()->GetFrameDeltaSec();
	//float deltaMove = 200.0f * deltaTime; // c++ 에선 10.f 이렇게 써도 됨
	float deltaMove = 5.0f;
	float deltaAngle = 90.0f * D3DX_PI / 180.0f * deltaTime;

	if (Input::Get()->GetKey(VK_RBUTTON) == false) {
		if (Input::Get()->GetKey('A'))
			this->MovePositionSelf(Vector2(-deltaMove, 0));
		else if (Input::Get()->GetKey('D'))
			this->MovePositionSelf(Vector2(deltaMove, 0));

		if (Input::Get()->GetKey('W'))
			this->MovePositionSelf(Vector2(0, -deltaMove));
		else if (Input::Get()->GetKey('S'))
			this->MovePositionSelf(Vector2(0, deltaMove));

		if (Input::Get()->GetKey('Q'))
			this->RotateSelf(-deltaAngle);
		else if (Input::Get()->GetKey('E'))
			this->RotateSelf(deltaAngle);

		if (Input::Get()->GetKey('Z'))
			this->SetScaling(Vector2(0.1f, 0.1f));
		if (Input::Get()->GetKey('X'))
			this->SetScaling(Vector2(-0.1f, -0.1f));
	}

}

void Transform::UpdateTransform()
{
	// 자신의 정보로 matLocal 행렬을 갱신
	this->matLocal = Matrix::Identity(4);
	this->matFinal = Matrix::Identity(4);

	// scale 값을 가진 축
	Vector2 scaledRight = this->right * this->scale.x;
	Vector2 scaledUp = this->up * this->scale.y;

	this->matLocal[0][0] = scaledRight.x;
	this->matLocal[0][1] = scaledRight.y;
	this->matLocal[1][0] = scaledUp.x;
	this->matLocal[1][1] = scaledUp.y;

	this->matLocal[3][0] = position.x;
	this->matLocal[3][1] = position.y;

	if (this->pParent == NULL) {
		this->matFinal = matLocal;
	}
	else {
		this->matFinal = matLocal * this->pParent->matFinal;
	}

	Transform* pChild = this->pFirstChild;
	while (pChild != NULL) {
		pChild->UpdateTransform();
		pChild = pChild->pNextSibling;
	}
}

Matrix Transform::GetFinalMatrix() const
{
	return matFinal;
}

Vector2 Transform::GetWorldPosition()
{
	Vector2 position = this->position;

	if (this->pParent)
		position = position.TransformCoord(pParent->matFinal);

	return position;
}

void Transform::GetUnitAxis(Vector2 * pVecArr) const
{
	for (int i = 0; i < 2; i++) {
		pVecArr[i] = axis[i];
		pVecArr[i] = pVecArr[i].Normalize();
	}

	if (this->pParent) {
		Matrix matParentFinal = this->pParent->matFinal;
		for (int i = 0; i < 2; i++) {
			pVecArr[i] = pVecArr[i].TransformNormal(matParentFinal);
		}
	}
}

Vector2 Transform::GetUnitAxis(int axisNum) const
{
	Vector2 result = this->axis[axisNum];
	result = result.Normalize();

	if (this->pParent) {
		Matrix matParentFinal = this->pParent->matFinal;

		result = result.TransformNormal(matParentFinal);
	}

	return result;
}

Vector2 Transform::GetScale() const
{
	return this->scale;
}

Matrix Transform::GetWorldRotateMatrix()
{
	Matrix matRotate;
	matRotate = Matrix::Identity(4);
	Vector2 axis[2];
	this->GetUnitAxis(axis);
	// 부모로 들어가면 정규화 필요

	matRotate[0][0] = axis[0].x;
	matRotate[0][1] = axis[0].y;

	matRotate[1][0] = axis[1].x;
	matRotate[1][1] = axis[1].y;

	// 3차원이면 z값까지 처리 필요

	return matRotate;
}
D3DXQUATERNION Transform::GetWorldRotateQuaternion()
{
	D3DXQUATERNION quat;

	// 월드 축으로 받아오는 녀석으로 해야됨
	// GetUnitAxis()를 월드축으로 받아와서 행렬로 변환 시켜서
	// 그녀석을 matWorld라는 녀석에 집어넣어야함

	D3DXMATRIX matWorld = this->GetWorldRotateMatrix().ToDXMatrix();

	// quat 값으로 변환됨 (행렬중 회전값만 반환됨)
	D3DXQuaternionRotationMatrix(&quat, &matWorld);

	return quat;
}

void Transform::DrawInterface()
{
#ifdef IMGUI_USE
	ImGui::Begin("Transform");
	{
		ImGui::Text("Postion");
		ImGui::InputFloat("X", &position.x, 1.0f);
		ImGui::InputFloat("Y", &position.y, 1.0f);

		ImGui::Text("AxisX");
		ImGui::InputFloat("X", &right.x, 1.0f);
		ImGui::InputFloat("Y", &right.y, 1.0f);

		ImGui::Text("AxisY");
		ImGui::InputFloat("X", &up.x, 1.0f);
		ImGui::InputFloat("Y", &up.y, 1.0f);

		ImGui::Text("Scale");
		ImGui::InputFloat("X", &scale.x, 1.0f);
		ImGui::InputFloat("Y", &scale.y, 1.0f);

		if (ImGui::Button("Reset"))
			this->Reset();
		this->UpdateTransform();
	}
	ImGui::End();
#endif // IMGUI_USE
}
