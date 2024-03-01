#ifndef COMPONENTSETHANDLER
#define COMPONENTSETHANDLER

#include "SparseSet.h"

#include <memory>

namespace Pengin
{
    class ComponentSetHandler
    {
    public:
        virtual ~ComponentSetHandler() = default;

        //virtual ...
    };

    template<typename ComponentType, typename Key>
    class TypedComponentSetHandler final :
        public ComponentSetHandler
    {
    public:
        TypedComponentSetHandler() : ComponentSetHandler{}, m_ComponentSet{} {}


        SparseSet<ComponentType, Key> m_ComponentSet; //Hide later, testing
    private:
    };
}

#endif