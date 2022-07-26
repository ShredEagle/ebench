
#include <entityAdapter/model.h>

#include <ebench/bench.h>
#include <benchmark/benchmark.h>
#include <string>

#include <entity/EntityManager.h>

static int yo = ad::ebench::allbench<ad::ebench::EntityWorld>();

BENCHMARK_MAIN();

/* struct Position */
/* { */
/*     float x; */
/*     float y; */
/* }; */

/* int main() */
/* { */
/*     ad::ent::EntityManager world; */
/*     ad::ent::Phase phase; */
/*     ad::ent::Handle<ad::ent::Entity> handle = world.addEntity(); */
/*     ad::ent::Entity entity = *handle.get(phase); */
/*     entity.add(Position{0.f, 0.f}); */
/* } */
