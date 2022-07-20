#include <iostream>
#include <chrono>
#include <math/Vector.h>

#include <benchmark/benchmark.h>

namespace ad {
namespace ebench {

    template<typename T_ecs>
    void BM_AddEntity(benchmark::State & aState)
    {
        T_ecs world;

        for (auto _ : aState)
        {
            world.addEntityWithPosition();
        }

        aState.SetItemsProcessed(aState.iterations());
    }

    template<typename T_ecs>
    void BM_AddEntityWithSystem(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::MovementSystem>();

        for (auto _ : aState)
        {
            world.addEntityWithPosition();
        }

        aState.SetItemsProcessed(aState.iterations());
    }

    template<typename T_ecs, int N_toRemove>
    void BM_RemoveEntity(benchmark::State & aState)
    {
        T_ecs world;
        for (auto _ : aState)
        {
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

            world.update();

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
            std::vector<typename T_ecs::Entity> entities;
            for (int i = 0; i < N_toRemove; i++)
            {
                entities.push_back(world.addEntity());
            }

            for (auto entityToRemove : entities)
            {
                world.remove(entityToRemove);
            }

            world.update();

        }
        aState.SetItemsProcessed(aState.iterations() * N_toRemove);
    }

    template<typename T_ecs>
    void BM_SimpleIteration(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::MovementSystem>();

        for (int i = 0; i < aState.range(0); i++)
        {

            world.addEntityWithPosition();
        }


        for (auto _ : aState)
        {
            world.update();
        }

        aState.SetItemsProcessed(aState.range(0) * aState.iterations());
    }

    template<typename T_ecs, typename T_struct>
    void BM_SimpleIterationOnVaryingStruct(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::template VaryingSizeSystem<T_struct>>();

        for (int i = 0; i < 10000; i++)
        {
            auto entity = world.addEntity();
            world.template addComponentTemplate<T_struct>(entity);
        }


        for (auto _ : aState)
        {
            world.update();
        }

        aState.SetItemsProcessed(10000 * aState.iterations());
    }

    template<typename T_ecs>
    void BM_NestedIterationOnSameArchetype(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::NestedSystem>();

        for (int i = 0; i < aState.range(0); i++)
        {

            world.addEntityWithPosition();
        }


        for (auto _ : aState)
        {
            world.update();
        }

        aState.SetItemsProcessed(aState.range(0) * aState.range(0) * aState.iterations());
    }

    template<typename T_ecs>
    void BM_NestedIterationOnTwoArchetype(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::NestedTwoSystem>();

        for (int i = 0; i < aState.range(0); i++)
        {

            world.addEntityWithPosition();
        }

        for (int i = 0; i < aState.range(1); i++)
        {

            world.addEntityWithDisplacement();
        }


        for (auto _ : aState)
        {
            world.update();
        }

        aState.SetItemsProcessed(aState.range(0) * aState.iterations() * aState.range(1));
    }

    template<typename T_ecs>
    void BM_AddComponent(benchmark::State & aState)
    {
        T_ecs world;

        auto entity = world.addEntity();

        for (auto _ : aState)
        {
            world.addComponent(entity);
        }

        aState.SetItemsProcessed(aState.iterations());

    }

    template<typename T_ecs, int N>
    void BM_AddAndRemoveComponent(benchmark::State & aState)
    {
        T_ecs world;

        for (auto _ : aState)
        {
            typename T_ecs::Entity entity = world.template addEntityWithMultipleComponent<N>();
            world.template removeMultipleComponent<N>(entity);
            world.update();
        }

        aState.SetItemsProcessed(aState.iterations() * N);
    }

    template<typename T_ecs, int N>
    void BM_RemoveComponent(benchmark::State & aState)
    {
        T_ecs world;

        for (auto _ : aState)
        {
            typename T_ecs::Entity entity = world.template addEntityWithMultipleComponent<N>();

            auto start = std::chrono::high_resolution_clock::now();

            world.template removeMultipleComponent<N>(entity);
            world.update();

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
            world.update();
        }
    }

    template<typename T>
    int allbench()
    {
        BENCHMARK_TEMPLATE(BM_AddEntity, T);
        BENCHMARK_TEMPLATE(BM_AddEntityWithSystem, T);
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
        BENCHMARK_TEMPLATE(BM_SimpleIteration, T)->Range(2 << 9, 2 << 16);
        BENCHMARK_TEMPLATE(BM_NestedIterationOnSameArchetype, T)->Range(2 << 4, 2 << 7);
        BENCHMARK_TEMPLATE(BM_NestedIterationOnTwoArchetype, T)->Ranges({{2 << 4, 2 << 7}, {2, 2 << 3}});
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingStruct, T, typename T::VariableStruct32)->Name("BM_SimpleIterationOnVaryingStruct<T, 32>/10k");
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingStruct, T, typename T::VariableStruct64)->Name("BM_SimpleIterationOnVaryingStruct<T, 64>/10k");
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingStruct, T, typename T::VariableStruct128)->Name("BM_SimpleIterationOnVaryingStruct<T, 128>/10k");
        BENCHMARK_TEMPLATE(BM_SimpleIterationOnVaryingStruct, T, typename T::VariableStruct256)->Name("BM_SimpleIterationOnVaryingStruct<T, 256>/10k");
        BENCHMARK_TEMPLATE(BM_AddComponent, T);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 4);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 16);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 64);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 256);
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
        return 0;
    }
}
}
