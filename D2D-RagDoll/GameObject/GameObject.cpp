#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

void GameObject::Release()
{
}

void GameObject::Update()
{
}

void GameObject::Render()
{
}

void GameObject::SetTransform(Transform* transform)
{
	this->transform->SetScale(transform->GetScale());
	this->transform->SetWorldPosition(transform->GetWorldPosition());
	D3DXQUATERNION quat = transform->GetWorldRotateQuaternion();
	this->transform->SetRotateWorld(quat);
}
