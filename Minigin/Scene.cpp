#include "Scene.h"

#include "ECS.h"
#include "Components.h"

#include "Renderer.h"

#include "EventManager.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) 
{
}

Scene::~Scene() = default;

void dae::Scene::FixedUpdate()
{
}

void Scene::Update()
{
	auto fpsComps = Pengin::ECS::GetInstance().GetComponents<Pengin::FPSCounterComponent>();

	for (auto& entity : fpsComps)
	{
		entity.Update();
	}

	auto textComps = Pengin::ECS::GetInstance().GetComponents<Pengin::TextComponent>();

	for (auto& entity : textComps)
	{
		entity.Update();
	}

	Pengin::EventManager::GetInstance().ProcessEvents();
}

void Scene::Render() const
{
	auto textureComps = Pengin::ECS::GetInstance().GetComponents<Pengin::TextureComponent>();
	
	for (const auto& entity : textureComps)
	{
		entity.Render();
	}
}

void Scene::RenderGUI() const
{
	//m_TrashCache.Render();
}

