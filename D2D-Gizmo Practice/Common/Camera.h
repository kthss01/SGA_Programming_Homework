#pragma once

#include "Transform.h"

class Camera : public Transform
{
private:
	Matrix matView;
	Matrix matProjection;
	Matrix matViewProjection;

public:
	Camera();
	~Camera();

	void UpdateMatrix();
	// Device에 view랑 matrix 세팅, 카메라 메인화면에 띄우겠다.
	void UpdateCamToDevice();

	Matrix GetViewMatrix() { return matView; }
	Matrix GetProjection() { return matProjection; }
	Matrix GetViewProjection() { return matViewProjection; }
};

