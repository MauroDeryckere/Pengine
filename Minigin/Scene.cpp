#include "Scene.h"
#include "GameObject.h"
#include "ECS.h"
#include "Components.h"
#include "Renderer.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void dae::Scene::FixedUpdate(float fixedTimeStep)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixedTimeStep);
	}
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}
	

	//Dirty flag for removal loop
}

void Scene::Render()
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}	
	
	auto& posComp = ecs.GetComponent<Pengin::PositionComponent>(1);
	auto& texturecomp = ecs.GetComponent<Pengin::TextureComponent>(1);
	if (texturecomp.m_pTexture)
	{
		Renderer::GetInstance().RenderTexture(*texturecomp.m_pTexture, static_cast<float>(posComp.x), static_cast<float>(posComp.y));
	}
	
}

