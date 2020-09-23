#include "aunteater_bench.h"

#include <aunteater/FamilyPoly.h>
#include <aunteater/UpdateTiming.h>

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
std::string humanDuration(T_duration aDuration)
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


template <class T_family, class T_engine=aunteater::Engine<T_family>>
void bench(std::size_t aEntityCount)
{
        Timer timer;
        T_engine engine;

        // Create entities
        auto timePoint = timer.now();
        ad::ebench::createEntities(engine, aEntityCount);

        std::cout << "Create " << aEntityCount << " entities: "
                  << humanDuration(timer.now() - timePoint)
                  << "\n"
                  ;

        // Update step
        engine.template addSystem<ad::ebench::MovementSystem_FamilyHelp<T_family>>();
        auto sumSystem = engine.template addSystem<ad::ebench::SumSystem_FamilyHelp<T_family>>();

        aunteater::UpdateTiming timing;
        timePoint = timer.now();
        ad::ebench::simulateStep(engine, timing);

        std::cout << "Update 2 system with " << aEntityCount << " entities: "
                  << humanDuration(timer.now() - timePoint)
                  << " (Sum: " << sumSystem->getSum() << ")\n"
                  ;

        for (const auto & [systemName, duration] : timing.getTimings())
        {
            std::cout << "\t" << systemName << ": " << humanDuration(duration) << "\n"
                      ;
        }
        std::cout << std::endl;
}


void printSection(const std::string & aName)
{
    std::string::size_type lineLength = 2*3 + 2 + aName.size();
    // Does not compile, what the heck...
    //const std::string line{lineLength, 'a'};
    const std::string line(lineLength, '#');
    std::cout << line << '\n'
              << "### " << aName << " ###\n"
              << line << '\n'
              ;
}

int main()
{
    printSection("v 0.8.0");
    for (const auto count : gEntityCounts)
    {
        bench<ad::ebench::Family>(count);
    }

    printSection("vector");
    for (const auto count : gEntityCounts)
    {
        bench<aunteater::Family<aunteater::vector_entity>>(count);
    }

    printSection("Polymorphic family");
    for (const auto count : gEntityCounts)
    {
        bench<aunteater::FamilyPoly>(count);
    }
    return EXIT_SUCCESS;
}
