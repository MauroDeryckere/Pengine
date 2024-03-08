#ifndef UNIQUETYPETRACKER
#define UNIQUETYPETRACKER

#include <unordered_set>
#include <typeindex>
#include <iostream>
#include <string>


namespace Pengin
{
    class UniqueTypes {
    public:
        static std::unordered_set<std::type_index>& GetSet() 
        {
            static std::unordered_set<std::type_index> uniqueTypesSet;
            return uniqueTypesSet;
        }
        static const std::unordered_set<std::type_index>& GetConstSet()
        {
            return GetSet();
        }
    };

    template<typename T>
    struct UniqueTypesTracker
    {
        inline static bool tracked = [] 
        {
            const auto& tyepId{ typeid(T) };

            std::cout << "Tracking unique type: " << tyepId.name() << "\n";
            UniqueTypes::GetSet().insert(tyepId);

            return true;
        }();
    };
}

#endif