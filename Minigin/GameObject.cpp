#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	deltaTime;
}

void dae::GameObject::FixedUpdate(float timeStep)
{
	for (auto& pPhysicsComp : m_pPhysicsComponents)
	{
		pPhysicsComp->FixedUpdate(timeStep);
	}
}

void dae::GameObject::Render() const
{
	for (const auto& pRenderComp : m_pRenderComponents)
	{
		pRenderComp->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
