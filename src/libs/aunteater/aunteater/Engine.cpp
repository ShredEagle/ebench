#include "Engine.h"
#include "System.h"

#include "Component.h"


namespace aunteater {

template <ENGINE_TMP_PARAMS>
weak_entity<LIVE_TMP_ARGS> Engine<ENGINE_TMP_ARGS>::addEntity(const std::string & aName, Entity aEntity)
{
    auto insertionResult = mNamedEntities.left.insert(std::make_pair(aName, nullptr));
    if (!insertionResult.second)
    {
        throw std::invalid_argument("Named entity is already present in the engine"
                                    "with the same name.");
    }

    /// \todo Multithreading issue ahead
    // Note: cannot be done directly at insert, because addEntity has side effects which should
    //       not take place if the name was already present
    auto id = addEntity(std::move(aEntity));
    mNamedEntities.left.replace_data(insertionResult.first, id);
    return id;
}

template <ENGINE_TMP_PARAMS>
weak_entity<LIVE_TMP_ARGS> Engine<ENGINE_TMP_ARGS>::addEntity(Entity aEntity)
{
    // Note: cannot emplace_back, before C++17 it returns void
    weak_entity<LIVE_TMP_ARGS> lastEntity =
        entityRefFrom(*mEntities.emplace(mEntities.end(), std::move(aEntity), *this, EngineTag{}));
    notifyAdditionToFamilies(lastEntity);
    return lastEntity;
}

/// TODO revisit, it compares an EntityId to a weak_entity, causing const-ness complications
///   (low level const in entity_id prevents matching against keys that are not low level const)
///   Take a look at "Transparent comparators"
///   Should allow constness on the mEntitiesToRemove pointers
template <ENGINE_TMP_PARAMS>
void Engine<ENGINE_TMP_ARGS>::removeEntities()
{
    /// TODO Measure if it is more efficient:
    ///   * to iterate flagged entities and look them up one by one in the associative containers (current)
    ///   * to iterate the associative containers entirely each time, and remove matching flagged entities
    /// TODO more constness here

    std::set<weak_entity<LIVE_TMP_ARGS>> removedEntities;
    // Note: this outer loop is required in case a family observer marks additional nodes for removal
    // see Engine_tests.cpp "Nested entities removal" scenario
    while(!mEntitiesToRemove.empty())
    {
        for (auto entityIt = mEntitiesToRemove.begin();
             entityIt != mEntitiesToRemove.end();
             /* in body */)
        {
            mNamedEntities.right.erase(*entityIt);
            notifyRemovalToFamilies(*entityIt);
            // set::extract() invalidates the current iterator: use post-increment to return a copy
            removedEntities.insert(mEntitiesToRemove.extract(entityIt++));
        }
    }

    /// TODO LiveEntity should be const here
    mEntities.remove_if([&removedEntities](LiveEntity<LIVE_TMP_ARGS> &aElem)
    {
        return removedEntities.count(&aElem); // contains() is C++20
    });
}


template <ENGINE_TMP_PARAMS>
void Engine<ENGINE_TMP_ARGS>::notifyAdditionToFamilies(weak_entity<LIVE_TMP_ARGS> aEntity)
{
    for (auto & typedFamily : mTypedFamilies)
    {
        typedFamily.second.addIfMatch(aEntity);
    }
}

template <ENGINE_TMP_PARAMS>
void Engine<ENGINE_TMP_ARGS>::notifyRemovalToFamilies(entity_id<LIVE_TMP_ARGS> aEntity)
{
    for (auto & typedFamily : mTypedFamilies)
    {
        typedFamily.second.removeIfPresent(aEntity);
    }
}

template <ENGINE_TMP_PARAMS>
void Engine<ENGINE_TMP_ARGS>::addSystem(std::shared_ptr<System> aSystem)
{
    mSystems.push_back(std::move(aSystem));
}

struct DefaultUpdater
{
    void start()
    {}
    void finish()
    {}

    void operator()(System & aSystem, const Timer aTime)
    {
        aSystem.update(aTime);
    }
};

template <ENGINE_TMP_PARAMS>
void Engine<ENGINE_TMP_ARGS>::update(const Timer aTime)
{
    update(aTime, DefaultUpdater{});
}

template <ENGINE_TMP_PARAMS>
bool Engine<ENGINE_TMP_ARGS>::isPaused()
{
    return mPaused;
}

template <ENGINE_TMP_PARAMS>
bool Engine<ENGINE_TMP_ARGS>::pause(bool aPauseMode)
{
    bool result = isPaused();
    mPaused = aPauseMode;
    return result;
}


// Implicit instantiations
template class Engine<Family<list_entity>>;
template class Engine<Family<vector_entity>>;

} // namespace aunteater
