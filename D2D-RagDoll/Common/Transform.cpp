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
	// ���ε� �ڽ��� �θ� ���� ����
	if (pNewChild->pParent == this)
		return;

	// ���� �θ�� ������ ���´�.
	pNewChild->ReleaseParent();

	// �θ��� ������� ��ǥ������ �����ϱ� ���ؼ� 
	// �θ��� final ����� ��
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

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void Transform::SetWorldPosition(Vector2 position)
{
	// �θ� �����ϸ� �θ� �������� �����ؾߵ�
	if (this->pParent != NULL) {
		// �θ��� ������� �������� 
		Matrix matInvParentFinal;
		float temp;
		matInvParentFinal = pParent->matFinal.Inverse(temp);

		// �ڽ��� matFinal�� �ڽ� ���� * �θ��� ����
		// ���� �θ��� ����ķ� ���� �ؾߵ�
		// ��� �� �ٽ� �θ� ���� ���ϴ°�

		// �θ��� ����İ���ŭ �̵�
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
	// �̵� ����
	Vector2 move;

	// �ڽ��� �̵� ���� ��´�
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

// axis�� world ������ �������־�����
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
	// 2D �� �ѹ��⸸ ȸ���ϸ� �� z������
	Matrix matRotateZ;
	matRotateZ = matRotateZ.Rotate(angle);

	for (int i = 0; i < 2; i++)
		this->axis[i] = this->axis[i].TransformNormal(matRotateZ);

	if (this->bAutoUpdate)
		this->UpdateTransform();

}

void Transform::RotateLocal(float angle)
{
	// 2D �� �ѹ��⸸ ȸ���ϸ� �� z������
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

void Transform::SetRotateLocal(D3DXQUATERNION & worldRotate)
{
	D3DXQUATERNION quatRot = worldRotate;

	D3DXMATRIX matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	Matrix matRot = Matrix(matRotate);

	this->right = Vector2(1, 0);
	this->up = Vector2(0, 1);

	for (int i = 0; i < 2; i++)
		this->axis[i] = this->axis[i].TransformNormal(matRot);

	if (this->bAutoUpdate)
		this->UpdateTransform();
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
	// t ������ 
	// 0 <= t <= 1
	// 0�� ������ from�� ������ 1�� ������ to�� �����
	// 0�� ������ ���� ©���� �ǰ�
	// 1�� ������ ���� ©���� �ȴٴ°� 
	// t ©���� ����

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
	// �������� 

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
	// ����
	// scale�� ��������
	// position�� ��������
	// rotate ���麸��

	// auto ������Ʈ ���� ���� ����ְ� auto ������Ʈ �������
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
	float deltaMove = 1000.0f * deltaTime; // c++ ���� 10.f �̷��� �ᵵ ��
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

		if (INPUT->GetKey('F')) this->SetScaling(Vector2(0.1f, 0));
		if (INPUT->GetKey('G')) this->SetScaling(Vector2(-0.1f, 0));
		if (INPUT->GetKey('V')) this->SetScaling(Vector2(0, 0.1f));
		if (INPUT->GetKey('B')) this->SetScaling(Vector2(0, -0.1f));

		if (INPUT->GetKeyDown(VK_LEFT)) this->MovePositionSelf(Vector2(  -deltaMove / 5, 0));
		if (INPUT->GetKeyDown(VK_RIGHT)) this->MovePositionSelf(Vector2(  deltaMove / 5, 0));
		if (INPUT->GetKeyDown(VK_UP)) this->MovePositionSelf(Vector2(0,  -deltaMove / 5));
		if (INPUT->GetKeyDown(VK_DOWN)) this->MovePositionSelf(Vector2(0, deltaMove / 5));

	}

}

void Transform::UpdateTransform()
{
	// �ڽ��� ������ matLocal ����� ����
	this->matLocal = Matrix::Identity(4);
	this->matFinal = Matrix::Identity(4);

	// scale ���� ���� ��
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

Vector2 Transform::GetLocalPosition()
{
	return this->position;
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

void Transform::GetLocalUnitAxis(Vector2 * pVecArr) const
{
	for (int i = 0; i < 2; i++) {
		pVecArr[i] = axis[i];
		pVecArr[i] = pVecArr[i].Normalize();
	}
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
	// �θ�� ���� ����ȭ �ʿ�

	matRotate[0][0] = axis[0].x;
	matRotate[0][1] = axis[0].y;

	matRotate[1][0] = axis[1].x;
	matRotate[1][1] = axis[1].y;

	// 3�����̸� z������ ó�� �ʿ�

	return matRotate;
}

Matrix Transform::GetLocalRotateMatrix()
{
	Matrix matRotate;
	matRotate = Matrix::Identity(4);
	Vector2 axis[2];
	this->GetLocalUnitAxis(axis);
	// �θ�� ���� ����ȭ �ʿ�

	matRotate[0][0] = axis[0].x;
	matRotate[0][1] = axis[0].y;

	matRotate[1][0] = axis[1].x;
	matRotate[1][1] = axis[1].y;

	// 3�����̸� z������ ó�� �ʿ�

	return matRotate;
}

D3DXQUATERNION Transform::GetWorldRotateQuaternion()
{
	D3DXQUATERNION quat;

	// ���� ������ �޾ƿ��� �༮���� �ؾߵ�
	// GetUnitAxis()�� ���������� �޾ƿͼ� ��ķ� ��ȯ ���Ѽ�
	// �׳༮�� matWorld��� �༮�� ����־����

	D3DXMATRIX matWorld = this->GetWorldRotateMatrix().ToDXMatrix();

	// quat ������ ��ȯ�� (����� ȸ������ ��ȯ��)
	D3DXQuaternionRotationMatrix(&quat, &matWorld);

	return quat;
}

D3DXQUATERNION Transform::GetLocalRotateQuaternion()
{
	D3DXQUATERNION quat;

	// ���� ������ �޾ƿ��� �༮���� �ؾߵ�
	// GetUnitAxis()�� ���������� �޾ƿͼ� ��ķ� ��ȯ ���Ѽ�
	// �׳༮�� matWorld��� �༮�� ����־����

	D3DXMATRIX matWorld = this->GetLocalRotateMatrix().ToDXMatrix();

	// quat ������ ��ȯ�� (����� ȸ������ ��ȯ��)
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