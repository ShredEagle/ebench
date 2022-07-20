#include <math/Vector.h>

#include <benchmark/benchmark.h>

namespace ad {
namespace ebench {
    template<typename T_ecs>
    void BM_AddEntity(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::MovementSystem>();

        for (auto _ : aState)
        {
            world.addEntityWithPosition();
        }

        aState.SetItemsProcessed(aState.iterations());
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
    void BM_AccessComponentInternal(benchmark::State & aState)
    {
        T_ecs world;
        world.template addSystem<typename T_ecs::template RandomAccessSystem<N>>();
        world.template addEntityForRandomAccess<N>();

        for (auto _ : aState)
        {
            world.update();
        }
    }

    template<typename T_ecs>
    void BM_AccessComponent(benchmark::State & aState)
    {
        switch (aState.range(0))
        {
            case 4:
                BM_AccessComponentInternal<T_ecs, 4>(aState);
                break;
            case 4 * 4:
                BM_AccessComponentInternal<T_ecs, 4 * 4>(aState);
                break;
            case 4 * 16:
                BM_AccessComponentInternal<T_ecs, 4 * 16>(aState);
                break;
            case 4 * 64:
                BM_AccessComponentInternal<T_ecs, 4 * 64>(aState);
                break;
        }
    }


    template<typename T>
    int allbench()
    {
        BENCHMARK_TEMPLATE(BM_AddEntity, T);
        BENCHMARK_TEMPLATE(BM_UpdateSystem, T)->Range(2 << 9, 2 << 16);
        BENCHMARK_TEMPLATE(BM_AddComponent, T);
        BENCHMARK_TEMPLATE(BM_AccessComponent, T)->RangeMultiplier(4)->Range(4, 256);
        return 0;
    }
}
}
