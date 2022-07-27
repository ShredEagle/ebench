#pragma once

#include "model.h"
#include "ebench/bench.h"

namespace ad {
namespace ebench {
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
    FlecsBenchState mState;
    std::unique_ptr<ent::Query<Simple>> qA;
    std::unique_ptr<ent::Query<SimpleB>> qB;
};
}
}
