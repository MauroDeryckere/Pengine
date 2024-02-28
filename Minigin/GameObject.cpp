#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"

namespace dae
{
	GameObject::~GameObject() = default;

	void GameObject::AddChild(GameObject* pChild)
	{
		pChild;
	}

	void GameObject::RemoveChild(GameObject* pChild)
	{
		pChild;
	}

	void GameObject::Update(float deltaTime)
	{
		for (const auto& pFuncComp : m_pFunctionalComponents)
		{
			pFuncComp->Update(deltaTime);
		}

		for (const auto& pRenderComp : m_pRenderComponents)
		{
			pRenderComp->Update(deltaTime);
		}
	}

	void GameObject::FixedUpdate(float timeStep)
	{
		for (auto& pPhysicsComp : m_pPhysicsComponents)
		{
			pPhysicsComp->FixedUpdate(timeStep);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& pRenderComp : m_pRenderComponents)
		{
			pRenderComp->Render();
		}
	}

	GameObject* GameObject::GetParent() const
	{
		return m_pParent;
	}

	void GameObject::SetParent(GameObject* pParent)
	{
		pParent;
	}

	size_t GameObject::GetChildCount()
	{
		return m_pChildren.size();
	}

	GameObject* GameObject::GetChild(size_t idx)
	{
		assert(idx < GetChildCount());

		return m_pChildren[idx];
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}

}