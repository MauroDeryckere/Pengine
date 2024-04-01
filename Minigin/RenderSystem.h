#ifndef RENDERSYSTEM
#define RENDERSYSTEM

namespace Pengin
{
	class ECS;

	class RenderSystem final
	{
	public:
		RenderSystem(const ECS& ecs) :
			m_ECS{ ecs }
		{}

		~RenderSystem() = default;

		void Render() const;

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&&) = delete;

	private:
		const ECS& m_ECS;
	};
}

#endif
