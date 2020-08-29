#include "aunteater_bench.h"

#include <chrono>
#include <iostream>
#include <vector>

#include <cstdlib>


using Timer = std::chrono::high_resolution_clock;


const std::vector<std::size_t> gEntityCounts {
    1000,
    10000,
    100000,
    1000000,
    //10000000,
};


template <class T_duration>
std::string humanTime(T_duration aDuration)
{
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(aDuration).count();
    if (microseconds > 1E6)
    {
        return std::to_string(double(microseconds) / 1E6) + "s";
    }
    else if (microseconds > 1E3)
    {
        return std::to_string(double(microseconds) / 1E3) + "ms";
    }
    else
    {
        return std::to_string(microseconds) + "µs";
    }
}

int main()
{
    Timer timer;

    for (const auto count : gEntityCounts)
    {
        aunteater::Engine engine;

        // Create entities
        auto timePoint = timer.now();
        ad::ebench::createEntities(engine, count);

        std::cout << "Create " << count << " entities: "
                  << humanTime(timer.now() - timePoint)
                  << "\n"
                  ;


        // Update step
        //ad::ebench::addSystems<ad::ebench::MovementSystem, ad::ebench::SumSystem>(engine);
        engine.addSystem<ad::ebench::MovementSystem>();
        auto sumSystem = engine.addSystem<ad::ebench::SumSystem>();

        timePoint = timer.now();
        ad::ebench::simulateStep(engine);

        std::cout << "Update 2 system with " << count << " entities: "
                  << humanTime(timer.now() - timePoint)
                  << " (Sum: " << sumSystem->getSum() << ")"
                  << "\n"
                  << std::endl
                  ;



    }
    return EXIT_SUCCESS;
}
