#pragma once

#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/Query.h>

namespace ad {
namespace ebench {
struct FlecsBenchState
{
    FlecsBenchState(ent::EntityManager && mWorld):
        mWorld{std::move(mWorld)},
        mPhase{std::make_unique<ent::Phase>()}
    {
    };

    ent::EntityManager mWorld;
    std::unique_ptr<ent::Phase> mPhase;
};

struct Simple
{
    int a = 0;
};

struct SimpleB
{
    int a = 0;
};

template<int N_arraySize>
struct VaryingSizeComponent
{
    std::array<int, N_arraySize> data;
};

template<int N_ran>
struct TemplatedComponent
{
    int u = N_ran;
};

template<int N>
struct TemplatedComponentHelper
{
    static int & get(int i, ent::Handle<ent::Entity> & aHandle, FlecsBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        if (i == N)
        {
            return entity.get<TemplatedComponent<N>>().u;
        }
        else
        {
            return TemplatedComponentHelper<N-1>::get(i, entity, aState);
        }
    };

    static void add(ent::Handle<ent::Entity> & aHandle, FlecsBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.add<TemplatedComponent<N>>({});
        TemplatedComponentHelper<N-1>::add(aHandle, aState);
    };

    static void remove(ent::Handle<ent::Entity> & aHandle, FlecsBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.remove<TemplatedComponent<N>>();
        TemplatedComponentHelper<N-1>::remove(aHandle, aState);
    }
};

template<>
struct TemplatedComponentHelper<0>
{
    static int & get(int i, ent::Handle<ent::Entity> & aHandle, FlecsBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        return entity.get<TemplatedComponent<0>>().u;
    }

    static void add(ent::Handle<ent::Entity> & aHandle, FlecsBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.add<TemplatedComponent<0>>({});
    }

    static void remove(ent::Handle<ent::Entity> & aHandle, FlecsBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.remove<TemplatedComponent<0>>();
    }
};
}
}
