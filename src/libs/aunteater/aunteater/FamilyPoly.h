#pragma once


#include "Archetype.h"
#include "globals.h"
//#include "templating.h"


#include <memory>


namespace aunteater {

struct FamilyBase
{
    virtual ~FamilyBase() = default;
};


template <class T_archetype>
class FamilyTyped;

template <class... VT_component>
class FamilyTyped<Archetype<VT_component...>> : public FamilyBase
{
    using Node = std::tuple<VT_component *...> ;

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

    std::size_t size() const noexcept
    { return mNodes.size(); }

private:
    std::vector<Node> mNodes;
};


class FamilyPoly
{
public:
    template <class T_archetype>
    static FamilyPoly Construct()
    {
        return FamilyPoly{std::make_unique<FamilyTyped<T_archetype>>()};
    }

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

    void componentAddedToEntity(weak_entity<FamilyPoly> aEntity, ComponentTypeId aComponent);
    void componentRemovedFromEntity(entity_id<FamilyPoly> aEntity, ComponentTypeId aComponent);

    std::unique_ptr<FamilyBase> mDerived{nullptr};

private:
    FamilyPoly(std::unique_ptr<FamilyBase> aDerived) :
        mDerived{std::move(aDerived)}
    {}
};


// TODO
inline void FamilyPoly::addIfMatch(weak_entity<FamilyPoly> aEntity)
{}
inline void FamilyPoly::removeIfPresent(entity_id<FamilyPoly> aEntity)
{}

inline void FamilyPoly::componentAddedToEntity(weak_entity<FamilyPoly> aEntity, ComponentTypeId aComponent)
{}
inline void FamilyPoly::componentRemovedFromEntity(entity_id<FamilyPoly> aEntity, ComponentTypeId aComponent)
{}


} // namespace aunteater
