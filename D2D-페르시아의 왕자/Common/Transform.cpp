#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
	:pParent(NULL), pFirstChild(NULL), pNextSibling(NULL)
{
	bAutoUpdate = true;
	this->Reset();
}

Transform::~Transform()
{
}

void Transform::AddChild(Transform * pChild)
{
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

void Transform::RotateWorld(Vector2 angle)
{
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

void Transform::RotateLocal(Vector2 angle)
{
}

void Transform::SetRotateWorld(const Matrix matWorldRotate)
{
}

void Transform::SetRotateLocal(const Matrix matLocalRotate)
{
}

void Transform::LookPosition(Vector2 pos, Vector2 up)
{
}

void Transform::DefaultControl2()
{
	float deltaTime = (float)Frame::Get()->GetFrameDeltaSec();
	float deltaMove = 1000.0f * deltaTime; // c++ 에선 10.f 이렇게 써도 됨
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
