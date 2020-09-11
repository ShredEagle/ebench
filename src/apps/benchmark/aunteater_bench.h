#pragma once


#include <ebench/aunteater.h>


namespace ad {
namespace ebench {


aunteater::Entity makeEntity()
{
    return aunteater::Entity{}
            .add<Position>()
            .add<Displacement>();
}


template <class T_engine>
void createEntities(T_engine & aEngine, std::size_t aCount)
{
    for (std::size_t id = 0; id != aCount; ++id)
    {
        aEngine.addEntity(makeEntity());
    }
}


template <int N=0>
Engine & addSystems(Engine & aEngine)
{
    return aEngine;
}


template <class T_systemHead, class... VT_systemTail>
Engine & addSystems(Engine & aEngine)
{
    aEngine.addSystem<T_systemHead>();
    return addSystems<VT_systemTail...>(aEngine);
}


template <class T_engine>
void simulateStep(T_engine & aEngine)
{
    aunteater::Timer timer;
    timer.mark(1.0);
    aEngine.update(timer);
}


} // namespace ebench
} // namespace ad
