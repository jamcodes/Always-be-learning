#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <charconv>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


namespace olc
{
template<typename T>
inline constexpr bool IsTrivialAndStandardLayout{std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>};
template<typename T>
using RequirePodType = std::enable_if_t<std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>>;
template<typename T>
using RequireNonPodType = std::enable_if_t<!std::is_trivially_copyable_v<T> || !std::is_standard_layout_v<T>>;
} // namespace olc

