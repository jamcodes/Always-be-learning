#include <iostream>
#include <type_traits>
#include <utility>


// What we're aiming for:
/* 
class JustBeforeReturn {
    // ...
}

class CoutShouter {
public:
    void handle(const JustBeforeReturn& evt) {
        std::cout << "Goodbye!\n";
    }
}

template <typename Listeners>
class Dispatcher {
public:
    template <typename Evt>
    static void post(const Evt&);
}

using listeners = type_list<CoutShouter>;
using dispatcher = Dispatcher<listeners>;

int main() {

    dispatcher::post(JustBeforeReturn{});
    return 0;
}
*/



// We'll need compile-time type data strcutures to be able to perform more complex operations

// type_list
template<typename... Ts>
struct type_list;

// base case - one element
template<typename T>
struct type_list<T>
{
    using head = T;
};

// more than one element
template<typename Head, typename... Tail>
struct type_list<Head, Tail...> : type_list<Head>
{
    using tail = type_list<Tail...>;
};


// void_t to help invoke SFINAE
template<typename...>
using void_t = void;

// Counting the elements of the list
template<int I>
using int_constant = std::integral_constant<int, I>;

// base case - one element
template<typename T, typename = void>
struct count : int_constant<1> { };

// specialization for type_list's with more than one element
template<typename T>
struct count<T, void_t<typename T::tail>> : int_constant<1 + count<typename T::tail>()> { };
                                        // operator() of std::integral_constant returns the value

// a different implementation of count - probably better due to fewer template instantiations
template<typename... Ts>
struct type_count;

template<typename... Ts>
struct type_count<type_list<Ts...>> : int_constant<sizeof...(Ts)> { };

template<typename T>
constexpr inline auto type_count_v = type_count<T>::value;

// helper using std::conditional - essentialy a ternary operator
template<typename T>
struct has_tail :   /*  predicate               if true          if false */
    std::conditional_t<type_count_v<T> == 1, std::false_type, std::true_type> { };

template<typename T>
constexpr inline auto has_tail_v = has_tail<T>::value;

// different has_tail implementation ?
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename...>
struct has_tail_alt;

template<typename T>
struct has_tail_alt<T> : std::false_type { };

template<typename T, typename... Ts>
struct has_tail_alt<T,Ts...> : std::true_type { };

template< template<class> class C, typename T>
struct has_tail_alt<C<T>> : std::false_type { };

template< template<class...> class C, typename T, typename... Ts>
struct has_tail_alt<C<T, Ts...>> : std::true_type { };
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// check if a class defines a required member function
template<typename Handler, typename Event, typename = void>
struct has_handler : std::false_type { };

template<typename Handler, typename Event>
struct has_handler<Handler, Event,
    decltype( Handler::handle(std::declval<const Event&>()) )> : std::true_type { };

template<typename Handler, typename Event>
constexpr inline auto has_handler_v = has_handler<Handler, Event>::value;

// implement dispatcher
template<typename Listeners>
class Dispatcher
{
    template<typename Event, typename List, bool HasTail, bool HasHandler>
    struct post_impl;

    template<typename Event, typename List>
    struct post_impl<Event, List, true, true>   // has tail, has handler
    {
        static void call(const Event& evt)
        {
            // std::cout << __PRETTY_FUNCTION__ << std::endl;
            List::head::handle(evt);

            using Tail = typename List::tail;

            constexpr auto tail_check = has_tail_v<Tail>;
            constexpr auto handler_check = has_handler_v<typename Tail::head, Event>;
            post_impl<Event, Tail, tail_check, handler_check>::call(evt);
        }
    };

    template<typename Event, typename List>
    struct post_impl<Event, List, true, false>  // has tail, no handler
    {
        static void call(const Event& evt)
        {
            // std::cout << __PRETTY_FUNCTION__ << std::endl;
            using Tail = typename List::tail;
            constexpr auto tail_check = has_tail_v<Tail>;
            constexpr auto handler_check = has_handler_v<typename Tail::head, Event>;
            post_impl<Event, Tail, tail_check, handler_check>::call(evt);
        }
    };

    template<typename Event, typename List>
    struct post_impl<Event, List, false, true>  // no tail, has handler
    {
        static void call(const Event& evt)
        {
            // std::cout << __PRETTY_FUNCTION__ << std::endl;
            List::head::handle(evt);
        }
    };

    template<typename Event, typename List>
    struct post_impl<Event, List, false, false>  // no tail, no handler
    {
        static void call(const Event& /* evt */) noexcept {}
    };

public:
    template<typename Event>
    static void post(const Event& evt)
    {
        constexpr auto tail_check = has_tail_v<Listeners>;
        constexpr auto handler_check = has_handler_v<typename Listeners::head, Event>;
        post_impl<Event, Listeners, tail_check, handler_check>::call(evt);
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
    using listeners = type_list<CoutShouter, QuietGuy, ComicBookNerd>;

    using dispatcher = Dispatcher<listeners>;

    dispatcher::post(InTheBeginning{});

    ReadingComicBooks spiderman{"Spiderman"};
    dispatcher::post(spiderman);

    dispatcher::post(JustBeforeReturn{});
    std::cout << "done." << std::endl;
    return 0;
}
