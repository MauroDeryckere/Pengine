#ifndef BASESYSTEM
#define BASESYSTEM

namespace Pengin
{
	class BaseSystem
	{
	public:
		virtual ~BaseSystem() = default;

		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void Render() const {}

		BaseSystem(const BaseSystem&) = delete;
		BaseSystem(BaseSystem&&) = delete;
		BaseSystem& operator=(const BaseSystem&) = delete;
		BaseSystem& operator=(const BaseSystem&&) = delete;

	protected:
		BaseSystem() = default;
	};
}

#endif
