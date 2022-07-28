#include "world.h"
#include <entity/Entity.h>
#include <math/Vector.h>

namespace ad {
namespace ebench {

void EntityWorld::addEntityWithSimple()
{

    ent::Handle<ent::Entity> handle = mState.mWorld.addEntity();
    ent::Entity entity = *handle.get(*mState.mPhase);
    entity.add(Simple{});
}

EntityWorld::Entity EntityWorld::addEntity()
{
    return mState.mWorld.addEntity();
}

void EntityWorld::remove(EntityWorld::Entity & aHandle)
{
    aHandle.get(*mState.mPhase)->erase();
}

void EntityWorld::commit()
{
    mState.mPhase = nullptr;
};

void EntityWorld::setup()
{
    mState.mPhase = std::make_unique<ent::Phase>();
};
void EntityWorld::prepareWorldForIteration()
{
    qA = std::make_unique<ent::Query<Simple>>(mState.mWorld);
}
void EntityWorld::prepareWorldForNestedIteration()
{
    prepareWorldForIteration();
}
void EntityWorld::prepareWorldForDiffIteration()
{
    qA = std::make_unique<ent::Query<Simple>>(mState.mWorld);
    qB = std::make_unique<ent::Query<SimpleB>>(mState.mWorld);
}
void EntityWorld::simpleIteration()
{
    qA->each([&](Simple & a)
    {
        a.a++;
    });
}
void EntityWorld::nestedIteration()
{
    qA->each([&](Simple & a)
    {
        qA->each([&](Simple & b)
        {
            a.a++;
            b.a++;
        });
    });
}
void EntityWorld::nestedDifferentIteration()
{
    qA->each([&](Simple & a)
    {
        qB->each([&](SimpleB & b)
        {
            a.a++;
            b.a++;
        });
    });
}

}
}
