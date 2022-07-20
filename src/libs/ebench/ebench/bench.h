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

    template<typename T_ecs>
    void BM_UpdateSystem(benchmark::State & aState)
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
    void BM_RandomDynamicAccessComponent(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::template RandomAccessSystem<N>>();
        world.template addEntityForRandomAccess<N>();

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
        BENCHMARK_TEMPLATE(BM_UpdateSystem, T)->Range(2 << 9, 2 << 16);
        BENCHMARK_TEMPLATE(BM_AddComponent, T);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 4);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 16);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 64);
        BENCHMARK_TEMPLATE(BM_RandomDynamicAccessComponent, T, 256);
        return 0;
    }
}
}
