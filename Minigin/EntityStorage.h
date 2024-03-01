#ifndef ENTITYSTORAGE
#define ENTITYSTORAGE

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <vector>
#include <typeindex>

namespace dae
{
    class EntityStorage
    {
    public:
        EntityStorage() = default;

        unsigned CreateEntity()
        {
            ++m_EntityCount;

            assert(m_EntityCount != std::numeric_limits<unsigned>::max() && "Maximum entity count reached");

            m_Entities.insert(m_EntityCount);
            m_ComponentFlags[m_EntityCount] = {};

            return m_EntityCount;
        };


    private:
        std::unordered_set<unsigned> m_Entities;
        std::unordered_map<unsigned, std::vector<std::type_index>> m_ComponentFlags;
        //while bitflags are more optimized, it comes with more development time, could be future implementation
        //std::unordered_map<unsigned, std::bitset<MAXCOMPONENTS>> m_ComponentFlags;

        unsigned m_EntityCount{ 0 };
    };
}

#endif

