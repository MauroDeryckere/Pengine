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
		void AddComponent(T&& component)
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
			}
			else
			{
				std::cout << "trying to add twice \n";
			}
		};

		void RemoveComponent();
		void GetComponent() const;
		bool HasComponentBeenAdded() const;

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
