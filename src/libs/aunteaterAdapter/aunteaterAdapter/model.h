#pragma once


#include <aunteater/Component.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>
#include <aunteater/SystemManager.h>
#include <experimental/random>
#include <math/Vector.h>


namespace ad {
namespace ebench {


using Floating = double;

// IMPORTANT: NodeList is not equilvalent to aunteater::list_entity
// see https://stackoverflow.com/q/32723988/1027706
//template <class T_family>
//using NodeList = aunteater::list_entity<T_family>;

using Engine = aunteater::SystemManager<>;

struct Position : public aunteater::Component<Position>
{
    Position(math::Position<2, float> aPos): 
        x{aPos.x()},
        y{aPos.y()}
    {};
    Floating x;
    Floating y;
};

template<int N_ran>
struct RandomAccessComponent : public aunteater::Component<RandomAccessComponent<N_ran>>
{
    int u = N_ran;
};


struct Displacement : public aunteater::Component<Displacement>
{
    static constexpr float gXDisplacement = 1.1f;
    Floating x{gXDisplacement};
    Floating y{2.5f};
};


using Movable = aunteater::Archetype<Position>;

// Note: Use the family as a type template parameter, instead of the collection as a template template parameter
// this is mostly because there is no good way to make an alias for a template template argument
// (the template and its alias are considered different)
//template <template <class> class TT_entityCollection>

class MovementSystem : public aunteater::System<>
{
public:
    MovementSystem(aunteater::EntityManager & aEngine) :
        mMovables{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [position] : mMovables)
        {
            position.x += aTimer.delta() * 1.1f;
            position.y += aTimer.delta() * 1.5f;
        }
    }

private:
    aunteater::FamilyHelp<Movable> mMovables;
};

template<int N>
struct RandomAccessor
{
    static int & get(int i, aunteater::LiveEntity & entity)
    {
        if (i == N)
        {
            return entity.get<RandomAccessComponent<N>>().u;
        }
        else
        {
            return RandomAccessor<N-1>::get(i, entity);
        }
    };
};

template<>
struct RandomAccessor<0>
{
    static int & get(int i, aunteater::LiveEntity & entity)
    {
        return entity.get<RandomAccessComponent<0>>().u;
    }
};

template<int N>
struct RandomAdd
{
    static void add(aunteater::Entity & entity)
    {
        entity.add<RandomAccessComponent<N>>();
        RandomAdd<N-1>::add(entity);
    };
};

template<>
struct RandomAdd<0>
{
    static void add(aunteater::Entity & entity)
    {
        entity.add<RandomAccessComponent<0>>();
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
            int & number = RandomAccessor<N_maxComponent>::get(
                    random, static_cast<aunteater::LiveEntity &>(entity));
            number++;
        }
    }

private:
    aunteater::FamilyHelp<aunteater::Archetype<RandomAccessComponent<N_maxComponent>>> mMovables;
};


struct AunteaterWorld
{
    using MovementSystem = MovementSystem;
    template<int T>
    using RandomAccessSystem = RandomAccessSystem<T>;

    AunteaterWorld();

    template<typename T_system>
    void addSystem()
    {
        mSystemManager.add<T_system>();
    }
    void addEntityWithPosition();

    template<int N_maxComponent>
    void addEntityForRandomAccess()
    {
        aunteater::Entity entity;
        RandomAdd<N_maxComponent>::add(entity);
        mEntityManager.addEntity(entity);
    }

    aunteater::LiveEntity * addEntity();
    void addComponent(aunteater::LiveEntity * aEntity);
    void update()
    {
        mSystemManager.update(mTimer);
    };

    aunteater::SystemManager<> mSystemManager;
    aunteater::EntityManager mEntityManager;
    aunteater::Timer mTimer;
};

/* using Positioned = aunteater::Archetype<Position>; */


/* template <class T_family> */
/* class SumSystem : public aunteater::System */
/* { */
/* public: */
/*     SumSystem(aunteater::Engine<T_family> & aEngine) : */
/*         mPositioneds{aEngine.template getFamily<Positioned>()} */
/*     {} */

/*     void update(const aunteater::Timer aTimer) override */
/*     { */
/*         for (auto & entity : mPositioneds) */
/*         { */
/*             auto & position = entity->template get<Position>(); */

/*             mSum += position.x; */
/*         } */
/*     } */

/*     Floating getSum() const */
/*     { */
/*         return mSum; */
/*     } */

/* private: */
/*     T_family & mPositioneds; */
/*     Floating mSum{0.}; */
/* }; */


/* template <class T_family> */
/* class SumSystem_FamilyHelp : public aunteater::System */
/* { */
/* public: */
/*     SumSystem_FamilyHelp(aunteater::Engine<T_family> & aEngine) : */
/*         mPositioneds{aEngine} */
/*     {} */

/*     void update(const aunteater::Timer aTimer) override */
/*     { */
/*         for (auto & [position] : mPositioneds) */
/*         { */
/*             mSum += position.x; */
/*         } */
/*     } */

/*     Floating getSum() const */
/*     { */
/*         return mSum; */
/*     } */

/* private: */
/*     aunteater::FamilyHelp<T_family, Positioned> mPositioneds; */
/*     Floating mSum{0.}; */
/* }; */


} // namespace ebench
} // namespace ad
