#ifndef BASECOMPONENTSTORAGE
#define BASECOMPONENTSTORAGE

#include "Entity.h"

namespace Pengin
{
    class BaseComponentStorage
    {
    public:
        virtual void RemoveComponent(const EntityId& id) noexcept = 0;
        virtual bool HasComponent(const EntityId& id) noexcept = 0;
        //AddComponent
        //...

        virtual ~BaseComponentStorage() = default;

        BaseComponentStorage(const BaseComponentStorage&) = delete;
        BaseComponentStorage& operator=(const BaseComponentStorage&) = delete;
        BaseComponentStorage(BaseComponentStorage&&) noexcept = delete;
        BaseComponentStorage& operator=(BaseComponentStorage&&) noexcept = delete;

    protected:
        BaseComponentStorage() = default;

    private:


    };
}

#endif