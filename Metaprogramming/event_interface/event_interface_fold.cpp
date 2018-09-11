#include <iostream>
#include <type_traits>
#include <utility>

#include "isdetected.h"
#include "typelist.h"


template<typename Handler, typename Event>
using handler_expression = decltype( Handler::handle(std::declval<const Event&>()) );

template<typename Handler, typename Event>
constexpr inline auto has_handler_v = is_detected_v<handler_expression, Handler, Event>;


template<typename Listeners>
class Dispatcher
{
public:
    template<typename Event>
    static constexpr void post(Event&& evt) noexcept
    { 
        // A) if Dispatcher was a variadic template we could just...
        // ( ... , post_impl<Listeners>(std::forward<Event>(evt)) );

        // but we want to use a list - possibly to define it once and use it for multiple
        // control structures. We have some options

        // B) Use a struct with specialization and an overloaded operator()
        post_impl<Listeners>{}(std::forward<Event>(evt));

        // C) use template function overloading
        // post_impl(std::forward<Event>(evt), Listeners{});
    }
private:
    // A)
    // template<typename Handler, typename Event>
    // static constexpr void post_impl(Event&& evt) noexcept
    // {
    //     if constexpr (has_handler_v<Handler, Event>) {
    //         Handler::handle(std::forward<Event>(evt));
    //     }
    // }
    
    // B)
    template<typename List, typename... Ts> struct post_impl;
    template<template<class...> class C, typename... Ts>
    struct post_impl<C<Ts...>> {
        template<typename Event>
        constexpr void operator()(Event&& evt) noexcept
        {
            ( ... , operator()<Event, Ts>(std::forward<Event>(evt)) );
        }

        template<typename Event, typename Handler>
        constexpr void operator()(Event&& evt) noexcept
        {
            if constexpr (has_handler_v<Handler, Event>) {
                Handler::handle(std::forward<Event>(evt));
            }
        }
    };

    // C)
    // template<template<class...> class List, typename Event, typename... Ts>
    // static constexpr void post_impl(Event&& evt, List<Ts...>) noexcept
    // {
    //     ( ... , post_impl<Event, Ts>(std::forward<Event>(evt)) );
    // }
    // template<typename Event, typename Handler>
    // static constexpr void post_impl(Event&& evt) noexcept
    // {
    //     if constexpr (has_handler_v<Handler, Event>) {
    //         Handler::handle(std::forward<Event>(evt));
    //     }
    // }
};


// events
struct JustBeforeReturn { };
struct InTheBeginning { };
struct ReadingComicBooks { std::string title; };

struct CoutShouter
{
    static void handle(const JustBeforeReturn& /* evt */)
    {
        std::cout << "Goodbye!\n";
    }

    static void handle(const InTheBeginning& /* evt */)
    {
        std::cout << "Hello!\n";
    }

    static void handle(const ReadingComicBooks& /* evt */)
    {
        std::cout << "Comics!\n";
    }
};

struct QuietGuy { };

struct ComicBookNerd
{
    static void handle(const ReadingComicBooks& evt)
    {
        std::cout << "I love " << evt.title << "!\n";
    }
};


int main()
{
    using listeners = list<CoutShouter, QuietGuy, ComicBookNerd>;

    using dispatcher = Dispatcher<listeners>;
    // using dispatcher = Dispatcher<CoutShouter, QuietGuy, ComicBookNerd>;

    dispatcher::post(InTheBeginning{});

    ReadingComicBooks spiderman{"Spiderman"};
    dispatcher::post(spiderman);

    dispatcher::post(JustBeforeReturn{});
    std::cout << "done." << std::endl;
    return 0;
}
