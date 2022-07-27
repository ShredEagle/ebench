#pragma once

#include <flecs.h>
#include <array>

namespace ad {
namespace ebench {
struct FlecsBenchState
{
    FlecsBenchState()
    {
    };

    flecs::world mWorld;
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
    static int & get(int i, flecs::entity & aEntity, FlecsBenchState & aState)
    {
        if (i == N)
        {
            return aEntity.get_mut<TemplatedComponent<N>>()->u;
        }
        else
        {
            return TemplatedComponentHelper<N-1>::get(i, aEntity, aState);
        }
    };

    static void add(flecs::entity & aEntity, FlecsBenchState & aState)
    {
        aEntity.add<TemplatedComponent<N>>({});
        TemplatedComponentHelper<N-1>::add(aEntity, aState);
    };

    static void remove(flecs::entity & aEntity, FlecsBenchState & aState)
    {
        aEntity.remove<TemplatedComponent<N>>();
        TemplatedComponentHelper<N-1>::remove(aEntity, aState);
    }
};

template<>
struct TemplatedComponentHelper<0>
{
    static int & get(int i, flecs::entity & aEntity, FlecsBenchState & aState)
    {
        return aEntity.get_mut<TemplatedComponent<0>>()->u;
    }

    static void add(flecs::entity & aEntity, FlecsBenchState & aState)
    {
        aEntity.add<TemplatedComponent<0>>({});
    }

    static void remove(flecs::entity & aEntity, FlecsBenchState & aState)
    {
        aEntity.remove<TemplatedComponent<0>>();
    }
};
}
}
