#pragma once
#include <iostream>
#include <chrono>
#include <math/Vector.h>

#include <benchmark/benchmark.h>


namespace ad {
namespace ebench {

struct BenchFeature
{
    bool addRemoveEntity = false;
    bool addRemoveComponent = false;
    bool iteration = false;
    bool entityAdditionDependentOnSystem = false;
};

template<typename T_ecs>
void BM_AddEntity(benchmark::State & aState)
{

    T_ecs world;
    for (auto _ : aState)
    {
        world.addEntity();
    }

    aState.SetItemsProcessed(aState.iterations());
}

template<typename T_ecs>
void BM_AddEntityWithSystem(benchmark::State & aState)
{
    T_ecs world;
    world.template addSystem<typename T_ecs::SimpleSystem>();

    for (auto _ : aState)
    {
        world.addEntityWithSimple();
    }

    aState.SetItemsProcessed(aState.iterations());
}

template<typename T_ecs, int N_toRemove>
void BM_RemoveEntity(benchmark::State & aState)
{
    T_ecs world;
    for (auto _ : aState)
    {
        world.setup();
        std::vector<typename T_ecs::Entity> entities;
        for (int i = 0; i < N_toRemove; i++)
        {
            entities.push_back(world.addEntity());
        }

        auto start = std::chrono::high_resolution_clock::now();

        for (auto entityToRemove : entities)
        {
            world.remove(entityToRemove);
        }

        world.commit();

        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);

        aState.SetIterationTime(elapsed_seconds.count());
    }
    aState.SetItemsProcessed(aState.iterations() * N_toRemove);
}

template<typename T_ecs, int N_toRemove>
void BM_AddAndRemoveEntity(benchmark::State & aState)
{
    T_ecs world;
    for (auto _ : aState)
    {
        world.setup();

        std::vector<typename T_ecs::Entity> entities;
        for (int i = 0; i < N_toRemove; i++)
        {
            entities.push_back(world.addEntity());
        }

        for (auto entityToRemove : entities)
        {
            world.remove(entityToRemove);
        }

        world.commit();

    }
    aState.SetItemsProcessed(aState.iterations() * N_toRemove);
}

template<typename T_ecs>
void BM_SimpleIteration(benchmark::State & aState)
{
    T_ecs world;
    world.prepareWorldForIteration();

    for (int i = 0; i < aState.range(0); i++)
    {

        typename T_ecs::Entity entity = world.addEntity();
        world.template addComponent<typename T_ecs::Simple>(entity);
    }

    world.commit();


    for (auto _ : aState)
    {
        world.simpleIteration();
    }

    aState.SetItemsProcessed(aState.range(0) * aState.iterations());
}

template<typename T_ecs, int N_arraySize>
void BM_SimpleIterationOnVaryingSize(benchmark::State & aState)
{
    T_ecs world;
    world.template prepareWorldForVaryingSizeIteration<N_arraySize>();

    for (int i = 0; i < 10000; i++)
    {
        auto entity = world.addEntity();
        world.template addComponentTemplate<N_arraySize>(entity);
    }

    world.commit();

    for (auto _ : aState)
    {
        world.template varyingSizeIteration<N_arraySize>();
    }

    aState.SetItemsProcessed(10000 * aState.iterations());
}

template<typename T_ecs>
void BM_NestedIterationOnSameArchetype(benchmark::State & aState)
{
    T_ecs world;
    world.prepareWorldForNestedIteration();

    for (int i = 0; i < aState.range(0); i++)
    {
        typename T_ecs::Entity entity = world.addEntity();
        world.template addComponent<typename T_ecs::Simple>(entity);
    }

    world.commit();


    for (auto _ : aState)
    {
        world.nestedIteration();
    }

    world.commit();

    aState.SetItemsProcessed(aState.range(0) * aState.range(0) * aState.iterations());
}

template<typename T_ecs>
void BM_NestedIterationOnTwoArchetype(benchmark::State & aState)
{
    T_ecs world;
    world.prepareWorldForDiffIteration();

    for (int i = 0; i < aState.range(0); i++)
    {
        typename T_ecs::Entity entity = world.addEntity();
        world.template addComponent<typename T_ecs::Simple>(entity);
    }

    for (int i = 0; i < aState.range(1); i++)
    {
        typename T_ecs::Entity entity = world.addEntity();
        world.template addComponent<typename T_ecs::SimpleB>(entity);
    }

    world.commit();


    for (auto _ : aState)
    {
        world.nestedDifferentIteration();
    }

    world.commit();

    aState.SetItemsProcessed(aState.range(0) * aState.iterations() * aState.range(1));
}

template<typename T_ecs>
void BM_AddComponent(benchmark::State & aState)
{
    T_ecs world;

    auto entity = world.addEntity();

    for (auto _ : aState)
    {
        world.setup();
        world.template addComponent<typename T_ecs::Simple>(entity);
        world.commit();
    }

    aState.SetItemsProcessed(aState.iterations());

}

template<typename T_ecs, int N>
void BM_AddAndRemoveComponent(benchmark::State & aState)
{
    T_ecs world;

    for (auto _ : aState)
    {
        world.setup();
        typename T_ecs::Entity entity = world.template addEntityWithMultipleComponent<N>();
        world.template removeMultipleComponent<N>(entity);
        world.commit();
    }

    aState.SetItemsProcessed(aState.iterations() * N);
}

template<typename T_ecs, int N>
void BM_RemoveComponent(benchmark::State & aState)
{
    T_ecs world;

    for (auto _ : aState)
    {
        world.setup();
        typename T_ecs::Entity entity = world.template addEntityWithMultipleComponent<N>();

        auto start = std::chrono::high_resolution_clock::now();

        world.template removeMultipleComponent<N>(entity);
        world.commit();

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
        aState.SetIterationTime(elapsed_seconds.count());
    }

    aState.SetItemsProcessed(aState.iterations() * N);
}

template<typename T_ecs, int N>
void BM_RandomDynamicAccessComponent(benchmark::State & aState)
{
    T_ecs world;
    world.template addSystem<typename T_ecs::template RandomAccessSystem<N>>();
    world.template addEntityWithMultipleComponent<N>();

    for (auto _ : aState)
    {
        world.commit();
    }
}

template<typename T>
int allbench()
{
    if constexpr (T::gFeatures.addRemoveEntity)
    {
        BENCHMARK_TEMPLATE(BM_AddEntity, T);
        BENCHMARK_TEMPLATE(BM_RemoveEntity, T, 1)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveEntity, T, 16)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveEntity, T, 32)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveEntity, T, 64)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveEntity, T, 128)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_AddAndRemoveEntity, T, 1);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveEntity, T, 16);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveEntity, T, 32);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveEntity, T, 64);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveEntity, T, 128);
    }
    if constexpr (T::gFeatures.entityAdditionDependentOnSystem)
    {
        BENCHMARK_TEMPLATE(BM_AddEntityWithSystem, T);
    }
    if constexpr (T::gFeatures.iteration)
    {
        BENCHMARK_TEMPLATE(BM_SimpleIteration, T)->Range(2 << 9, 2 << 16);
        BENCHMARK_TEMPLATE(BM_SimpleIteration, T)->Arg(1000000);
        BENCHMARK_TEMPLATE(BM_NestedIterationOnSameArchetype, T)->Range(2 << 4, 2 << 7);
        BENCHMARK_TEMPLATE(BM_NestedIterationOnTwoArchetype, T)->Ranges({{2 << 4, 2 << 7}, {2, 2 << 3}});
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingSize, T, 1);
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingSize, T, 2);
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingSize, T, 4);
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingSize, T, 8);
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingSize, T, 16);
    }
    if constexpr (T::gFeatures.addRemoveComponent)
    {
        BENCHMARK_TEMPLATE(BM_AddComponent, T);
        BENCHMARK_TEMPLATE(BM_RemoveComponent, T, 1)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveComponent, T, 16)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveComponent, T, 32)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveComponent, T, 64)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_RemoveComponent, T, 128)->UseManualTime();
        BENCHMARK_TEMPLATE(BM_AddAndRemoveComponent, T, 1);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveComponent, T, 16);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveComponent, T, 32);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveComponent, T, 64);
        BENCHMARK_TEMPLATE(BM_AddAndRemoveComponent, T, 128);
    }
    /* BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 4); */
    /* BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 16); */
    /* BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 64); */
    /* BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 256); */
    return 0;
}
}
}
