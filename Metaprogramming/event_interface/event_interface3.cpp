#include <iostream>
#include <type_traits>
#include <utility>

#include "isdetected.h"


template<typename Handler, typename Event>
using handler_expression = decltype( Handler::handle(std::declval<const Event&>()) );

template<typename Handler, typename Event>
constexpr inline auto has_handler_v = is_detected_v<handler_expression, Handler, Event>;


template<typename... Listeners>
class Disp
{
public:
    template<typename Event>
    static constexpr void post(Event&& evt) noexcept
    {
        post_impl<Event, Listeners...>(std::forward<Event>(evt));
    }
private:
    template<typename Event>
    static constexpr void post_impl(Event&&) noexcept { }

    template<typename Event, typename H, typename... Hs>
    static constexpr void post_impl(Event&& evt) noexcept
    {
        if constexpr (has_handler_v<H, Event>) {
            H::handle(std::forward<Event>(evt));
        }
        post_impl<Event, Hs...>(std::forward<Event>(evt));
    }
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
    using disp = Disp<CoutShouter, QuietGuy, ComicBookNerd>;
    disp::post(InTheBeginning{});
    ReadingComicBooks spiderman{"Spiderman"};
    disp::post(spiderman);
    disp::post(JustBeforeReturn{});
    std::cout << "done." << std::endl;
    return 0;
}
