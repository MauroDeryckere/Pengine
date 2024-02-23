#include "RenderComponent.h"
#include <iostream>

dae::RenderComponent::RenderComponent(GameObject* pGameObj):
	Component(pGameObj)
{
	std::cout << "render comp\n";
}
