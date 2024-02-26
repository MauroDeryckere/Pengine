#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <string>
#include <cassert>

dae::FPSComponent::FPSComponent(GameObject* pGameObj):
	FunctionalComponent(pGameObj)
{
    m_pTextComp = pGameObj->GetComponent<dae::TextComponent>();
    assert(m_pTextComp);
}

void dae::FPSComponent::Update(float deltaTime)
{
    m_AccumulatedTime += deltaTime;
    ++m_FrameCount;

    if (m_AccumulatedTime >= 1.0f)
    {
        m_FPS = static_cast<float>(m_FrameCount) / m_AccumulatedTime;

        m_FrameCount = 0;
        m_AccumulatedTime -= 1.f;
        //std::format("{;.1f}", m_count/m_delay)
        m_pTextComp->SetText(std::to_string(m_FPS)); //dont need m_FPS
    }
}
