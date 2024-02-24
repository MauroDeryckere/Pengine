#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

#include "Transform.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "FunctionalComponent.h"

#include <iostream>

namespace dae
{
	template<typename T>
	concept ComponentConcept = std::is_base_of_v<Component, T>;

	class Texture2D;

	class GameObject final
	{

	public:
		virtual void Update(float deltaTime);
		void FixedUpdate(float timeStep);
		virtual void Render() const;

		void SetPosition(float x, float y);
		const Transform& GetTransform() { return m_transform; };

		//Components
		template<typename T>
		bool AddComponent(T&& component) requires ComponentConcept<std::decay_t<decltype(*component)>>;

		template<typename ComponentT>
		bool RemoveComponent() requires ComponentConcept<ComponentT>;

		template<typename ComponentT>
		ComponentT* GetComponent() const requires ComponentConcept<ComponentT>;

		template<typename ComponentT>
		bool HasComponentBeenAdded() const requires ComponentConcept<ComponentT>;
		bool HasComponentBeenAdded(const std::type_index& typeIdx) const
		{
			return (m_AddedComponentsRegistry.find(typeIdx) != end(m_AddedComponentsRegistry));
		};
		//----------

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
		std::vector<std::unique_ptr<FunctionalComponent>> m_pFunctionalComponents;
		
		Transform m_transform{};

		template<typename ComponentT, typename ContainerT>
		bool RemoveComponent(ContainerT& container) requires ComponentConcept<ComponentT>;

		template<typename ComponentT, typename ContainerT>
		ComponentT* GetComponent(const ContainerT& container) const requires ComponentConcept<ComponentT>;

		template<typename ComponentT, typename ContainerT>
		bool HasComponentBeenAdded(const ContainerT& container) const requires ComponentConcept<ComponentT>;
	};

    template<typename ComponentT, typename ContainerT>
    bool GameObject::RemoveComponent(ContainerT& container) requires ComponentConcept<ComponentT>
    {
        auto it{ std::remove_if(std::begin(container), std::end(container),
        [](const auto& ptr)
        {
            return dynamic_cast<ComponentT*>(ptr.get()) != nullptr;
        }) };

        if (it != std::end(container))
        {
            container.erase(it);
            return true;
        }
        return false;
    }

    template<typename ComponentT, typename ContainerT>
    ComponentT* GameObject::GetComponent(const ContainerT& container) const requires ComponentConcept<ComponentT>
    {
        for (const auto& ptr : container)
        {
            if (auto* component = dynamic_cast<ComponentT*>(ptr.get()))
                return component;
        }
        return nullptr;
    }

    template<typename ComponentT, typename ContainerT>
    bool GameObject::HasComponentBeenAdded(const ContainerT& container) const requires ComponentConcept<ComponentT>
    {
        return (m_AddedComponentsRegistry.find(typeid(ComponentT)) != m_AddedComponentsRegistry.end());
    }

    template<typename T>
    bool GameObject::AddComponent(T&& component)
        requires ComponentConcept<std::decay_t<decltype(*component)>>
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
            else if constexpr (std::is_base_of_v<FunctionalComponent, ComponentType>)
            {
                m_pFunctionalComponents.emplace_back(std::forward<T>(component));
            }

            return true;
        }
        else
        {
            //could throw
            return false;
        }
    }

    template<typename ComponentT>
    bool GameObject::RemoveComponent() requires ComponentConcept<ComponentT>
    {
        if constexpr (std::is_base_of_v<RenderComponent, ComponentT>)
        {
            return RemoveComponent<ComponentT>(m_pRenderComponents);
        }
        else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentT>)
        {
            return RemoveComponent<ComponentT>(m_pPhysicsComponents);
        }
        else if constexpr (std::is_base_of_v<FunctionalComponent, ComponentT>)
        {
            return RemoveComponent<ComponentT>(m_pFunctionalComponents);
        }
        else
        {
            return false;
        }
    }

    template<typename ComponentT>
    ComponentT* GameObject::GetComponent() const requires ComponentConcept<ComponentT>
    {
        if constexpr (std::is_base_of_v<RenderComponent, ComponentT>)
        {
            return GetComponent<ComponentT>(m_pRenderComponents);
        }
        else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentT>)
        {
            return GetComponent<ComponentT>(m_pPhysicsComponents);
        }
        else if constexpr (std::is_base_of_v<FunctionalComponent, ComponentT>)
        {
            return GetComponent<ComponentT>(m_pFunctionalComponents);
        }
        else
        {
            return nullptr;
        }
    }

    template<typename ComponentT>
    bool GameObject::HasComponentBeenAdded() const requires ComponentConcept<ComponentT>
    {
        if constexpr (std::is_base_of_v<RenderComponent, ComponentT>)
        {
            return HasComponentBeenAdded<ComponentT>(m_pRenderComponents);
        }
        else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentT>)
        {
            return HasComponentBeenAdded<ComponentT>(m_pPhysicsComponents);
        }
        else if constexpr (std::is_base_of_v<FunctionalComponent, ComponentT>)
        {
            return HasComponentBeenAdded<ComponentT>(m_pFunctionalComponents);
        }
        else
        {
            return false;
        }
    }
}
