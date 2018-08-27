#include <iostream>
#include <cassert>
#include "sml.hpp"


namespace sml = boost::sml;

namespace
{

// events
struct release { };
struct ack { };
struct fin { };
struct timeout { };

// guards
const auto is_ack_valid = [](const ack&) { return true; };
const auto is_fin_valid = [](const fin&) { return true; };

// actions
const auto send_fin = []{std::cout << "FIN\n";};
const auto send_ack = []{std::cout << "ACK\n";};

// sm implementation - operator()() const, and returning make_transition_table are required
struct hello_world
{
    auto operator()() const
    {
        using namespace sml;
// asterisk * marks the initial state
// user-defined literals can be used to define the states inline...
        return make_transition_table(
            *"established"_s + event<release> / send_fin = "fin_wait_1"_s
            ,"fin_wait_1"_s  + event<ack> [is_ack_valid] = "fin_wait_2"_s
            ,"fin_wait_2"_s + event<fin> [is_fin_valid] / send_ack = "timed_wait"_s
            ,"timed_wait"_s + event<timeout> / send_ack = X
        );
    }
};

} // namespace


int main()
{
    using namespace sml;

    sm<hello_world> sm;
    static_assert(1 == sizeof(sm), "sizeof(sm) != 1B"); // small footprint
    assert(sm.is("established"_s));     // checking the current state

    sm.process_event(release{});
    assert(sm.is("fin_wait_1"_s));

    sm.process_event(ack{});
    assert(sm.is("fin_wait_2"_s));

    sm.process_event(fin{});
    assert(sm.is("timed_wait"_s));

    sm.process_event(timeout{});
    assert(sm.is(X));   // released
}
