#include "Scene.h"
#include "GameObject.h"
#include "ECS.h"
#include "Components.h"
#include "Renderer.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) 
{
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	object->SetDeleteFlag();
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
	
	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](const auto& obj) {
		return obj->GetDeleteFlag();
		}), m_objects.end());
}

void Scene::Render() const
{
	auto textureComps = Pengin::ECS::GetInstance().GetComponents<Pengin::TextureComponent>();
	
	for (const auto& entity : textureComps)
	{
		entity.Render();
	}
	

	//for (const auto& object : m_objects)
	//{
		//object->Render();
	//}
}

void Scene::RenderGUI() const
{
	//m_TrashCache.Render();
}

