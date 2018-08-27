#include <iostream>
#include <cassert>
#include <variant>
#include "sml.hpp"

namespace sml = boost::sml;

namespace
{

// events
struct connect {};
struct disconnect {};

class data {

  struct Disconnected {};
  struct Connected {
    int id{};
  };

 public:
  explicit data(const std::string& address) : address{address} {}

  inline auto operator()()
  {
    using namespace boost::sml;
    return make_transition_table(
      * state<Disconnected> + event<connect> / [this] {
        //   data_ = Connected{42};
            // id = 42;
            connected_.id = 42;
        } = state<Connected>
      , state<Connected> + event<disconnect> / [this] {
        //   std::cout << std::get<Connected>(data_).id << std::endl;
            // std::cout << id << std::endl;
            std::cout << connected_.id << std::endl;
        } = X
    );
  }

 private:
// Any data needs to be managed manually
  std::string address{};
// This includes the state instances themselves
// If any state instances have state (data) we need to instantiate and refer to the instances
// manually in guard / actions
//   std::variant<Disconnected, Connected> data_{};
//   int id{};
    Disconnected disconnected_{};
    Connected    connected_{};
};

}  // namespace

int main() {
  sml::sm<data> sm{data{"127.0.0.1"}};
  sm.process_event(connect{});
  sm.process_event(disconnect{});
  assert(sm.is(sml::X));
}