#include "EcsEventInter.h"

#include "EventManager.h"

void Pengin::EcsEventInter::SetObserverDirty(EntityId entityId, std::type_index typeIdx)
{
	EventManager::GetInstance().SetObserverDirty(entityId, typeIdx);
}
