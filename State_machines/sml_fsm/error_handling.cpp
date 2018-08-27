#include <iostream>
#include <cassert>
#include "sml.hpp"

namespace sml = boost::sml;


namespace {
struct some_event {};

struct error_handling {
  auto operator()() const {
    using namespace sml;
    return make_transition_table(
        *("idle"_s) + "event1"_e / [] { throw std::runtime_error{"error"}; }
      ,   "idle"_s  + "event2"_e / [] { throw 0; }

      , *("exceptions handling"_s) + exception<std::runtime_error> / [] { std::cout << "exception caught" << std::endl; }
      ,   "exceptions handling"_s  + exception<_> / [] { std::cout << "generic exception caught" << std::endl; } = X

      , *("unexpected events handling"_s) + unexpected_event<some_event> / [] { std::cout << "unexpected event 'some_event'" << std::endl; }
      ,   "unexpected events handling"_s  + unexpected_event<_> / [] { std::cout << "generic unexpected event" << std::endl; } = X
    );
  }
};
}  // namespace


int main() {
  using namespace sml;
  sm<error_handling> sm;

  sm.process_event("event1"_e());  // throws runtime_error
  assert(sm.is("idle"_s, "exceptions handling"_s, "unexpected events handling"_s));

  sm.process_event("event2"_e());  // throws 0
  assert(sm.is("idle"_s, X, "unexpected events handling"_s));

  sm.process_event(some_event{});  // unexpected event
  assert(sm.is("idle"_s, X, "unexpected events handling"_s));

  sm.process_event(int{});  // unexpected any event
  assert(sm.is("idle"_s, X, X));
}
