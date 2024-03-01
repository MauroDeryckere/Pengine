#ifndef COMPONENTSTORAGE
#define COMPONENTSTORAGE

#include "SparseSet.h"
#include "Components.h"

#include <memory>
#include <unordered_map>
#include <typeindex>

namespace Pengin
{
    class ComponentStorage final
    {
    public:
        ComponentStorage()
        {
            //using SetPtr = std::unique_ptr<void, void(*)(void*)>;


            //auto deleter = [](void* ptr) { delete static_cast<SparseSet<TestComponent, unsigned>*>(ptr); };
            //auto set = std::unique_ptr<void, decltype(deleter)>(new SparseSet<TestComponent, unsigned>(), deleter);
            //m_componentStorage[typeid(TestComponent)] = std::move(set);
        
        };
        ~ComponentStorage() = default;

        ComponentStorage(const ComponentStorage&) = delete;
        ComponentStorage& operator=(const ComponentStorage&) = delete;
        ComponentStorage(ComponentStorage&&) noexcept = delete;
        ComponentStorage& operator=(ComponentStorage&&) noexcept = delete;

        //template<typename ComponentType, typename Key>
        //requires std::is_default_constructible_v<Key> && std::is_constructible_v<ComponentType>
        //SparseSet<ComponentType, Key>* getComponentSet()  const //Private in future, just testing 
        //{
        //    const auto it{ m_componentStorage.find(typeid(ComponentType)) };
        //    if (it != m_componentStorage.end()) 
        //    {
        //        const auto handler{ static_cast<TypedComponentSetHandler<ComponentType, Key>*>(it->second.get()) };
        //        return &(handler->m_ComponentSet);
        //    }
        //    else 
        //    {
        //        return nullptr;
        //    }
        //}

        template<typename ComponentType, typename Key>
        SparseSet<ComponentType, Key>& getComponentSet() {
            auto it = m_componentStorage.find(typeid(ComponentType));
            if (it != m_componentStorage.end()) {
                return *static_cast<SparseSet<ComponentType, Key>*>(it->second.get());
            }
            else {
                auto set = std::make_shared<SparseSet<ComponentType, Key>>();
                m_componentStorage[typeid(ComponentType)] = set;
                return *set;
            }
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_componentStorage;
    };
}

#endif
