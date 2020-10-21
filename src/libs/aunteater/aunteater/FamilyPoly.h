#pragma once


#include "Archetype.h"
#include "Engine.h" // coming real close to circular inclusion here...
#include "globals.h"
//#include "templating.h"


#include <memory>
#include <iostream>


namespace aunteater {

class FamilyPoly;

struct FamilyBase
{
    virtual ~FamilyBase() = default;

    virtual std::size_t size() const noexcept = 0;

    virtual void addIfMatch(weak_entity<FamilyPoly> aEntity) = 0;
    virtual void removeIfPresent(entity_id<FamilyPoly> aEntity) = 0;
};


template <class T_archetype>
class FamilyTyped;



template <class T_componentHead, class... VT_componentTail>
bool hasAllComponents(weak_entity<FamilyPoly> aEntity)
{
    return aEntity->has<T_componentHead>()
           && hasAllComponents<VT_componentTail...>(aEntity);
}

template <void * Dummy = nullptr>
bool hasAllComponents(weak_entity<FamilyPoly> aEntity)
{
    return true;
}


template <class... VT_component>
class FamilyTyped<Archetype<VT_component...>> : public FamilyBase
{
    struct Node
    {
        weak_entity<FamilyPoly> entity;
        std::tuple<VT_component *...> components;
    };

public:
    auto begin() noexcept
    { return mNodes.begin(); }
    auto begin() const noexcept
    { return mNodes.begin(); }
    auto cbegin() const noexcept
    { return mNodes.cbegin(); }

    auto end() noexcept
    { return mNodes.end(); }
    auto end() const noexcept
    { return mNodes.end(); }
    auto cend() const noexcept
    { return mNodes.cend(); }

    std::size_t size() const noexcept override
    { return mNodes.size(); }

    void addIfMatch(weak_entity<FamilyPoly> aEntity) override
    {
        if (hasAllComponents<VT_component...>(aEntity))
        {
            // emplace_back not working with aggregate init before C++20
            mNodes.push_back(Node{aEntity, std::make_tuple(&(aEntity->get<VT_component>())...)});
            if (!mEntitiesPositions.emplace(entityIdFrom(*aEntity), mNodes.size()-1).second)
            {
                throw std::logic_error("Entity already present in this family.");
            }
            //broadcastNotification(&FamilyObserver<Family>::addedEntity, **insertedIt);
        }
    }

    void removeIfPresent(entity_id<FamilyPoly> aEntity) override
    {
        auto foundIt = mEntitiesPositions.find(entityIdFrom(*aEntity));
        if (foundIt != mEntitiesPositions.end())
        {
            //broadcastNotification(&FamilyObserver<Family>::removedEntity, **(foundIt->second));
            mNodes.erase(mNodes.begin() + foundIt->second);
            mEntitiesPositions.erase(foundIt);
        }
    }

private:
    std::vector<Node> mNodes;
    std::map<entity_id<FamilyPoly>, typename std::vector<Node>::size_type> mEntitiesPositions;
};


class FamilyPoly
{
public:
    template <class T_archetype>
    static FamilyPoly Construct()
    {
        return FamilyPoly{std::make_unique<FamilyTyped<T_archetype>>()};
    }

    std::size_t size() const noexcept;

    // TODO
    //auto begin() noexcept;
    //auto begin() const noexcept;
    //auto cbegin() const noexcept;

    //auto end() noexcept;
    //auto end() const noexcept;
    //auto cend() const noexcept;

//private:
    void addIfMatch(weak_entity<FamilyPoly> aEntity);
    void removeIfPresent(entity_id<FamilyPoly> aEntity);

    // TODO
    //void componentAddedToEntity(weak_entity<FamilyPoly> aEntity, ComponentTypeId aComponent);
    //void componentRemovedFromEntity(entity_id<FamilyPoly> aEntity, ComponentTypeId aComponent);

    std::unique_ptr<FamilyBase> mDerived{nullptr};

private:
    FamilyPoly(std::unique_ptr<FamilyBase> aDerived) :
        mDerived{std::move(aDerived)}
    {}
};


inline void FamilyPoly::addIfMatch(weak_entity<FamilyPoly> aEntity)
{
    mDerived->addIfMatch(aEntity);
}

inline void FamilyPoly::removeIfPresent(entity_id<FamilyPoly> aEntity)
{
    mDerived->removeIfPresent(aEntity);
}

inline std::size_t FamilyPoly::size() const noexcept
{
    return mDerived->size();
}

//inline void FamilyPoly::componentAddedToEntity(weak_entity<FamilyPoly> aEntity, ComponentTypeId aComponent)
//{}
//inline void FamilyPoly::componentRemovedFromEntity(entity_id<FamilyPoly> aEntity, ComponentTypeId aComponent)
//{}


} // namespace aunteater
