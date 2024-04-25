#ifndef SYSTEMMANAGER
#define SYSTEMMANAGER

#include "SystemRegistry.h"
#include "BaseSystem.h"
#include "DebugOutput.h"

#include <typeindex>
#include <type_traits>

namespace Pengin
{
	class SystemManager final
	{
	public:
		SystemManager() = default;
		~SystemManager() = default;
			
		template<typename DerivedSystem> requires std::is_base_of_v<BaseSystem, DerivedSystem>
		std::shared_ptr<BaseSystem> RegisterSystem(std::shared_ptr<BaseSystem> pSystem, const std::vector<std::shared_ptr<BaseSystem>>& pDependencies = {}) noexcept
		{
			auto it = m_Type_RegistryIdxMap.find(typeid(DerivedSystem));
			if (it == end(m_Type_RegistryIdxMap))
			{
				const size_t idx = m_SysReg.Register(pSystem, pDependencies);
				m_Type_RegistryIdxMap[typeid(DerivedSystem)] = idx;

				return pSystem;
			}

			DEBUG_OUT(typeid(DerivedSystem).name() << " already registered in the systemmanager, returning the current ptr in the system");
			return m_SysReg.GetSystem(it->second);
		}

		template<typename DerivedSystem> requires std::is_base_of_v<BaseSystem, DerivedSystem>
		[[nodiscard]] std::shared_ptr<BaseSystem> GetSystem() const noexcept
		{
			auto it = m_Type_RegistryIdxMap.find(typeid(DerivedSystem));
			assert(it != end(m_Type_RegistryIdxMap) && "Returning nullptr, trying to get sys that doesnt exist");
			return m_SysReg.GetSystem(it->second);
		}

		template<typename DerivedSystem> requires std::is_base_of_v<BaseSystem, DerivedSystem>
		[[nodiscard]] bool IsRegistered() const noexcept
		{
			auto it = m_Type_RegistryIdxMap.find(typeid(DerivedSystem));
			return (it != end(m_Type_RegistryIdxMap));
		}

		void Update();
		void FixedUpdate();
		void Render() const;

		SystemManager(const SystemManager&) = delete;
		SystemManager(SystemManager&&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;
		SystemManager& operator=(const SystemManager&&) = delete;
		
	private:
		Impl::SystemRegistry m_SysReg{};

		std::unordered_map<std::type_index, size_t> m_Type_RegistryIdxMap;
	};
}

#endif