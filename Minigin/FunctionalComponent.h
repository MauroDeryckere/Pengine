#ifndef FUNCTIONALCOMPONENT
#define FUNCTIONALCOMPONENT

#include "Component.h"

namespace dae
{
	class GameObject;

	class FunctionalComponent :
		public Component
	{
	public:
		FunctionalComponent(GameObject* pGameObj);
		virtual ~FunctionalComponent() override = default;

		virtual void Update(float deltaT) = 0;

		FunctionalComponent(const FunctionalComponent& other) = delete;
		FunctionalComponent(FunctionalComponent&& other) = delete;
		FunctionalComponent& operator=(const FunctionalComponent& other) = delete;
		FunctionalComponent& operator=(FunctionalComponent&& other) = delete;

	private:

	};
}
#endif

