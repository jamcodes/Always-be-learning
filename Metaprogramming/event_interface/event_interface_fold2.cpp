#include <iostream>
#include <type_traits>
#include <utility>

#include "isdetected.h"
#include "typelist.h"


template<typename Handler, typename Event>
using handler_expression = decltype( Handler::handle(std::declval<const Event&>()) );

template<typename Handler, typename Event>
constexpr inline auto has_handler_v = is_detected_v<handler_expression, Handler, Event>;


template<typename... Listeners>
class Dispatcher
{
public:
    template<template<class...> class List, typename... Ts>
    constexpr Dispatcher(List<Ts...>) noexcept { }

    template<typename Event>
    static constexpr void post(Event&& evt) noexcept
    { 
        // A) if Dispatcher was a variadic template we could just...
        ( ... , post_impl<Listeners>(std::forward<Event>(evt)) );
    }
private:
    // A) and we can - deduce the Listeners by passing an instance of the type list
    // to the constructor and providing a template deduction guide (or a factory function)
    template<typename Handler, typename Event>
    static constexpr void post_impl(Event&& evt) noexcept
    {
        if constexpr (has_handler_v<Handler, Event>) {
            Handler::handle(std::forward<Event>(evt));
            // maybe some error handling here?
            // Check error codes, catch (or don't?) exceptions etc
        }
    }
};

template<template<class...> class List, typename... Ts>
Dispatcher(List<Ts...>) -> Dispatcher<Ts...>;


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

// We have some options on how we can use this structure...

// We can use it statically, by declaring an alias for the Dispatcher using decltype...
    using dispatcher = decltype(Dispatcher(listeners{}));
    dispatcher::post(InTheBeginning{});
    ReadingComicBooks spiderman{"Spiderman"};
    dispatcher::post(spiderman);
    dispatcher::post(JustBeforeReturn{});

    std::cout << "static dispatch done...\n\n";

// Or we can just instantiate the Dispatcher (at compile time...) and dispatch through the object
    constexpr auto disp = Dispatcher(listeners{});
    disp.post(InTheBeginning{});
    auto batman = ReadingComicBooks{"Batman"};
    disp.post(batman);
    disp.post(JustBeforeReturn{});

    std::cout << "object dispatch done." << std::endl;
    return 0;
}
