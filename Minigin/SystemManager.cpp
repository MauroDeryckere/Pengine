#include "SystemManager.h"

namespace Pengin
{
	SystemManager::SystemManager():
		m_pSysReg { std::make_unique<Impl::SystemRegistry>() }
	{ }

	std::shared_ptr<BaseSystem> SystemManager::RegisterSystem(std::shared_ptr<BaseSystem> pSystem, std::vector<std::shared_ptr<BaseSystem>> pDependencies) noexcept
	{
		m_pSysReg->Register(pSystem, pDependencies);
		return pSystem;
	}

	void SystemManager::Update()
	{
		m_pSysReg->Update();
	}

	void SystemManager::FixedUpdate()
	{
		m_pSysReg->FixedUpdate();
	}

	void SystemManager::Render() const
	{
		m_pSysReg->Render();
	}
}


