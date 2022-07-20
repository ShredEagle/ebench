#include "model.h"
#include <math/Vector.h>

namespace ad {
namespace ebench {
AunteaterWorld::AunteaterWorld() :
    mSystemManager{mEntityManager}
{}

void AunteaterWorld::addEntityWithPosition()
{

    mEntityManager.addEntity(aunteater::Entity{}.add<Position>());
}

aunteater::LiveEntity * AunteaterWorld::addEntity()
{
    return mEntityManager.addEntity(aunteater::Entity{});
}

void AunteaterWorld::addComponent(aunteater::LiveEntity * aEntity)
{
    aEntity->add<Position>(math::Position<2, float>::Zero());
}

}
}
