#include "model.h"
#include <entity/Entity.h>
#include <math/Vector.h>

namespace ad {
namespace ebench {

/* void EntityWorld::addEntityWithPosition() */
/* { */

/*     ent::Handle<ent::Entity> handle = mWorld.addEntity(); */
/*     ent::Entity entity = *handle.get(mPhase); */
/*     entity.add(Position{0.f, 0.f}); */
/* } */

/* void EntityWorld::addEntityWithDisplacement() */
/* { */

/*     ent::Handle<ent::Entity> handle = mWorld.addEntity(); */
/*     ent::Entity entity = *handle.get(mPhase); */
/*     entity.add(Displacement{0.2f, 0.3f}); */
/* } */

EntityWorld::Entity EntityWorld::addEntity()
{
    return mWorld.addEntity();
}

/* void EntityWorld::addComponent(EntityWorld::Entity & aHandle) */
/* { */

/*     ent::Entity entity = *aHandle.get(mPhase); */
/*     entity.add(Position{0.2f, 0.3f}); */
/* } */

/* void EntityWorld::remove(EntityWorld::Entity & aHandle) */
/* { */
/*     ent::Entity entity = *aHandle.get(mPhase); */
/*     entity.erase(); */
/* } */

}
}
