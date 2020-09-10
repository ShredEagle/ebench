#pragma once


#include <aunteater/Component.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {
namespace ebench {


using Floating = double;

// IMPORTANT: NodeList is not equilvalent to aunteater::list_entity
// see https://stackoverflow.com/q/32723988/1027706
//template <class T_family>
//using NodeList = aunteater::list_entity<T_family>;

#define NodeList aunteater::list_entity

using Family = aunteater::Family<NodeList>;
using Engine = aunteater::Engine<Family>;

#undef NodeList

struct Position : public aunteater::Component<Position>
{
    Floating x;
    Floating y;
};


struct Displacement : public aunteater::Component<Displacement>
{
    Floating x{1.1f};
    Floating y{2.5f};
};


using Movable = aunteater::Archetype<Displacement, Position>;

// Note: Use the family as a type template parameter, instead of the collection as a template template parameter
// this is mostly because there is no good way to make an alias for a template template argument
// (the template and its alias are considered different)
//template <template <class> class TT_entityCollection>
template <class T_family>
class MovementSystem : public aunteater::System
{
public:
    MovementSystem(Engine & aEngine) :
        mMovables{aEngine.getFamily<Movable>()}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & entity : mMovables)
        {
            auto & position = entity->template get<Position>();
            auto & displacement = entity->template get<Displacement>();

            position.x += aTimer.delta() * displacement.x;
            position.y += aTimer.delta() * displacement.y;
        }
    }

private:
    T_family & mMovables;
};


template <class T_family>
class MovementSystem_FamilyHelp : public aunteater::System
{
public:
    MovementSystem_FamilyHelp(Engine & aEngine) :
        mMovables{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [displacement, position] : mMovables)
        {
            position.x += aTimer.delta() * displacement.x;
            position.y += aTimer.delta() * displacement.y;
        }
    }

private:
    aunteater::FamilyHelp<T_family, Movable> mMovables;
};


using Positioned = aunteater::Archetype<Position>;


template <class T_family>
class SumSystem : public aunteater::System
{
public:
    SumSystem(Engine & aEngine) :
        mPositioneds{aEngine.getFamily<Positioned>()}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & entity : mPositioneds)
        {
            auto & position = entity->template get<Position>();

            mSum += position.x;
        }
    }

    Floating getSum() const
    {
        return mSum;
    }

private:
    T_family & mPositioneds;
    Floating mSum{0.};
};


template <class T_family>
class SumSystem_FamilyHelp : public aunteater::System
{
public:
    SumSystem_FamilyHelp(Engine & aEngine) :
        mPositioneds{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [position] : mPositioneds)
        {
            mSum += position.x;
        }
    }

    Floating getSum() const
    {
        return mSum;
    }

private:
    aunteater::FamilyHelp<T_family, Positioned> mPositioneds;
    Floating mSum{0.};
};


} // namespace ebench
} // namespace ad
