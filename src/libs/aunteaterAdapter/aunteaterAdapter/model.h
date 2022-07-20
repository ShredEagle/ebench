#pragma once


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
struct VariableStruct32
{
    u_int8_t a;
    u_int8_t b;
    u_int8_t c;
    u_int8_t d;
};

struct VariableStruct64
{
    u_int16_t a;
    u_int16_t b;
    u_int16_t c;
    u_int16_t d;
};

struct VariableStruct128
{
    u_int32_t a;
    u_int32_t b;
    u_int32_t c;
    u_int32_t d;
};

struct VariableStruct256
{
    u_int64_t a;
    u_int64_t b;
    u_int64_t c;
    u_int64_t d;
};

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
struct TemplatedComponent : public aunteater::Component<TemplatedComponent<N_ran>>
{
    int u = N_ran;
};

template<typename T>
concept HasMemberX = requires(T a)
{
    {a.a + a.b + a.c + a.d} -> std::convertible_to<long long>;
};


template<HasMemberX T_struct>
struct VaryingSizeComponent : public aunteater::Component<VaryingSizeComponent<T_struct>>
{
    T_struct data;
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
using Displaceable = aunteater::Archetype<Displacement>;

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

class NestedSystem : public aunteater::System<>
{
public:
    NestedSystem(aunteater::EntityManager & aEngine) :
        mMovables{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [position] : mMovables)
        {
            for (auto & [positionTwo] : mMovables)
            {
                positionTwo.x += aTimer.delta() * position.x;
                positionTwo.y += aTimer.delta() * position.y;
            }
        }
    }

private:
    aunteater::FamilyHelp<Movable> mMovables;
};

class NestedTwoSystem : public aunteater::System<>
{
public:
    NestedTwoSystem(aunteater::EntityManager & aEngine) :
        mMovables{aEngine},
        mDisplacement{aEngine}
    {}

    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [position] : mMovables)
        {
            for (auto & [displacement] : mDisplacement)
            {
                position.x += aTimer.delta() * displacement.x;
                position.y += aTimer.delta() * displacement.y;
            }
        }
    }

private:
    aunteater::FamilyHelp<Movable> mMovables;
    aunteater::FamilyHelp<Displaceable> mDisplacement;
};

template<typename T>
class VaryingSizeSystem : public aunteater::System<>
{
public:
    VaryingSizeSystem(aunteater::EntityManager & aEngine) :
        mVariables{aEngine}
    {}
    void update(const aunteater::Timer aTimer) override
    {
        for (auto & [variableStruct] : mVariables)
        {
            variableStruct.data.a++;
            variableStruct.data.b++;
            variableStruct.data.c++;
            variableStruct.data.d++;
        }
    }

private:
    aunteater::FamilyHelp<aunteater::Archetype<VaryingSizeComponent<T>>> mVariables;
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


struct AunteaterWorld
{
    using MovementSystem = MovementSystem;
    using NestedSystem = NestedSystem;
    using NestedTwoSystem = NestedTwoSystem;
    using Entity = aunteater::weak_entity;
    template<int T>
    using RandomAccessSystem = RandomAccessSystem<T>;
    template<typename T>
    using VaryingSizeSystem = VaryingSizeSystem<T>;
    using VariableStruct32 = VariableStruct32;
    using VariableStruct64 = VariableStruct64;
    using VariableStruct128 = VariableStruct128;
    using VariableStruct256 = VariableStruct256;

    AunteaterWorld();

    template<typename T_system>
    void addSystem()
    {
        mSystemManager.add<T_system>();
    }

    void addEntityWithPosition();
    void addEntityWithDisplacement();

    template<int N_maxComponent>
    Entity addEntityWithMultipleComponent()
    {
        aunteater::Entity entity;
        TemplatedComponentHelper<N_maxComponent>::add(entity);
        return mEntityManager.addEntity(entity);
    }

    template<int N_maxComponent>
    void removeMultipleComponent(Entity aEntity)
    {
        TemplatedComponentHelper<N_maxComponent>::remove(aEntity, mEntityManager);
    }

    Entity addEntity();
    void addComponent(Entity aEntity);
    template<typename T_struct>
    void addComponentTemplate(Entity aEntity)
    {
        aEntity->add<VaryingSizeComponent<T_struct>>();
    };
    void remove(Entity aEntity);
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
