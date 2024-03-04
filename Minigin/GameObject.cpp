#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"

namespace dae
{
	GameObject::~GameObject()
	{
	};

	void GameObject::AddChild(GameObject* pChild)
	{
		m_pChildren.emplace_back(pChild);
	}

	void GameObject::RemoveChild(GameObject* pChild)
	{
		auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
		if (it != m_pChildren.end())
		{
			m_pChildren.erase(it);
		}
	}

	bool GameObject::IsChild(GameObject* pGameobj) const
	{
		return std::find(m_pChildren.begin(), m_pChildren.end(), pGameobj) != end(m_pChildren);
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

	void GameObject::SetParent(GameObject* pParent, bool keepWorldPos)
	{
		if (IsChild(pParent) || pParent == this || m_pParent == pParent)
		{
			return;
		}

		if (pParent == nullptr)
		{
			SetLocalPosition(GetWorldPosition());
		}
		else
		{
			if (keepWorldPos)
			{
				SetLocalPosition(GetWorldPosition() - pParent->GetWorldPosition());
				SetPosDirty();
			}
		}
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}

		m_pParent = pParent;
		
		if (m_pParent)
		{
			m_pParent->AddChild(this);
		}
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

	void GameObject::SetWorldPosition(float x, float y)
	{
		if (!m_pParent)
		{
			m_transform.SetPosition(x, y, 0);
		}
		
		SetPosDirty();
	}

	void GameObject::SetLocalPosition(const glm::vec3& pos)
	{
		m_transform.SetPosition(pos.x, pos.y, pos.z);

		SetPosDirty();
	}
	const glm::vec3& GameObject::GetWorldPosition()
	{
		if (m_PositionIsDirty)
		{
			UpdateWorldPosition();
		}
		return m_WorldPosition;

	}
	void GameObject::UpdateWorldPosition()
	{
		if (m_PositionIsDirty)
		{
			if (m_pParent == nullptr)
			{
				m_WorldPosition = m_transform.GetPosition();
			}
			else
			{
				m_WorldPosition = m_pParent->GetWorldPosition() + m_transform.GetPosition();
			}
		}
		m_PositionIsDirty = false;
	}
}