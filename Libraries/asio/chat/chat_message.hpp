#pragma once

#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <system_error>
#include <iostream>


class ChatMessage
{
public:
    static constexpr inline std::size_t header_length{4};
    static constexpr inline std::size_t max_body_length{512};

    ChatMessage() noexcept = default;

    char* data() noexcept { return data_.data(); }
    const char* data() const noexcept { return data_.data(); }

    std::size_t length() const noexcept { return header_length + body_length_; }

    char* body() noexcept { return std::next(data_.begin(), header_length); }
    const char* body() const noexcept { return std::next(data_.begin(), header_length); }

    auto body_length() const noexcept { return body_length_; }
    void body_length(std::size_t length) noexcept { body_length_ = length; }

    std::errc decode_header() noexcept
    {
        const std::from_chars_result fr{std::from_chars(data_.cbegin(),
                                                  std::next(data_.cbegin(), header_length),
                                                  body_length_)};
        if (fr.ec != std::errc{}) {
            body_length_ = 0;
        }

        return fr.ec;
    }

    bool encode_header()
    {
        std::array<char, header_length + 1> header;
        std::sprintf(header.data(), "%04d", static_cast<int>(body_length_));
        // const std::to_chars_result tr{std::to_chars(data_.begin(),
        //                                       std::next(data_.begin(), header_length + 1),
        //                                       body_length_)};
        std::copy(header.cbegin(), std::next(header.cbegin(), header_length), data_.begin());
        // if (tr.ec != std::errc{}) {
        //     return false;
        // }
        return true;
    }

private:
    std::array<char, header_length + max_body_length> data_{};
    std::size_t body_length_{0};
};
