
#include "GameEngine/GameEngine.hpp"

struct Entity{
    uint8_t components_; 
};

struct CPhysics{ static constexpr uint8_t id{0};};
struct CRender{ static constexpr uint8_t id{1};};
struct CHealth{static constexpr uint8_t id{2};};

using ComponentList = generic_GameEngine::Typelist<CPhysics, CRender,  CHealth>;

struct TPlayer{static constexpr uint8_t id{0};};
struct TEnemy{static constexpr uint8_t id{1};};
struct TBullet{static constexpr uint8_t id{2};};

using TagList = generic_GameEngine::Typelist<TPlayer,TEnemy,TBullet>;



//using TN = nth_type<i, Ts...>::type;

int main(){ 

    using namespace generic_GameEngine::MetaProgramming;

    return 0;
}