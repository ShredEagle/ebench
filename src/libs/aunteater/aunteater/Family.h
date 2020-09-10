#pragma once

#include "Archetype.h"
#include "Entity.h"

#include "globals.h"
#include "templating.h"

#include <map>
#include <list>
#include <vector>

namespace aunteater
{
    template <class>
    class Engine;

    template <LIVE_TMP_PARAMS>
    class FamilyObserver
    {
    public:
        virtual void addedEntity(LiveEntity<LIVE_TMP_ARGS> &aEntity) = 0;
        virtual void removedEntity(LiveEntity<LIVE_TMP_ARGS> &aEntity) = 0;

        virtual ~FamilyObserver() = default;
    };


    // The implementation assumes that inserting/removing an Entity from the collection
    // does not invalidate any existing iterator into the collection.
    //typedef std::list<weak_entity<Family>> EntityList;
    //typedef std::vector<weak_entity> EntityList;

    template <LIVE_TMP_PARAMS>
    using list_entity = std::list<weak_entity<LIVE_TMP_ARGS>>;

    template <LIVE_TMP_PARAMS>
    using vector_entity = std::vector<weak_entity<LIVE_TMP_ARGS>>;


    // Review note: Should probably be non-copyable,

    template <FAMILY_TMP_PARAMS>
    class Family
    {
        // Needs to call addIfMatch
        template <ENGINE_TMP_PARAMS>
        friend class Engine;
        //// Needs to call componentAddedTo(RemovedFrom)Entity
        //friend class LiveEntity;

        using EntityList = TT_entityCollection<Family>;
    public:
        // Note: must be accessible to pair forwarded constructor (cannot be private)
        explicit Family(ArchetypeTypeSet aComponentsTypeInfo);

        // Non-copyable, to avoid the risk of a component forgetting to take it by ref
        //
        // Implementer's note: aOther is *const* reference, otherwise it cannot be used in a pair
        // see https://stackoverflow.com/q/22357887/1027706
        // Yes, C++ requires you to delete the right signature
        Family(const Family & aOther) = delete;
        Family & operator=(Family & aOther) = delete;

        /// \brief Inteded for client use, not used as an internal mechanism
        Family & registerObserver(FamilyObserver<Family> *aObserver);
        Family & cancelObserver(FamilyObserver<Family> *aObserver);

        std::size_t size() const noexcept;

        auto begin() noexcept;
        auto begin() const noexcept;
        auto cbegin() const noexcept;

        auto end() noexcept;
        auto end() const noexcept;
        auto cend() const noexcept;

        typename EntityList::iterator find(entity_id<Family> aEntityId);
        typename EntityList::const_iterator find(entity_id<Family> aEntityId) const;

        /// \brief Returns a weak_entity if it is part of this family, nullptr otherwise
        /// \note This makes for an easy test in conditions
        weak_entity<Family> contains(entity_id<Family> aEntityId);

    private:
        bool isPresent(entity_id<Family> aEntity) const;
        bool includesComponent(ComponentTypeId aComponent) const;

        void addIfMatch(weak_entity<Family> aEntity);
        void removeIfPresent(entity_id<Family> aEntity);

        void componentAddedToEntity(weak_entity<Family> aEntity, ComponentTypeId aComponent);
        void componentRemovedFromEntity(entity_id<Family> aEntity, ComponentTypeId aComponent);

        typedef void (FamilyObserver<Family>::*NotificationMethod)(LiveEntity<Family> &);
        void broadcastNotification(NotificationMethod aTargetMethod, LiveEntity<Family> & aEntity) const;
        void cancelObserverImpl(FamilyObserver<Family> *aObserver);
        void notifyOfExistingEntities(FamilyObserver<Family> *aObserver);

    private:
        EntityList mEntities;
        ArchetypeTypeSet mComponentsTypeInfo;

        // Note: This map is usefull to test if an entity is present in the current family instance
        //       It works to use an iterator because for an std::list
        //       operations on an element do not affect other iterators.
        std::map<entity_id<Family>, typename EntityList::iterator> mEntitiesPositions;

        // TODO This is unsafe, if an observer expires without removing itself
        // Refactor with shared and weak ptrs ?
        std::vector<FamilyObserver<Family> *> mObservers;
    };

    /***
     * Implementations
     ***/
    template <FAMILY_TMP_PARAMS>
    std::size_t Family<FAMILY_TMP_ARGS>::size() const noexcept
    {
        return mEntities.size();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::begin() noexcept
    {
        return mEntities.begin();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::begin() const noexcept
    {
        return mEntities.cbegin();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::cbegin() const noexcept
    {
        return mEntities.cbegin();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::end() noexcept
    {
        return mEntities.end();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::end() const noexcept
    {
        return mEntities.cend();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::cend() const noexcept
    {
        return mEntities.cend();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::find(entity_id<Family> aEntityId) -> typename Family::EntityList::iterator
    {
        auto found = mEntitiesPositions.find(aEntityId);
        return (found != mEntitiesPositions.end()) ? found->second
                                                   : end();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::find(entity_id<Family> aEntityId) const -> typename EntityList::const_iterator
    {
        auto found = mEntitiesPositions.find(aEntityId);
        return (found != mEntitiesPositions.end()) ? found->second
                                                   : end();
    }

    template <FAMILY_TMP_PARAMS>
    auto Family<FAMILY_TMP_ARGS>::contains(entity_id<Family> aEntityId) -> weak_entity<Family>
    {
        auto found = mEntitiesPositions.find(aEntityId);
        return (found != mEntitiesPositions.end()) ? *(found->second)
                                                   : nullptr;
    }

} // namespace aunteater
