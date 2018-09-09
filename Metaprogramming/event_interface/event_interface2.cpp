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
        post_impl<Listeners>(std::forward<Event>(evt));
    }
private:
    template<typename List, typename Event>
    static constexpr void post_impl(Event&& evt) noexcept
    {
        using head = front<List>;
        if constexpr (has_handler_v<head, Event>) {
            head::handle(evt);
        }
        if constexpr (has_tail_v<List>) {
            post_impl<pop_front<List>>(std::forward<Event>(evt));
        }
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
    using listeners = list<CoutShouter, QuietGuy, ComicBookNerd>;

    using dispatcher = Dispatcher<listeners>;

    dispatcher::post(InTheBeginning{});

    ReadingComicBooks spiderman{"Spiderman"};
    dispatcher::post(spiderman);

    dispatcher::post(JustBeforeReturn{});
    std::cout << "done." << std::endl;
    return 0;
}
