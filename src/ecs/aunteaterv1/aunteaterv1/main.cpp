
#include <aunteaterAdapter/world.h>

#include <ebench/bench.h>
#include <benchmark/benchmark.h>
#include <string>


namespace ad
{
namespace ebench
{

static int yo = allbench<AunteaterWorld>();

BENCHMARK_MAIN();
}
}
