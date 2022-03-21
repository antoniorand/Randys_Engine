#pragma once
#include <iostream>
#include <cstdint>
#include <type_traits>

namespace generic_GameEngine{

    namespace MetaProgramming {


        template<typename T, T VAL>
        struct constant {static constexpr T value {VAL};};

        struct true_type  : constant<bool,true>{};
        struct false_type : constant<bool,false>{};

        template<typename T, typename U>
        struct is_same : false_type{};

        template <typename T>
        struct is_same<T,T> : true_type{};

        template <bool Cond, typename T, typename F>
        struct IFT : type_id <F>{};
        template <typename T, typename F>
        struct IFT<true,T,F> : type_id <T>{};
        template <bool Cond, typename T, typename F>
        using IFT_t =typename IFT<Cond,T,F>::type;

        template<typename T, typename U>
        constexpr bool is_same_v = is_same<T,U>::value;

        template<typename T, typename... Ts>
        struct position_type {
            static_assert(sizeof...(Ts) !=0, "Empty list!");
        };
        template<typename T, typename... Ts>
        constexpr std::size_t position_type_v 
            = position_type<T,Ts...>::value;
        template<typename T, typename... Ts>
        struct position_type<T,T,Ts...> : constant<std::size_t,0>{};
        template<typename T,typename U, typename... Ts>
        struct position_type<T,U,Ts...> : 
            constant<std::size_t,1 + position_type_v<T,Ts...>>{};

        void seetype(auto){
            std::cout << __PRETTY_FUNCTION__ << '\n';
        }


        template<typename T>
        struct type_id{
            using type = T;
        };
        template <std::size_t N, typename... Ts>
        struct nth_type {
            static_assert(sizeof...(Ts) != 0, "Error: Typelist empty");
        };
        template <std::size_t N, typename... Ts>
        using nth_type_t = typename nth_type<N,Ts...>::type;

        template <typename T, typename... Ts>
        struct nth_type<0, T, Ts...> : type_id<T>{
        };

        template <std::size_t N,typename T, typename... Ts>
        struct nth_type<N, T, Ts...> 
            : type_id<nth_type_t<N-1,Ts...>>{
        };

    }
    

    using namespace MetaProgramming;

    template <typename... Ts>
    struct Typelist{

        consteval static std::size_t size() noexcept{
            return sizeof...(Ts);
        }
        template<typename T>
        consteval static bool contains() noexcept {
            return(false || ... || is_same_v<T,Ts>);
        }

        template<typename T>
        consteval static std::size_t position() noexcept {
            static_assert(contains<T>());
            return position_type_v<T,Ts...>;
        }
    };

    template<typename TAGLIST>
    struct tags_traits{

        using mask_type = IFT_t<(TAGLIST::size() <= 8), uint8_t, uint16_t>;

        consteval static std::size_t size() noexcept{
            return TAGLIST::size();
        } 
        template<typename TAG>
        consteval static std::size_t id() noexcept {
            static_assert(TAGLIST::template contains<TAG>());
            return TAGLIST::template position<TAG>();
        }
        template<typename TAG>
        consteval static mask_type mask() noexcept {return (1 << id<TAG>());}
        
    };

    template<typename CMPS>
    struct component_traits : tags_traits<CMPS>{
    };

    template <typename CMPS, typename TAGS>
    struct GameEngine{

        using tags = tags_traits<TAGS>;
        using cmps = component_traits<CMPS>;
    };
}