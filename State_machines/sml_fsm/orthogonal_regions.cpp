#include <iostream>
#include <cassert>
#include "sml.hpp"

namespace sml = boost::sml;


namespace
{

// events
struct evt1 {};
struct evt2 {};
struct evt3 {};

// states
class Idle1;
class State1;
class Idle2;
class State2;

struct orthogonal_regions
{
    auto operator()() const noexcept
    {
        using namespace sml;
        return make_transition_table(
           *state<Idle1> + event<evt1> = state<State1>
          , state<State1> + event<evt2> = X
          , state<State1> + on_entry<_> / []{std::cout << "State1 on entry\n";}
          , state<State1> + on_exit<_>  / []{std::cout << "State1 on exit\n";}

          ,*state<Idle2> + event<evt2> = state<State2>
          , state<State2> + event<evt3> = X
          , state<State2> + on_entry<_> / []{std::cout << "State2 on entry\n";}
          , state<State2> + on_exit<_>  / []{std::cout << "State2 on exit\n";}
        );
    }
};

} // namespace


int main()
{
    sml::sm<orthogonal_regions> sm;
    using namespace sml;

    sm.process_event(evt1{});
    assert(sm.is(state<State1>, state<Idle2>));

    sm.process_event(evt2{});
    assert(sm.is(X, state<State2>));

    sm.process_event(evt3{});
    assert(sm.is(X, X));
}
