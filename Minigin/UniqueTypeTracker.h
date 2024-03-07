#ifndef UNIQUETYPETRACKER
#define UNIQUETYPETRACKER

#include <unordered_set>
#include <typeindex>
#include <iostream>
#include <string>


namespace Pengin
{
    inline std::unordered_set<std::type_index>& GetUniqueTypesSet()
    {
        static std::unordered_set<std::type_index> uniqueTypesSet;
        return uniqueTypesSet;
    }


    template<typename T>
    struct UniqueTypesTracker
    {
        inline static bool tracked = [] 
        {
            const auto& tyepId{ typeid(T) };

            std::cout << "Tracking unique type: " << tyepId.name() << "\n";
            GetUniqueTypesSet().insert(tyepId);

            return true;
        }();
    };
}

#endif