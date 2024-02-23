#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <type_traits>
#include <typeindex>

#include "Transform.h"

#include "Component.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "TextureComponent.h"

#include <iostream>

namespace dae
{
	class Texture2D;

	template<typename T> //TODO
	concept IsRenderOrPhysicsComponent = std::is_base_of_v<RenderComponent, std::remove_pointer_t<T>> ||
										 std::is_base_of_v<PhysicsComponent, std::remove_pointer_t<T>> ||
										 std::is_same_v<PhysicsComponent, std::remove_pointer_t<T>>;

	// todo: this should become final.
	class GameObject
	{

	public:
		virtual void Update(float deltaTime);
		void FixedUpdate(float timeStep);
		virtual void Render() const;

		void SetPosition(float x, float y);
		const Transform& GetTransform() { return m_transform; };

		template<typename T>
		bool AddComponent(T&& component)
		//requires IsRenderOrPhysicsComponent<T>
		{
			using ComponentType = std::decay_t<decltype(*component)>;
			if (m_AddedComponentsRegistry.insert(typeid(ComponentType)).second)
			{
				if constexpr (std::is_base_of_v<RenderComponent, ComponentType>)
				{
					m_pRenderComponents.emplace_back(std::forward<T>(component));
				}
				else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentType>)
				{
					m_pPhysicsComponents.emplace_back(std::forward<T>(component));
				}

				return true;
			}
			else
			{
				//could throw
				return false;
			}
		};

		template<typename ComponentT>
		bool RemoveComponent()
		requires std::is_base_of_v<Component, ComponentT>
		{
			if constexpr (std::is_base_of_v<RenderComponent, ComponentT>)
			{
				auto it{ std::remove_if(begin(m_pRenderComponents), end(m_pRenderComponents),
				[](const auto& ptr) 
				{
						return dynamic_cast<ComponentT*>(ptr.get()) != nullptr;
				}) };

				if (it != m_pRenderComponents.end())
				{
					m_pRenderComponents.erase(it);
					return true;
				}
				return false;
			}
			else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentT>)
			{
				auto it{ std::remove_if(begin(m_pPhysicsComponents), end(m_pPhysicsComponents),
				[](const auto& ptr) 
				{
					return dynamic_cast<ComponentT*>(ptr.get()) != nullptr;
				}) };

				if (it != m_pPhysicsComponents.end())
				{
					m_pPhysicsComponents.erase(it);
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		};

		template<typename ComponentT>
		ComponentT* GetComponent() const
		requires std::is_base_of_v<Component, ComponentT>
		{
			if constexpr (std::is_base_of_v<RenderComponent, ComponentT>)
			{
				for (const auto& ptr : m_pRenderComponents)
				{
					if (auto* component = dynamic_cast<ComponentT*>(ptr.get()))
						return component;
				}
			}
			else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentT>)
			{
				for (const auto& ptr : m_pPhysicsComponents)
				{
					if (auto* component = dynamic_cast<ComponentT*>(ptr.get()))
						return component;
				}
			}

			return nullptr;
		}

		template<typename ComponentT>
		bool HasComponentBeenAdded() const
		requires std::is_base_of_v<Component, ComponentT>
		{
			return HasComponentBeenAdded(type_index(ComponentT));
		};

		bool HasComponentBeenAdded(const std::type_index& typeIdx) const
		{
			return (m_AddedComponentsRegistry.find(typeIdx) != end(m_AddedComponentsRegistry));
		};

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::unordered_set<std::type_index> m_AddedComponentsRegistry;

		std::vector<std::unique_ptr<PhysicsComponent>> m_pPhysicsComponents;
		std::vector<std::unique_ptr<RenderComponent>> m_pRenderComponents;
		
		Transform m_transform{};
	};
}
