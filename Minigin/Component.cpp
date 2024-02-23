#include "Component.h"

#include "GameObject.h"

dae::Component::Component(GameObject* pGameObj) :
	m_pGameObject{ pGameObj }
{
}
