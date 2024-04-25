#ifndef SYSTEMREGISTRY
#define SYSTEMREGISTRY

#include "BaseSystem.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cassert>

namespace Pengin 
{
	namespace Impl {
		class SystemRegistry final
		{
		public:
			SystemRegistry() = default;
			~SystemRegistry() = default;

			const size_t Register(std::shared_ptr<BaseSystem> sys, const std::vector<std::shared_ptr<BaseSystem>>& dependencies) noexcept
			{
				m_pSystems.emplace_back(sys);
				m_pDependencies.emplace_back(dependencies);

				m_Sys_SysVecIdxMap[sys.get()] = (m_pSystems.size() - 1);

				return (m_pSystems.size() - 1);
			}

			[[nodiscard]] std::shared_ptr<BaseSystem> GetSystem(const size_t idx) const noexcept
			{
				assert(idx < m_pSystems.size() && "array idx out of bounds");

				return m_pSystems[idx];
			}

			void Update()
			{
				std::unordered_set<BaseSystem*> updatedSystems; //ensure nothing is updated twice (when something is stored as a dependency by multiple systems)

				for (size_t systemIndex{ 0 }; systemIndex < m_pSystems.size(); ++systemIndex)
				{
					UpdateSystem(systemIndex, updatedSystems);
				}
			}

			void FixedUpdate()
			{
				std::unordered_set<BaseSystem*> updatedSystems;

				for (size_t systemIndex{ 0 }; systemIndex < m_pSystems.size(); ++systemIndex)
				{
					FixedUpdateSystem(systemIndex, updatedSystems);
				}
			}

			void Render() const
			{
				std::unordered_set<BaseSystem*> renderedSystems;

				for (size_t systemIndex{ 0 }; systemIndex < m_pSystems.size(); ++systemIndex)
				{
					RenderSystem(systemIndex, renderedSystems);
				}
			}

			SystemRegistry(const SystemRegistry&) = delete;
			SystemRegistry(SystemRegistry&&) = delete;
			SystemRegistry& operator=(const SystemRegistry&) = delete;
			SystemRegistry& operator=(const SystemRegistry&&) = delete;
		private:
			std::vector<std::shared_ptr<BaseSystem>> m_pSystems{};

			std::vector<std::vector<std::shared_ptr<BaseSystem>>> m_pDependencies{};
			std::unordered_map<BaseSystem*, size_t> m_Sys_SysVecIdxMap{};

			void UpdateSystem(const size_t idx, std::unordered_set<BaseSystem*>& updatedSystems)
			{
				if (updatedSystems.find(m_pSystems[idx].get()) != end(updatedSystems))
				{
					return; //already updated
				}

				for (auto& depSys : m_pDependencies[idx])
				{
					const size_t vecIdx{ m_Sys_SysVecIdxMap.at(depSys.get()) };
					UpdateSystem(vecIdx, updatedSystems);
				}

				m_pSystems[idx]->Update();
				updatedSystems.insert(m_pSystems[idx].get());
			}

			void FixedUpdateSystem(const size_t idx, std::unordered_set<BaseSystem*>& updatedSystems)
			{
				if (updatedSystems.find(m_pSystems[idx].get()) != end(updatedSystems))
				{
					return;
				}

				for (auto& depSys : m_pDependencies[idx])
				{
					const size_t vecIdx{ m_Sys_SysVecIdxMap.at(depSys.get()) };
					FixedUpdateSystem(vecIdx, updatedSystems);
				}

				m_pSystems[idx]->FixedUpdate();
				updatedSystems.insert(m_pSystems[idx].get());
			}

			void RenderSystem(const size_t idx, std::unordered_set<BaseSystem*>& renderedSystems) const
			{
				if (renderedSystems.find(m_pSystems[idx].get()) != end(renderedSystems))
				{
					return;
				}

				for (auto& depSys : m_pDependencies[idx])
				{
					const size_t vecIdx{ m_Sys_SysVecIdxMap.at(depSys.get()) };
					RenderSystem(vecIdx, renderedSystems);
				}

				m_pSystems[idx]->Render();
				renderedSystems.insert(m_pSystems[idx].get());
			}
		};
	}
}

#endif
