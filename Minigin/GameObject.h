#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <stdexcept>

#include "Transform.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "FunctionalComponent.h"

#include <iostream>

//Gameobj raw ptr, vectors own it (oor sparse s) ?

namespace dae
{
	template<typename T>
	concept ComponentConcept = std::is_base_of_v<Component, T>;

	class Texture2D;

	class GameObject final
	{

	public:
		void Update();
		void FixedUpdate();
		void Render() const;

        //Scene graph
        GameObject* GetParent() const;
        void SetParent(GameObject* pParent, bool keepWorldPos = false);
        size_t GetChildCount();
        GameObject* GetChild(size_t idx);
        //-----------

        void SetWorldPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& pos);

        const glm::vec3& GetWorldPosition();

        void SetPosDirty() 
        { 
            m_PositionIsDirty = true;

            for (auto& child : m_pChildren)
            {
                child->SetPosDirty();
            }
        }
        
        const Transform& GetTransform() const { return m_transform; }
        Transform& GetTransform() { return m_transform; }

        void SetDeleteFlag() 
        {
            m_DeleteFlag = true;
        
            for (auto& child : m_pChildren)
            {
                child->SetDeleteFlag();
            }
        }

        [[nodiscard]] bool GetDeleteFlag() const { return m_DeleteFlag; }

		//Components
		template<typename T>
		void AddComponent(T&& component) requires ComponentConcept<std::decay_t<decltype(*component)>>;

		template<typename ComponentT>
		void RemoveComponent() requires ComponentConcept<ComponentT>;

		template<typename ComponentT>
		[[nodiscard]] ComponentT* GetComponent() const requires ComponentConcept<ComponentT>;

		template<typename ComponentT>
		bool HasComponentBeenAdded() const requires ComponentConcept<ComponentT>;
		bool HasComponentBeenAdded(const std::type_index& typeIdx) const
		{
			return (m_AddedComponentsRegistry.find(typeIdx) != end(m_AddedComponentsRegistry));
		};
		//----------

		GameObject() = default;
		~GameObject() = default;

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
        bool m_DeleteFlag = false;
        
        GameObject* m_pParent;
        std::vector<GameObject*> m_pChildren;

		std::unordered_set<std::type_index> m_AddedComponentsRegistry;

		std::vector<std::unique_ptr<PhysicsComponent>> m_pPhysicsComponents;
		std::vector<std::unique_ptr<RenderComponent>> m_pRenderComponents;
		std::vector<std::unique_ptr<FunctionalComponent>> m_pFunctionalComponents;
		
        bool m_PositionIsDirty = false;

        glm::vec3 m_WorldPosition{};

		Transform m_transform{this};

        void UpdateWorldPosition();

        void AddChild(GameObject* pChild);
        void RemoveChild(GameObject* pChild);

        bool IsChild(GameObject* pGameobj) const;

		template<typename ComponentT, typename ContainerT>
		void RemoveComponent(ContainerT& container) requires ComponentConcept<ComponentT>;

		template<typename ComponentT, typename ContainerT>
		ComponentT* GetComponent(const ContainerT& container) const requires ComponentConcept<ComponentT>;

		template<typename ComponentT, typename ContainerT>
		bool HasComponentBeenAdded(const ContainerT& container) const requires ComponentConcept<ComponentT>;
	};

#pragma region Template component functions
    template<typename ComponentT, typename ContainerT>
    void GameObject::RemoveComponent(ContainerT& container) requires ComponentConcept<ComponentT>
    {
        auto it{ std::remove_if(std::begin(container), std::end(container),
        [](const auto& ptr)
        {
            return dynamic_cast<ComponentT*>(ptr.get()) != nullptr;
        }) };

        if (it != std::end(container))
        {
            container.erase(it);
        }
        throw std::runtime_error("Component not found in container");
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
    void GameObject::AddComponent(T&& component)
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
            else
            {
                throw std::runtime_error("No valid comp Type");
            }
        }
        else
        {
            throw std::runtime_error("Component already added");
        }
    }

    template<typename ComponentT>
    void GameObject::RemoveComponent() requires ComponentConcept<ComponentT>
    {
        if constexpr (std::is_base_of_v<RenderComponent, ComponentT>)
        {
            RemoveComponent<ComponentT>(m_pRenderComponents);
        }
        else if constexpr (std::is_base_of_v<PhysicsComponent, ComponentT>)
        {
            RemoveComponent<ComponentT>(m_pPhysicsComponents);
        }
        else if constexpr (std::is_base_of_v<FunctionalComponent, ComponentT>)
        {
            RemoveComponent<ComponentT>(m_pFunctionalComponents);
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
#pragma endregion
}
