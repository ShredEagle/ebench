#include <aunteaterAdapter/world.h>

#include <ebench/bench.h>
#include <benchmark/benchmark.h>
#include <string>

static int yo = ad::ebench::allbench<ad::ebench::AunteaterWorld>();

BENCHMARK_MAIN();
