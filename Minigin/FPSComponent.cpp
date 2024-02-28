#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <string>
#include <format>
#include <cassert>

dae::FPSComponent::FPSComponent(GameObject* pGameObj) :
    FunctionalComponent(pGameObj),
    m_AccumulatedTime{ 0.f },
    m_FrameCount{ 0 },
    m_pTextComp{ pGameObj->GetComponent<dae::TextComponent>() }
{
    assert(m_pTextComp);
}

void dae::FPSComponent::Update(float deltaTime)
{
    m_AccumulatedTime += deltaTime;
    ++m_FrameCount;

    if (m_AccumulatedTime >= 1.0f)
    {
        const auto txt{ std::format("{:.1f}", m_FrameCount / m_AccumulatedTime) };
        m_pTextComp->SetText(txt);

        m_FrameCount = 0;
        m_AccumulatedTime = 0.f;
    }
}
