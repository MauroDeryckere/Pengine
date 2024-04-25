#include "SystemManager.h"

namespace Pengin
{
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


