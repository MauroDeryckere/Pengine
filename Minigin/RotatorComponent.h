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
		RotatorComponent(GameObject* pGameObj, float rotationSpeed, bool rotateAroundParent = true, const glm::vec3& rotPoint={});
		~RotatorComponent() override = default;

		virtual void Update() override;

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

	private:
		Transform& m_ObjTransform;

		float m_RotSpeed;

		bool m_RotateAroundParent;
		glm::vec3 m_RotPoint;
	};
}

#endif