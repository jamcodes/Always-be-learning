#include <iostream>
#include <variant>
#include <type_traits>


template<typename... Ts>
struct overload : Ts... { using Ts::operator()...; };

template<typename... Ts>
overload(Ts...) -> overload<Ts...>;


struct Fluid { };
struct LightItem { };
struct HeavyItem { };
struct FragileItem { };

struct GlassBox { };
struct CardbordBox { };
struct ReinforcedBox { };
struct AmortizedBox { };
struct BrokenBox { };


int main()
{
    using item_variant = std::variant<Fluid, LightItem, HeavyItem, FragileItem>;

// std::visit can be used on multiple std::variant objects at once
// The overload pattern matching is then done on pairs of types (or however many variants are used)
    auto item_a = item_variant{std::in_place_type_t<Fluid>{}};
    auto item_b = item_variant{std::in_place_type_t<FragileItem>{}};
    std::visit(overload{
        [](const Fluid&, const Fluid&) { std::cout << "2x Fluid\n"; },
        [](const Fluid&, const LightItem&) { std::cout << "Fluid, LightItem\n"; },
        [](const Fluid&, const HeavyItem&) { std::cout << "Fluid, HeavyItem\n"; },
        [](const Fluid&, const FragileItem&) { std::cout << "Fluid, FragileItem\n"; },
        [](const auto&, const Fluid&) { std::cout << "Something and a Fluid\n"; },
        [](const auto&, const LightItem&) { std::cout << "Something and a LightItem\n"; },
        [](const auto&, const HeavyItem&) { std::cout << "Something and a HeavyItem\n"; },
        [](const auto&, const FragileItem&) { std::cout << "Someting and a FragileItem\n"; }
        },
        item_a, item_b);

// the variants don't have to be of the same type
    using box_variant = std::variant<GlassBox, CardbordBox, ReinforcedBox, AmortizedBox, BrokenBox>;
    auto item = item_variant{std::in_place_type_t<Fluid>{}};
    auto box = box_variant{std::in_place_type_t<CardbordBox>{}};
    auto newbox = std::visit(overload{
        [](const Fluid&, const GlassBox& b) -> box_variant
            { std::cout << "Fluid in a GlassBox\n"; return b; },
        [](const Fluid&, const auto&) -> box_variant
            { std::cout << "Fluid in a wrong container!\n";
              return box_variant{std::in_place_type_t<BrokenBox>{}}; },
        [](const LightItem&, const CardbordBox& b) -> box_variant
            { std::cout << "LightItem in a CardbordBox\n"; return b; },
        [](const LightItem&, auto&& b) -> box_variant
            { std::cout << "LightItem not in a CardbordBox, what a waste\n";
              return std::forward<decltype(b)>(b); },
        [](const HeavyItem&, const ReinforcedBox& b) -> box_variant
            { std::cout << "HeavyItem in a reinforced box\n"; return b; },
        [](const HeavyItem&, const auto&) -> box_variant
            { std::cout << "HeavyItem in a wrong type of box\n";
              return box_variant{std::in_place_type_t<BrokenBox>{}}; },
        [](const FragileItem&, const AmortizedBox& b) -> box_variant
            { std::cout << "FragileItem in an amortized box\n"; return b; },
        [](const FragileItem&, auto&& b) -> box_variant
            { std::cout << "FragileItem in a wrong type of box\n";
              // change FragileItem state to broken
              return std::forward<decltype(b)>(b); }
        },
        item, box);

    if (std::holds_alternative<BrokenBox>(newbox) ) {
        std::cout << "We now have a BrokenBox\n";
    }
}
