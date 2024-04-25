#include "DebugDrawSystem.h"

#include "Renderer.h"

#include "DebugDrawComponent.h"
#include "TransformComponent.h"
#include "ECS.h"

namespace Pengin
{
	void DebugDrawSystem::Render() const
	{
		auto debugDrawComps = m_ECS.GetComponents<DebugDrawComponent>();
		for (auto it{std::begin(debugDrawComps)}; const auto & comp : debugDrawComps)
		{
			const auto entityId = debugDrawComps.GetIdFromIterator(it);

			const auto& transform = m_ECS.GetComponent<TransformComponent>(entityId);

			const UtilStructs::Rect16 dest{ static_cast<int16_t>(transform.worldPos.x), 
											static_cast<int16_t>(transform.worldPos.y),  
											static_cast<int16_t>(comp.width * transform.scale.x),  
											static_cast<int16_t>(comp.height * transform.scale.y) };

			if (comp.fill)
			{
				dae::Renderer::GetInstance().FillRect(dest, { comp.color.x, comp.color.y, comp.color.z, comp.color.w });
			}
			else
			{
				dae::Renderer::GetInstance().DrawRect(dest, { comp.color.x, comp.color.y, comp.color.z, comp.color.w });
			}

			++it;
		}
	}
}