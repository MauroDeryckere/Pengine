#ifndef RENDERCOMPONENT
#define RENDERCOMPONENT

#include "Component.h"

namespace dae
{
	class RenderComponent
		: public Component
	{
	public:
		virtual ~RenderComponent() override = default;

		virtual void Update(float) = 0;
		virtual void Render() const = 0;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

	protected:
		RenderComponent(GameObject* pGameObj);
	private:

	};
}

#endif



