#ifndef COMPONENTSTORAGE
#define COMPONENTSTORAGE

#include <unordered_map>
#include <typeindex>
#include <limits>
#include <any>


namespace Pengin
{
    template<typename ComponentType>
    class ComponentStorage
    {
    public:

    private:
        std::vector<ComponentType> m_DenseComponentArr;
    };
}

#endif

