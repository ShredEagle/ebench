#pragma once

#include "model.h"

#include <ebench/bench.h>

namespace ad {
namespace ebench {

struct FlecsWorld
{
    constexpr static BenchFeature gFeatures{
        .addRemoveEntity = true,
        .addRemoveComponent = true,
        .iteration = true
    };

    using Simple = Simple;
    using SimpleB = SimpleB;
    using Entity = flecs::entity;

    FlecsWorld() = default;

    Entity addEntity();
    void addEntityWithSimple();
    void remove(Entity & aEntity);
    void commit();
    void setup();
    void prepareWorldForIteration();
    void prepareWorldForNestedIteration();
    void prepareWorldForDiffIteration();
    void simpleIteration();
    void nestedIteration();
    void nestedDifferentIteration();

    template<typename T_system>
    void addSystem()
    {
    }
    template<typename T_comp>
    void addComponent(Entity aEntity)
    {
        aEntity.add<T_comp>();
    }
    template<int N_maxComponent>
    Entity addEntityWithMultipleComponent()
    {
        auto entity = mState.mWorld.entity();
        TemplatedComponentHelper<N_maxComponent>::add(entity, mState);
        return entity;
    }

    template<int N_maxComponent>
    void removeMultipleComponent(Entity aEntity)
    {
        TemplatedComponentHelper<N_maxComponent>::remove(aEntity, mState);
    }
    template<int N_arraySize>
    void addComponentTemplate(Entity aEntity)
    {
        aEntity.add<VaryingSizeComponent<N_arraySize>>();
    };
    template<int N_arraySize>
    void prepareWorldForVaryingSizeIteration()
    {
    }
    template<int N_arraySize>
    void varyingSizeIteration()
    {
        auto query = mState.mWorld.query<VaryingSizeComponent<N_arraySize>>();
        query.each([](VaryingSizeComponent<N_arraySize> vss)
                {
                    for (auto & datum: vss.data)
                    {
                        datum++;
                    }
                });
    }

    private:
    FlecsBenchState mState;
    flecs::query<Simple> fA;
    flecs::query<SimpleB> fB;
};

}
}
