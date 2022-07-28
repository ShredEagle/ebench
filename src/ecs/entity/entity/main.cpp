#include <entityAdapter/world.h>

#include <ebench/bench.h>
#include <benchmark/benchmark.h>

static int yo = ad::ebench::allbench<ad::ebench::EntityWorld>();

BENCHMARK_MAIN();
