#pragma once

#include <ebench/bench.h>

#include <entity/Entity.h>
#include <entity/EntityManager.h>

namespace ad {
namespace ebench {
struct EntityBenchState
{
    EntityBenchState(ent::EntityManager && mWorld):
        mWorld{std::move(mWorld)},
        mPhase{}
    {
    };

    ent::EntityManager mWorld;
    ent::Phase mPhase;
};

struct Position
{
    float x;
    float y;
};

struct Displacement
{
    float dX;
    float dY;
};

template<int N_ran>
struct TemplatedComponent
{
    int u = N_ran;
};

/* template<int N> */
/* struct TemplatedComponentHelper */
/* { */
/*     static int & get(int i, ent::Handle<ent::Entity> & aHandle, EntityWorld & aState) */
/*     { */
/*         ent::Entity entity = *aHandle.get(aState.mPhase); */
/*         if (i == N) */
/*         { */
/*             return entity.get<TemplatedComponent<N>>().u; */
/*         } */
/*         else */
/*         { */
/*             return TemplatedComponentHelper<N-1>::get(i, entity, aState); */
/*         } */
/*     }; */

/*     static void add(ent::Handle<ent::Entity> & aHandle, EntityWorld & aState) */
/*     { */
/*         ent::Entity entity = *aHandle.get(aState.mPhase); */
/*         entity.add<TemplatedComponent<N>>({}); */
/*         TemplatedComponentHelper<N-1>::add(aHandle, aState); */
/*     }; */

/*     static void remove(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState) */
/*     { */
/*         ent::Entity entity = *aHandle.get(aState.mPhase); */
/*         entity.remove<TemplatedComponent<N>>(); */
/*         TemplatedComponentHelper<N-1>::remove(aHandle, aState.mPhase); */
/*     } */
/* }; */

/* template<> */
/* struct TemplatedComponentHelper<0> */
/* { */
/*     static int & get(int i, ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState) */
/*     { */
/*         ent::Entity entity = *aHandle.get(aState.mPhase); */
/*         return entity.get<TemplatedComponent<0>>().u; */
/*     } */

/*     static void add(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState) */
/*     { */
/*         ent::Entity entity = *aHandle.get(aState.mPhase); */
/*         entity.add<TemplatedComponent<0>>({}); */
/*     } */

/*     static void remove(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState) */
/*     { */
/*         ent::Entity entity = *aHandle.get(aState.mPhase); */
/*         entity.remove<TemplatedComponent<0>>(); */
/*     } */
/* }; */

struct EntityWorld
{
    using Entity = ent::Handle<ent::Entity>;
    constexpr static BenchFeature gFeatures{.addEntity = true};

    EntityWorld() /*:
        mState{ent::EntityManager{}}*/
    {};

    ent::Handle<ent::Entity> addEntity();
    void addEntityWithPosition();
    void addEntityWithDisplacement();
    void addComponent(EntityWorld::Entity & aEntity);
    void remove(EntityWorld::Entity & aEntity);

    /* template<int N_maxComponent> */
    /* Entity addEntityWithMultipleComponent() */
    /* { */
    /*     ent::Handle<ent::Entity> handle = mWorld.addEntity(); */
    /*     TemplatedComponentHelper<N_maxComponent>::add(handle, mState); */
    /*     return handle; */
    /* } */


    private:
        //EntityBenchState mState;
        ent::EntityManager mWorld;
        //ent::Phase mPhase;
};
}
}
