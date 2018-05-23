#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::UpdateMatrix()
{
	Vector2 LookAt = position + Vector2(0, 0, 1);
	matView = Matrix::View(position, LookAt, up);

	matProjection = Matrix::Ortho(0, WINSIZE_X, WINSIZE_Y, 0, 0, 1.0f);
	matViewProjection = matView * matProjection;
}

void Camera::UpdateCamToDevice()
{
	this->UpdateMatrix();

	D2D::GetDevice()->SetTransform(
		D3DTS_VIEW, &this->matView.ToDXMatrix());
	D2D::GetDevice()->SetTransform(
		D3DTS_PROJECTION, &this->matProjection.ToDXMatrix());
}
