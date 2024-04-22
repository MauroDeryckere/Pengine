#ifndef SYSTEMMANAGER
#define SYSTEMMANAGER

#include "SystemRegistry.h"
#include "BaseSystem.h"

namespace Pengin
{
	class SystemManager final
	{
	public:
		SystemManager() = default;
		~SystemManager() = default;

		std::shared_ptr<BaseSystem> RegisterSystem(std::shared_ptr<BaseSystem> pSystem, std::vector<std::shared_ptr<BaseSystem>> pDependencies = {}) noexcept;
		//RemoveSystem();

		void Update();
		void FixedUpdate();
		void Render() const;

		SystemManager(const SystemManager&) = delete;
		SystemManager(SystemManager&&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;
		SystemManager& operator=(const SystemManager&&) = delete;
		
	private:
		Impl::SystemRegistry m_SysReg{};
	};
}

#endif