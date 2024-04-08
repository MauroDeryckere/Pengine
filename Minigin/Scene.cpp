#include "Scene.h"

#include "Serializer.h"

#include "Entity.h"
#include "TransformComponent.h"

namespace Pengin
{
	unsigned int Scene::m_IdCounter = 0;

	Scene::Scene(const std::string& name) : 
		m_Name{ name }
	{ }

	Entity Scene::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		const auto id{ m_Ecs.CreateEntity() };

		Entity entity{ id, weak_from_this() };
		entity.AddComponent<TransformComponent>(position, rotation, scale);

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity, bool keepChildren)
	{
		entity.SetParent({ NULL_ENTITY_ID, shared_from_this() }, keepChildren);
		return m_Ecs.DestroyEntity(entity.GetEntityId());
	}

	bool Scene::DestroyEntity(const EntityId entityId, bool keepChildren)
	{
		return DestroyEntity({entityId, shared_from_this() }, keepChildren);
	}

	void Scene::FixedUpdate()
	{
	}

	void Scene::Update()
	{
		//Order doesnt matter
		m_TextSystem->Update();
		m_FPSSystem->Update();
		m_AnimationSystem->Update(); 
		//-------------------
		
		//Detect collision (and respond) - might adjust velocity so update the positions | no complex phjysics so not required to be in fixed update rn
		m_CollSystem->Update(); 
		//Update the character positions based on velocity - might set dirty
		m_MovementSystem->Update();
		//Update the dirty world positions that are left
		m_WorldPosSystem->Update();
	}

	void Scene::Render() const
	{
		m_RenderSystem->Render();
	}

	void Scene::RenderImGUI()
	{
		m_SceneInfoPanel->Render(m_Ecs);
		m_InputInfoPanel->Render();
	}

	void Scene::SerializeEntity(const EntityId id) noexcept
	{
		Serializer::GetInstance().SerializeEntity(m_Ecs, id, "../Data/jsonTestSerEntity.json");
	}

	void Scene::DeserializeEntity() noexcept
	{
		Serializer::GetInstance().DeserializeEntity(m_Ecs, "../Data/jsonTestSerEntity.json");
	}

}