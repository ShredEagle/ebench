#pragma once

#include "templating.h"

#include <memory>
#include <set>
#include <typeinfo>

namespace aunteater
{
    class ComponentBase;
    template <LIVE_TMP_PARAMS> class LiveEntity;
    class System;

    typedef const std::type_info * ComponentTypeId;


    /// \brief Returns the ComponentTypeId for the provided Component type (T_component)
    template <class T_component>
    static typename std::enable_if_t<std::is_base_of<ComponentBase, T_component>::value,
                                     ComponentTypeId>
    type()
    {
        return &typeid(T_component);
    }

    /// \todo We do not want to share ownership : Entity is semantically owning its components,
    /// other objects are refering to them.
    template <class T_component = ComponentBase>
    using own_component = std::unique_ptr<T_component>;

    typedef const std::type_info *  ArchetypeTypeId;
    template <class T_Archetype>
    constexpr ArchetypeTypeId archetypeTypeId()
    {
        return & typeid(T_Archetype);
    }

    template <LIVE_TMP_PARAMS>
    using entity_id = const LiveEntity<LIVE_TMP_ARGS>*; // Could alternatively be a const void*: should never be used to invoke methods.

    template <LIVE_TMP_PARAMS>
    using weak_entity= LiveEntity<LIVE_TMP_ARGS>*;

    template <LIVE_TMP_PARAMS>
    using const_weak_entity= const LiveEntity<LIVE_TMP_ARGS>*;

    template <LIVE_TMP_PARAMS>
    entity_id<LIVE_TMP_ARGS> entityIdFrom(const LiveEntity<LIVE_TMP_ARGS> &aWrapper)
    {
        return &aWrapper;
    }

    template <LIVE_TMP_PARAMS>
    entity_id<LIVE_TMP_ARGS> entityIdFrom(weak_entity<LIVE_TMP_ARGS> aWeakEntity)
    {
        return aWeakEntity;
    }

    template <LIVE_TMP_PARAMS>
    weak_entity<LIVE_TMP_ARGS> entityRefFrom(LiveEntity<LIVE_TMP_ARGS> &aWrapper)
    {
        return &aWrapper;
    }

} // namespace aunteater
