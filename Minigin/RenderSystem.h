#ifndef RENDERSYSTEM
#define RENDERSYSTEM

#include "BaseSystem.h"

namespace Pengin
{
	class ECS;

	class RenderSystem final : public BaseSystem
	{
	public:
		RenderSystem(const ECS& ecs) :
			BaseSystem{ },
			m_ECS{ ecs }
		{}

		virtual ~RenderSystem() override = default;

		virtual void Render() const override;

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&&) = delete;

	private:
		const ECS& m_ECS;
	};
}

#endif
