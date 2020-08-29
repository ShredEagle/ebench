#pragma once


#include <aunteater/Component.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {
namespace ebench {


using Floating = double;


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

class MovementSystem : public aunteater::System
{
public:
    MovementSystem(aunteater::Engine & aEngine) :
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
    aunteater::FamilyHelp<Movable> mMovables;
};


using Positioned = aunteater::Archetype<Position>;

class SumSystem : public aunteater::System
{
public:
    SumSystem(aunteater::Engine & aEngine) :
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
    aunteater::FamilyHelp<Positioned> mPositioneds;
    Floating mSum{0.};
};


} // namespace ebench
} // namespace ad
