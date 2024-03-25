#include "EcsEventInter.h"

#include "EventManager.h"

void Pengin::EcsEventInter::SetObserverDirty(EntityId entityId, std::type_index typeIdx) const noexcept
{
	EventManager::GetInstance().SetObserverDirty(entityId, typeIdx);
}
