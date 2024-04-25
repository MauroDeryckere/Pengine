#include "SystemManager.h"

namespace Pengin
{
	std::shared_ptr<BaseSystem> SystemManager::RegisterSystem(std::shared_ptr<BaseSystem> pSystem, const std::vector<std::shared_ptr<BaseSystem>>& pDependencies) noexcept
	{
		m_SysReg.Register(pSystem, pDependencies);
		return pSystem;
	}

	void SystemManager::Update()
	{
		m_SysReg.Update();
	}

	void SystemManager::FixedUpdate()
	{
		m_SysReg.FixedUpdate();
	}

	void SystemManager::Render() const
	{
		m_SysReg.Render();
	}
}


