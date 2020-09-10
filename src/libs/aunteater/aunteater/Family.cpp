#include "Family.h"

#include "Engine.h"
#include "Entity.h"

#include <algorithm>


namespace aunteater {


template <FAMILY_TMP_PARAMS>
Family<FAMILY_TMP_ARGS>::Family(ArchetypeTypeSet aComponentsTypeInfo):
        mComponentsTypeInfo(std::move(aComponentsTypeInfo))
{}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::addIfMatch(weak_entity<Family> aEntity)
{
    if (std::all_of(mComponentsTypeInfo.begin(), mComponentsTypeInfo.end(),
                    [&aEntity](ComponentTypeId compId)
                    {
                        return aEntity->has(compId);
                    }))
    {
        auto insertedIt = mEntities.insert(mEntities.end(), aEntity);
        if (!mEntitiesPositions.emplace(entityIdFrom(*aEntity), insertedIt).second)
        {
            throw std::logic_error("Entity already present in this family.");
        }
        broadcastNotification(&FamilyObserver<Family>::addedEntity, **insertedIt);
    }
}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::removeIfPresent(entity_id<Family> aEntity)
{
    auto foundIt = mEntitiesPositions.find(entityIdFrom(*aEntity));
    if (foundIt != mEntitiesPositions.end())
    {
        broadcastNotification(&FamilyObserver<Family>::removedEntity, **(foundIt->second));
        mEntities.erase(foundIt->second);
        mEntitiesPositions.erase(foundIt);
    }
}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::componentAddedToEntity(weak_entity<Family> aEntity, ComponentTypeId /*aComponent*/)
{
    if(!isPresent(aEntity))
    {
        addIfMatch(aEntity);
    }
}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::componentRemovedFromEntity(entity_id<Family> aEntity, ComponentTypeId aComponent)
{
    if (includesComponent(aComponent))
    {
        removeIfPresent(aEntity);
    }
}

template <FAMILY_TMP_PARAMS>
bool Family<FAMILY_TMP_ARGS>::isPresent(entity_id<Family> aEntity) const
{
    return mEntitiesPositions.find(aEntity) != mEntitiesPositions.end();
}

template <FAMILY_TMP_PARAMS>
bool Family<FAMILY_TMP_ARGS>::includesComponent(ComponentTypeId aComponent) const
{
    return mComponentsTypeInfo.count(aComponent);
}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::broadcastNotification(NotificationMethod aTargetMethod, LiveEntity<Family> &aEntity) const
{
    for(auto observer : mObservers)
    {
        (observer->*aTargetMethod)(aEntity);
    }
}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::cancelObserverImpl(FamilyObserver<Family> *aObserver)
{
    /// \todo gsl::Expect prime candidate
    assert(std::find(mObservers.begin(), mObservers.end(), aObserver) != mObservers.end());
    mObservers.erase(std::find(mObservers.begin(), mObservers.end(), aObserver));
}

template <FAMILY_TMP_PARAMS>
void Family<FAMILY_TMP_ARGS>::notifyOfExistingEntities(FamilyObserver<Family> *aObserver)
{
    for(weak_entity<Family> entity : mEntities)
    {
        broadcastNotification(&FamilyObserver<Family>::addedEntity, *entity);
    };
}

template <FAMILY_TMP_PARAMS>
auto Family<FAMILY_TMP_ARGS>::registerObserver(FamilyObserver<Family> *aObserver) -> Family &
{
    mObservers.push_back(aObserver);
    notifyOfExistingEntities(aObserver);
    return *this;
}

template <FAMILY_TMP_PARAMS>
auto Family<FAMILY_TMP_ARGS>::cancelObserver(FamilyObserver<Family> *aObserver) -> Family &
{
    cancelObserverImpl(aObserver);
    return *this;
}

template class Family<list_entity>;

} // namespace aunteater
