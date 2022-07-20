#include "model.h"
#include <math/Vector.h>

namespace ad {
namespace ebench {
AunteaterWorld::AunteaterWorld() :
    mSystemManager{mEntityManager}
{}

void AunteaterWorld::addEntityWithPosition()
{

    mEntityManager.addEntity(aunteater::Entity{}.add<Position>(math::Position<2, float>::Zero()));
}

void AunteaterWorld::addEntityWithDisplacement()
{

    mEntityManager.addEntity(aunteater::Entity{}.add<Displacement>(0.1f, 0.2f));
}

aunteater::weak_entity AunteaterWorld::addEntity()
{
    return mEntityManager.addEntity(aunteater::Entity{});
}

void AunteaterWorld::addComponent(aunteater::LiveEntity * aEntity)
{
    aEntity->add<Position>(math::Position<2, float>::Zero());
}

void AunteaterWorld::remove(aunteater::weak_entity aEntity)
{
    mEntityManager.markToRemove(aEntity);
}

}
}
