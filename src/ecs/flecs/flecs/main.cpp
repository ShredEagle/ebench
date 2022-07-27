#include <flecsAdapter/world.h>

#include <ebench/bench.h>
#include <benchmark/benchmark.h>

static int yo = ad::ebench::allbench<ad::ebench::FlecsWorld>();

BENCHMARK_MAIN();
