#ifndef UNIQUETYPETRACKER
#define UNIQUETYPETRACKER

#include <unordered_set>
#include <typeindex>
#include <iostream>
#include <string>


namespace Pengin
{
    inline std::unordered_set<std::type_index>& GetGlobalUniqueTypesSet()
    {
        static std::unordered_set<std::type_index> uniqueTypesSet;
        return uniqueTypesSet;
    }


    template<typename T>
    struct UniqueTypesTracker
    {
        inline static bool tracked = [] 
        {
            std::cout << "Tracking unique type: " << typeid(T).name() << "\n\n";
            GetGlobalUniqueTypesSet().insert(typeid(T));

            return true;
        }();
    };
}

#endif