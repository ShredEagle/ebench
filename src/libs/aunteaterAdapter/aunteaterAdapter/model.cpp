#include "model.h"
#include <math/Vector.h>

namespace ad {
namespace ebench {
AunteaterWorld::AunteaterWorld() :
    mSystemManager{mEntityManager}
{}

void AunteaterWorld::addEntityWithSimple()
{

    mEntityManager.addEntity(aunteater::Entity{}.add<Simple>());
}

aunteater::weak_entity AunteaterWorld::addEntity()
{
    return mEntityManager.addEntity(aunteater::Entity{});
}

void AunteaterWorld::remove(aunteater::weak_entity aEntity)
{
    mEntityManager.markToRemove(aEntity);
}

void AunteaterWorld::commit()
{
    mSystemManager.update(mTimer);
}

void AunteaterWorld::setup()
{}

void AunteaterWorld::prepareWorldForIteration()
{
    mSystemManager.add<SimpleSystem>();
}

void AunteaterWorld::prepareWorldForNestedIteration()
{
    mSystemManager.add<NestedSystem>();
}

void AunteaterWorld::prepareWorldForDiffIteration()
{
    mSystemManager.add<NestedTwoSystem>();
}

void AunteaterWorld::simpleIteration()
{
    mSystemManager.update(mTimer);
}

void AunteaterWorld::nestedIteration()
{
    mSystemManager.update(mTimer);
}

void AunteaterWorld::nestedDifferentIteration()
{
    mSystemManager.update(mTimer);
}
}
}
