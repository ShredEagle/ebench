#pragma once

#include "Archetype.h"
#include "Engine.h"
#include "Family.h"

#include "templating.h"


namespace aunteater {


    template <ENTITYWRAP_TMP_PARAMS, class T_iterator, class T_archetype>
    class EntityWrap;

    template <ENTITYWRAP_TMP_PARAMS, class T_iterator, class... VT_components>
    class EntityWrap<ENTITYWRAP_TMP_ARGS, T_iterator, Archetype<VT_components...>>
    {
        template <class>
        friend class FamilyHelp;

        typedef T_iterator iterator;

        EntityWrap(iterator aIterator) :
            mEntity(aIterator)
        {}

    public:

        weak_entity<LIVE_TMP_ARGS> operator->() const
        {
            return *mEntity;
        }

        /*implicit*/ operator weak_entity<LIVE_TMP_ARGS>() const
        {
            return *mEntity;
        }

        /// TODO should it be implicit?
        explicit operator LiveEntity<LIVE_TMP_ARGS> & () const
        {
            return **mEntity;
        }

        // Part of structured binding implementation
        template <std::size_t I>
        auto & get() &
        {
            return (*mEntity)->template get<typename std::tuple_element<I, EntityWrap>::type>();
        }

        template <std::size_t I>
        auto get() &&
        {
            return (*mEntity)->template get<typename std::tuple_element<I, EntityWrap>::type>();
        }

        template <std::size_t I>
        auto & get() const &
        {
            return (*mEntity)->template get<typename std::tuple_element<I, EntityWrap>::type>();
        }

        template <std::size_t I>
        auto get() const &&
        {
            return (*mEntity)->template get<typename std::tuple_element<I, EntityWrap>::type>();
        }

        // Note: This operator has to behave in a surprising manner:
        //  It will be used by the range-base for loop, assigning its result
        //  to the "range_declaration".
        //  If it behaved in a normal manner (returning the pointed to object, i.e. weak_entity),
        //  it would lose all benefits from wrapping
        //  (i.e. structured binding and statically checked get)
        EntityWrap & operator*()
        {
            return *this;
        }

        auto operator==(const EntityWrap & aOther) const
        {
            return mEntity == aOther.mEntity;
        }

        auto operator!=(const EntityWrap & aOther) const
        {
            return mEntity != aOther.mEntity;
        }

        EntityWrap & operator++()
        {
            ++mEntity;
            return *this;
        }

        EntityWrap operator++(int)
        {
            return mEntity++;
        }

    private:
         iterator mEntity;
    };


    template <HELP_TMP_PARAMS, class T_archetype>
    class FamilyHelp;

    template <HELP_TMP_PARAMS, class... VT_components>
    class FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>
    {
    public:
        using Wrap = EntityWrap<ENTITYWRAP_TMP_ARGS,
                                decltype(std::declval<T_family>().begin()),
                                Archetype<VT_components...>>;
        using const_Wrap = EntityWrap<ENTITYWRAP_TMP_ARGS,
                                      decltype(std::declval<T_family>().cbegin()),
                                      Archetype<VT_components...>>;

        FamilyHelp(Engine<ENGINE_TMP_ARGS> & aEngine) :
            mFamily(aEngine.template getFamily<Archetype<VT_components...>>())
        {}

        template <class T_functor>
        void forEach(const T_functor & aOperation) const;

        std::size_t size() const noexcept;

        Wrap begin() noexcept;
        const_Wrap begin() const noexcept;
        const_Wrap cbegin() const noexcept;

        Wrap end() noexcept;
        const_Wrap end() const noexcept;
        const_Wrap cend() const noexcept;

        Wrap find(entity_id<LIVE_TMP_ARGS> aEntityId);
        const_Wrap find(entity_id<LIVE_TMP_ARGS> aEntityId) const;

    private:
        T_family & mFamily;
    };

    /***
     * Implementations
     ***/
    template <HELP_TMP_PARAMS, class... VT_components>
    template <class T_functor>
    void FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::forEach(const T_functor &aOperation) const
    {
        for(auto entity : mFamily)
        {
            aOperation(entity->template get<VT_components>()...);
        }
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    std::size_t FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::size() const noexcept
    {
        return mFamily.size();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::begin() noexcept -> Wrap
    {
        return mFamily.begin();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::begin() const noexcept -> const_Wrap
    {
        return mFamily.cbegin();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::cbegin() const noexcept -> const_Wrap
    {
        return mFamily.cbegin();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::end() noexcept -> Wrap
    {
        return mFamily.end();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::end() const noexcept -> const_Wrap
    {
        return mFamily.cend();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::cend() const noexcept -> const_Wrap
    {
        return mFamily.cend();
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::find(entity_id<LIVE_TMP_ARGS> aEntityId) -> Wrap
    {
        return mFamily.find(aEntityId);
    }

    template <HELP_TMP_PARAMS, class... VT_components>
    auto FamilyHelp<HELP_TMP_ARGS, Archetype<VT_components...>>::find(entity_id<LIVE_TMP_ARGS> aEntityId) const -> const_Wrap
    {
        // Top-level const on FamilyHelp does not become low-level const on referenced Family
        // (i.e. same as for pointer data members, we would need propagate_const<>)
        return static_cast<const T_family &>(mFamily).find(aEntityId);
    }

} // namespace aunteater

//
// Structured binding implementation
//
namespace std {

template <ENTITYWRAP_TMP_PARAMS, class T_iterator, class T_archetype>
class tuple_size<aunteater::EntityWrap<ENTITYWRAP_TMP_ARGS, T_iterator, T_archetype>> :
    public std::integral_constant<size_t, aunteater::Archetype_size<T_archetype>::value>
{};

template <std::size_t I, ENTITYWRAP_TMP_PARAMS, class T_iterator, class T_archetype>
class tuple_element<I, aunteater::EntityWrap<ENTITYWRAP_TMP_ARGS, T_iterator, T_archetype>> :
    public aunteater::Archetype_element<I, T_archetype>
{};


} // namespace std
