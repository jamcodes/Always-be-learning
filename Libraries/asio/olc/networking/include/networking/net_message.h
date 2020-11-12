#pragma once

#include "net_common.h"


namespace olc
{
namespace net
{

template<typename T>
struct message_header
{
    static_assert(std::is_enum_v<T>, "message_header can be parameterized only with enum-class types");
    T id{};
    std::uint32_t size{0};
};

template<typename T>
struct message
{
    message_header<T> header{};
    std::vector<std::uint8_t> body{};

    std::size_t size() const noexcept
    {
        // return sizeof(header) + body.size();
        return body.size();
    }

    friend std::ostream& operator<<(std::ostream& os, message<T> const& msg)
    {
        using MsgIdType = std::underlying_type_t<T>;
        return os << "ID:" << static_cast<MsgIdType>(msg.header.id) << " Size:" << msg.header.size;
    }

    template<typename DataType/* , typename = RequirePodType<DataType> */>
    friend message<T>& operator<<(message<T>& msg, DataType const& data)
    {
        static_assert(IsTrivialAndStandardLayout<DataType>, "message can operate only on PoD types");

        // cache the current buffer size - this will be the point we insert data at
        auto const cur_size{msg.body.size()};
        // resize the vector by the size of the data being pushed
        msg.body.resize(cur_size + sizeof(DataType));
        std::memcpy(msg.body.data() + cur_size, &data, sizeof(DataType));
        // std::copy(static_cast<std::uint8_t const*>(&data), static_cast<std::uint8_t const*>(++&data),
        //           std::next(std::begin(msg.body), static_cast<std::ptrdiff_t>(cur_size))
        //           );
        msg.header.size = static_cast<std::uint32_t>(msg.size());
        return msg;
    }

    template<typename DataType>
    friend message<T>& operator>>(message<T>& msg, DataType& data)
    {
        static_assert(IsTrivialAndStandardLayout<DataType>, "message can operate only on PoD types");

        // cache the location at the end of the vector where the pulled data starts
        const auto begin_idx{msg.body.size() - sizeof(DataType)};
        // physically copy the data from the vector into the user variable
        std::memcpy(&data, msg.body.data() + begin_idx, sizeof(DataType));
        return msg;
    }

    // template<typename DataType, typename = RequireNonPodType<DataType>>
    // friend message<T>& operator<<(message<T>& msg, DataType const& msg) = delete;
};


template<typename T>
struct connection;


template<typename T>
struct owned_message
{
    std::shared_ptr<connection<T>> remote{nullptr};
    message<T> msg;

    friend std::ostream& operator<<(std::ostream& os, owned_message const& msg)
    {
        return os << msg.msg;
    }
};


} // namespace net

} // namespace olc
