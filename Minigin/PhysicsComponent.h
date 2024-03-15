#ifndef PHYSICSCOMPONENT
#define PHYSICSCOMPONENT

#include "Component.h"
namespace dae
{
    class PhysicsComponent
        : public Component
    {
    public:
        ~PhysicsComponent() = default;

        virtual void FixedUpdate() = 0;

        PhysicsComponent(const PhysicsComponent& other) = delete;
        PhysicsComponent(PhysicsComponent&& other) = delete;
        PhysicsComponent& operator=(const PhysicsComponent& other) = delete;
        PhysicsComponent& operator=(PhysicsComponent&& other) = delete;

    protected:
        PhysicsComponent(GameObject* pGameObj);

    private:

    };
}

#endif

