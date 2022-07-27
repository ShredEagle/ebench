#include "world.h"

#include <math/Vector.h>

namespace ad {
namespace ebench {

void FlecsWorld::addEntityWithSimple()
{

}

FlecsWorld::Entity FlecsWorld::addEntity()
{
    return mState.mWorld.entity();
}

void FlecsWorld::remove(FlecsWorld::Entity & aHandle)
{
    aHandle.destruct();
}

void FlecsWorld::commit()
{
};

void FlecsWorld::setup()
{
};
void FlecsWorld::prepareWorldForIteration()
{
    fA = mState.mWorld.query<Simple>();
}
void FlecsWorld::prepareWorldForNestedIteration()
{
    fA = mState.mWorld.query<Simple>();
}
void FlecsWorld::prepareWorldForDiffIteration()
{
    fA = mState.mWorld.query<Simple>();
    fB = mState.mWorld.query<SimpleB>();
}
void FlecsWorld::simpleIteration()
{
    fA.each([](Simple & a)
            {
                a.a++;
            });
}
void FlecsWorld::nestedIteration()
{
    fA.each([&](Simple & a)
    {
        fA.each([&](Simple & b)
        {
            a.a++;
            b.a++;
        });
    });
}
void FlecsWorld::nestedDifferentIteration()
{
    fA.each([&](Simple & a)
    {
        fB.each([&](SimpleB & b)
        {
            a.a++;
            b.a++;
        });
    });
}

}
}
