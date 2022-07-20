
#include <aunteaterAdapter/model.h>

#include <ebench/bench.cpp>
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
