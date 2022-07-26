#pragma once

#include <ebench/bench.h>

#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/Query.h>

namespace ad {
namespace ebench {
struct EntityBenchState
{
    EntityBenchState(ent::EntityManager && mWorld):
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

template<int N>
struct TemplatedComponentHelper
{
    static int & get(int i, ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState)
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

    static void add(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.add<TemplatedComponent<N>>({});
        TemplatedComponentHelper<N-1>::add(aHandle, aState);
    };

    static void remove(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.remove<TemplatedComponent<N>>();
        TemplatedComponentHelper<N-1>::remove(aHandle, aState);
    }
};

template<>
struct TemplatedComponentHelper<0>
{
    static int & get(int i, ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        return entity.get<TemplatedComponent<0>>().u;
    }

    static void add(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.add<TemplatedComponent<0>>({});
    }

    static void remove(ent::Handle<ent::Entity> & aHandle, EntityBenchState & aState)
    {
        ent::Entity entity = *aHandle.get(*aState.mPhase);
        entity.remove<TemplatedComponent<0>>();
    }
};

class EntityWorld
{
public:
    constexpr static BenchFeature gFeatures{
        .addRemoveEntity = true,
        .addRemoveComponent = true,
        .iteration = true
    };

    using Entity = ent::Handle<ent::Entity>;
    using Simple = Simple;
    using SimpleB = SimpleB;

    EntityWorld() :
        mState{ent::EntityManager{}}
    {};

    Entity addEntity();
    void addEntityWithSimple();
    void remove(EntityWorld::Entity & aEntity);
    void commit();
    void setup();
    void prepareWorldForIteration();
    void prepareWorldForNestedIteration();
    void prepareWorldForDiffIteration();
    void simpleIteration();
    void nestedIteration();
    void nestedDifferentIteration();

    template<typename T_comp>
    void addComponent(Entity & aHandle)
    {
        ent::Entity entity = *aHandle.get(*mState.mPhase);
        entity.add(T_comp{});
    }
    template<int N_maxComponent>
    Entity addEntityWithMultipleComponent()
    {
        ent::Handle<ent::Entity> handle = mState.mWorld.addEntity();
        TemplatedComponentHelper<N_maxComponent>::add(handle, mState);
        return handle;
    }
    template<int N_maxComponent>
    void removeMultipleComponent(Entity aEntity)
    {
        TemplatedComponentHelper<N_maxComponent>::remove(aEntity, mState);
    }
    template<int N_arraySize>
    void addComponentTemplate(Entity & aHandle)
    {
        ent::Entity entity = *aHandle.get(*mState.mPhase);
        entity.add(VaryingSizeComponent<N_arraySize>{});
    }
    template<int N_arraySize>
    void prepareWorldForVaryingSizeIteration()
    {}
    template<int N_arraySize>
    void varyingSizeIteration()
    {
        ent::Query<VaryingSizeComponent<N_arraySize>> q{mState.mWorld};
        q.each([](VaryingSizeComponent<N_arraySize> & a)
        {
            for (auto & datum : a.data)
            {
                datum++;
            }
        });
    }

private:
    EntityBenchState mState;
    std::unique_ptr<ent::Query<Simple>> qA;
    std::unique_ptr<ent::Query<SimpleB>> qB;
};
}
}
