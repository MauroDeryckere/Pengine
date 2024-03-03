#ifndef ROTATORCOMPONENT
#define ROTATORCOMPONENT

#include "FunctionalComponent.h"
#include "Transform.h"

namespace dae
{
	class GameObject;

	class RotatorComponent final :
		public FunctionalComponent
	{
	public:
		RotatorComponent(GameObject* pGameObj, float rotationSpeed);
		~RotatorComponent() = default;

		virtual void Update(float deltaT) override;

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

	private:
		Transform& m_ObjTransform;

		float m_RotSpeed;
	};
}

#endif