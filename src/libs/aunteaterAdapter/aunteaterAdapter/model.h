#pragma once


#include <ebench/bench.h>
#include <array>
#include <aunteater/Component.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>
#include <aunteater/SystemManager.h>
#include <experimental/random>
#include <math/Vector.h>
#include <sys/types.h>


namespace ad {
namespace ebench {


using Floating = double;

// IMPORTANT: NodeList is not equilvalent to aunteater::list_entity
// see https://stackoverflow.com/q/32723988/1027706
//template <class T_family>
//using NodeList = aunteater::list_entity<T_family>;

using Engine = aunteater::SystemManager<>;

struct Simple : public aunteater::Component<Simple>
{
    int a = 0;
};

struct SimpleB : public aunteater::Component<SimpleB>
{
    int a = 0;
};

struct Position : public aunteater::Component<Position>
{
    Position(math::Position<2, float> aPos): 
        x{aPos.x()},
        y{aPos.y()}
    {};
    Floating x = 0.f;
    Floating y = 0.f;
};

template<int N_ran>
struct TemplatedComponent : public aunteater::Component<TemplatedComponent<N_ran>>
{
    int u = N_ran;
};

template<int N_arraySize>
struct VaryingSizeComponent : public aunteater::Component<VaryingSizeComponent<N_arraySize>>
{
    std::array<int, N_arraySize> data;
};


struct Displacement : public aunteater::Component<Displacement>
{
    Displacement(float aX, float aY) :
        x{aX},
        y{aY}
    {};
    static constexpr float gXDisplacement = 1.1f;
    Floating x{gXDisplacement};
    Floating y{2.5f};
};


using Movable = aunteater::Archetype<Position>;
using Simplable = aunteater::Archetype<Simple>;
using SimplableB = aunteater::Archetype<SimpleB>;
using Displaceable = aunteater::Archetype<Displacement>;

// Note: Use the family as a type template parameter, instead of the collection as a template template parameter
// this is mostly because there is no good way to make an alias for a template template argument
// (the template and its alias are considered different)
//template <template <class> class TT_entityCollection>

class SimpleSystem : public aunteater::System<>
{
public:
    SimpleSystem(aunteater::EntityManager & aEngine) :
        mSimples{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [simple] : mSimples)
        {
            simple.a++;
        }
    }

private:
    aunteater::FamilyHelp<Simplable> mSimples;
};

class NestedSystem : public aunteater::System<>
{
public:
    NestedSystem(aunteater::EntityManager & aEngine) :
        mSimples{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [simpleA] : mSimples)
        {
            for (auto & [simpleB] : mSimples)
            {
                simpleA.a++;
                simpleB.a++;
            }
        }
    }

private:
    aunteater::FamilyHelp<Simplable> mSimples;
};

class NestedTwoSystem : public aunteater::System<>
{
public:
    NestedTwoSystem(aunteater::EntityManager & aEngine) :
        mSimples{aEngine},
        mSimplesB{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [simpleA] : mSimples)
        {
            for (auto & [simpleB] : mSimplesB)
            {
                simpleA.a++;
                simpleB.a++;
            }
        }
    }

private:
    aunteater::FamilyHelp<Simplable> mSimples;
    aunteater::FamilyHelp<SimplableB> mSimplesB;
};

template<int N_arraySize>
class VaryingSizeSystem : public aunteater::System<>
{
public:
    VaryingSizeSystem(aunteater::EntityManager & aEngine) :
        mVariables{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [variableComp] : mVariables)
        {
            for (int & datum : variableComp.data)
            {
                datum++;
            }
        }
    }

private:
    aunteater::FamilyHelp<aunteater::Archetype<VaryingSizeComponent<N_arraySize>>> mVariables;
};

template<int N>
struct TemplatedComponentHelper
{
    static int & get(int i, aunteater::LiveEntity & entity)
    {
        if (i == N)
        {
            return entity.get<TemplatedComponent<N>>().u;
        }
        else
        {
            return TemplatedComponentHelper<N-1>::get(i, entity);
        }
    };

    static void add(aunteater::Entity & entity)
    {
        entity.add<TemplatedComponent<N>>();
        TemplatedComponentHelper<N-1>::add(entity);
    };

    static void remove(aunteater::weak_entity & aEntity, aunteater::EntityManager & aEntityManager)
    {
        aEntityManager.markComponentToRemove<TemplatedComponent<N>>(aEntity);
        TemplatedComponentHelper<N-1>::remove(aEntity, aEntityManager);
    }
};

template<>
struct TemplatedComponentHelper<0>
{
    static int & get(int i, aunteater::LiveEntity & entity)
    {
        return entity.get<TemplatedComponent<0>>().u;
    }

    static void add(aunteater::Entity & entity)
    {
        entity.add<TemplatedComponent<0>>();
    }

    static void remove(aunteater::weak_entity & aEntity, aunteater::EntityManager & aEntityManager)
    {
        aEntityManager.markComponentToRemove<TemplatedComponent<0>>(aEntity);
    }
};

template<int N_maxComponent>
class RandomAccessSystem : public aunteater::System<>
{
public:
    RandomAccessSystem(aunteater::EntityManager & aEngine) :
        mMovables{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & entity : mMovables)
        {
            int random = std::experimental::randint(0, N_maxComponent);
            int & number = TemplatedComponentHelper<N_maxComponent>::get(
                    random, static_cast<aunteater::LiveEntity &>(entity));
            number++;
        }
    }

private:
    aunteater::FamilyHelp<aunteater::Archetype<TemplatedComponent<N_maxComponent>>> mMovables;
};
}
}
