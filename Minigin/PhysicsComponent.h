#ifndef PHYSICSCOMPONENT
#define PHYSICSCOMPONENT

#include "Component.h"
namespace dae
{
    class PhysicsComponent
        : public Component
    {
    public:
        PhysicsComponent(GameObject* pGameObj);
        ~PhysicsComponent() = default;

        virtual void FixedUpdate(float fixedTimeStep) = 0;

        PhysicsComponent(const PhysicsComponent& other) = delete;
        PhysicsComponent(PhysicsComponent&& other) = delete;
        PhysicsComponent& operator=(const PhysicsComponent& other) = delete;
        PhysicsComponent& operator=(PhysicsComponent&& other) = delete;

    private:

    };
}

#endif

