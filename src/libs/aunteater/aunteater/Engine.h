#pragma once

#include "Entity.h"
#include "Family.h"
#include "Timer.h"

#include "globals.h"

#include <boost/bimap.hpp>

#include <map>
#include <string>
#include <vector>

namespace aunteater
{


struct EngineTag
{
private:
    template <ENGINE_TMP_PARAMS>
    friend class Engine;
    EngineTag() = default;
};

/// \brief LiveEntity class
template <LIVE_TMP_PARAMS>
class LiveEntity
{

    template <FAMILY_TMP_PARAMS>
    friend class Family;

    // Non-copiable, non-movable
    LiveEntity(const LiveEntity&) = delete;
    LiveEntity & operator=(const LiveEntity&) = delete;

public:
    // Note: Only constructible by Engine
    LiveEntity(Entity aEntity, Engine<ENGINE_TMP_ARGS> & aEngine, EngineTag) :
        mEntity(std::move(aEntity)),
        mEngine(aEngine)
    {}

    /// \brief Return an Entity value by copying the wrapped entity
    explicit operator Entity()
    {
        return mEntity;
    }

    template <class T_component, class... Args>
    LiveEntity & add(Args&&... aArgs);

    /// \brief Removes the component of type T_component from this Entity.
    template <class T_component>
    LiveEntity & remove();

    template <class T_component>
    bool has()
    {
        return mEntity.has<T_component>();
    }

    template <class T_component>
    T_component & get()
    {
        return mEntity.get<T_component>();
    }

    template <class T_component>
    const T_component & get() const
    {
        return mEntity.get<T_component>();
    }

    void markToRemove();

private:

    /// \deprecated Currently required for Family inclusion test
    bool has(ComponentTypeId aId)
    {
        return mEntity.has(aId);
    }

private:
    Entity mEntity;
    Engine<ENGINE_TMP_ARGS> &mEngine;
};


template <ENGINE_TMP_PARAMS>
class Engine
{
    // The engine register its address to the Entities added to it: it cannot have copy/move semantic.
    Engine(const Engine&) = delete;
    Engine & operator=(const Engine&) = delete;

public:
    /*
     * Construction
     */
    Engine() = default;

    /*
     * Entities manipulation
     */
    weak_entity<LIVE_TMP_ARGS> addEntity(Entity aEntity);
    weak_entity<LIVE_TMP_ARGS> addEntity(const std::string & aName, Entity aEntity);

    void markToRemove(weak_entity<LIVE_TMP_ARGS> aEntity)
    {
        mEntitiesToRemove.emplace(aEntity);
    }
    void markToRemove(const std::string & aEntityName)
    {
        markToRemove(getEntity(aEntityName));
    }

    weak_entity<LIVE_TMP_ARGS> getEntity(const std::string & aEntityName)
    {
        return mNamedEntities.left.find(aEntityName)->second;
    }

    std::size_t countEntities() const
    {
        return mEntities.size();
    }

    /*
     * Families
     */
    template <class T_archetype>
    T_family & getFamily();

    /*
     * System
     */
    /// \brief Instantiate and add a system of class T_system.
    ///
    ///  Provides engine reference as 1st ctor argument, forwards aArgs as following arguments.
    template <class T_system, class... VT_ctorArgs>
    std::shared_ptr<T_system> addSystem(VT_ctorArgs &&... aArgs);

    void addSystem(std::shared_ptr<System> aSystem);

    /*
     * Update
     */
    template <class T_updater>
    void update(const Timer aTime, T_updater && aUpdater);

    void update(const Timer aTime);

    /// \return The pause state before the call
    bool isPaused();
    /// \return The pause state before the call
    bool pause(bool aPauseMode);

    void forEachFamily(std::function<void(T_family &aFamily)> aFamilyFunctor)
    {
        for (auto &typedFamily : mTypedFamilies)
        {
            aFamilyFunctor(typedFamily.second);
        }
    }

protected:
    void removeEntities();
    void notifyAdditionToFamilies(weak_entity<LIVE_TMP_ARGS> aEntity);
    void notifyRemovalToFamilies(entity_id<LIVE_TMP_ARGS> aEntity);

private:
    typedef boost::bimap<std::string, weak_entity<LIVE_TMP_ARGS> > NameEntityMap;
    typedef std::map<ArchetypeTypeId, T_family> ArchetypeFamilyMap;

    std::list<LiveEntity<LIVE_TMP_ARGS>> mEntities;
    NameEntityMap mNamedEntities;
    std::set<weak_entity<LIVE_TMP_ARGS>> mEntitiesToRemove;
    ArchetypeFamilyMap mTypedFamilies;
    bool mPaused{false};

protected:
    std::vector<std::shared_ptr<System>> mSystems;
};


/*
 * Implementations
 */

template <LIVE_TMP_PARAMS>
void LiveEntity<LIVE_TMP_ARGS>::markToRemove()
{
    mEngine.markToRemove(this);
}

template <LIVE_TMP_PARAMS>
template <class T_component, class... Args>
LiveEntity<LIVE_TMP_ARGS> & LiveEntity<LIVE_TMP_ARGS>::add(Args&&... aArgs)
{
    mEntity.add<T_component>(std::forward<Args>(aArgs)...);

    // Note: does not test if insertion actually took place (return value from addComponent())
    //       It is expected to be rare to replace a component this way, so avoid branching
    //       (i.e. always iterate all families, not necessary in the rare replace situation)
    mEngine.forEachFamily([this](T_family &family)
    {
       family.componentAddedToEntity(this, type<T_component>());
    });
    return *this;
}

/// \brief Removes the component of type T_component from this Entity.
template <LIVE_TMP_PARAMS>
template <class T_component>
LiveEntity<LIVE_TMP_ARGS> & LiveEntity<LIVE_TMP_ARGS>::remove()
{
    mEngine.forEachFamily([this](T_family &family)
    {
      family.componentRemovedFromEntity(entityIdFrom(*this), type<T_component>());
    });
    mEntity.remove<T_component>();
    return *this;
}

template <ENGINE_TMP_PARAMS>
template <class T_system, class... VT_ctorArgs>
std::shared_ptr<T_system> Engine<ENGINE_TMP_ARGS>::addSystem(VT_ctorArgs &&... aArgs)
{
    auto result = std::make_shared<T_system>(*this, std::forward<VT_ctorArgs>(aArgs)...);
    addSystem(result);
    return result;
}

template <ENGINE_TMP_PARAMS>
template <class T_archetype>
T_family & Engine<ENGINE_TMP_ARGS>::getFamily()
{
    auto insertionResult = mTypedFamilies.emplace(archetypeTypeId<T_archetype>(),
                                                  T_archetype::TypeSet());
    if (insertionResult.second)
    {
        T_family &familyRef = insertionResult.first->second;
        for (LiveEntity<LIVE_TMP_ARGS> & entity : mEntities)
        {
            familyRef.addIfMatch(entityRefFrom(entity));
        }
    }
    return insertionResult.first->second;
}

template <ENGINE_TMP_PARAMS>
template <class T_updater>
void Engine<ENGINE_TMP_ARGS>::update(const Timer aTime, T_updater && aUpdater)
{
    if (isPaused())
    {
        return;
    }

    aUpdater.start();

    for (auto & system : mSystems)
    {
        aUpdater(*system, aTime);
    }
    removeEntities();

    aUpdater.finish();
}

} // namespace aunteater