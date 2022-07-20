#include "bench.h"
#include <aunteaterAdapter/model.h>

#include <benchmark/benchmark.h>
#include <string>


namespace ad
{
namespace ebench
{

/* void BM_UpdateSystem(benchmark::State & aState) */
/* { */
/*     aunteater::Timer timer; */

/*     aunteater::EntityManager entityManager; */
/*     aunteater::SystemManager<> systemManager{entityManager}; */
/*     systemManager.add<MovementSystem>(); */

/*     for (int i = 0; i < aState.range(0); i++) */
/*     { */
/*         entityManager.addEntity(aunteater::Entity{}.add<Position>(0., 0.).add<Displacement>()); */
/*     } */


/*     for (auto _ : aState) */
/*     { */
/*         systemManager.update(timer); */
/*     } */

/*     aState.SetItemsProcessed(aState.range(0) * aState.iterations()); */
/* } */

/* BENCHMARK(BM_UpdateSystem)->Range(2, 2 << 9); */
}
}
