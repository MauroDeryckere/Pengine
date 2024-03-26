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

void Scene::FixedUpdate()
{
}

void Scene::Update()
{
	Pengin::EventManager::GetInstance().ProcessEventQueue();

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

	auto aniComps = Pengin::ECS::GetInstance().GetComponents<Pengin::AnimationComponent>();
	for (auto& entity : aniComps)
	{
		entity.Update();
	}
}

void Scene::Render() const
{
	auto staticTexturecomps = Pengin::ECS::GetInstance().GetComponents<Pengin::StaticTextureComponent>();
	for (const auto& entity : staticTexturecomps)
	{
		entity.Render();
	}

	auto aniComps = Pengin::ECS::GetInstance().GetComponents<Pengin::AnimationComponent>();
	for (auto& entity : aniComps)
	{
		entity.Render();
	}
}

void Scene::RenderGUI() const
{

}

