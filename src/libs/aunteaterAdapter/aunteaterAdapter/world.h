#pragma once

#include "model.h"

#include <ebench/bench.h>

namespace ad {
namespace ebench {

struct AunteaterWorld
{
    constexpr static BenchFeature gFeatures{
        .addRemoveEntity = true,
        .addRemoveComponent = true,
        .iteration = true,
        .entityAdditionDependentOnSystem = true
    };

    using Simple = Simple;
    using SimpleB = SimpleB;
    using SimpleSystem = SimpleSystem;
    using Entity = aunteater::weak_entity;
    template<int T>
    using RandomAccessSystem = RandomAccessSystem<T>;
    template<int N_arraySize>
    using VaryingSizeSystem = VaryingSizeSystem<N_arraySize>;

    AunteaterWorld();

    Entity addEntity();
    void addEntityWithSimple();
    void remove(Entity aEntity);
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
        mSystemManager.add<T_system>();
    }
    template<typename T_comp>
    void addComponent(Entity aEntity)
    {
        aEntity->add<T_comp>();
    }
    template<int N_maxComponent>
    Entity addEntityWithMultipleComponent()
    {
        aunteater::Entity entity;
        TemplatedComponentHelper<N_maxComponent>::add(entity);
        return mEntityManager.addEntity(entity);
    }

    template<int N_maxComponent>
    void removeMultipleComponent(Entity aEntity)
    {
        TemplatedComponentHelper<N_maxComponent>::remove(aEntity, mEntityManager);
    }
    template<int N_arraySize>
    void addComponentTemplate(Entity aEntity)
    {
        aEntity->add<VaryingSizeComponent<N_arraySize>>();
    };
    template<int N_arraySize>
    void prepareWorldForVaryingSizeIteration()
    {
        mSystemManager.add<VaryingSizeSystem<N_arraySize>>();
    }
    template<int N_arraySize>
    void varyingSizeIteration()
    {
        mSystemManager.update(mTimer);
    }

    private:
    aunteater::SystemManager<> mSystemManager;
    aunteater::EntityManager mEntityManager;
    aunteater::Timer mTimer;
};

}
}
