#pragma once

#include <vector>
#include <chrono>
#include <utility>

#include "active.h"


template<typename T>
class Backgrounder {
    jam::Active active_;
    std::vector<T>& received_;

    struct Data {
        Data(T v) : value{std::move(v)} { }
        T value;
    };

    void bgStoreData(Data msg_) {
        received_.push_back(std::move(msg_.value));
        std::this_thread::sleep_for(std::chrono::microseconds{11});
    }

public:
    // using typename jam::Active::Callback;
    using Callback = typename jam::Active::Callback;

    explicit Backgrounder(std::vector<T>& received)
        : active_{jam::Active::create()}
        , received_{received}
        { }

    ~Backgrounder() = default;

    void save_data(T value)
    {
        active_.send([this, v=std::move(value)]{bgStoreData(Data(std::move(v)));});
    }
};


// This isn't that great - it's still possible to construct the Backgrounder2 object
// without using the factory member function.
// Proper construction is done via the base class static member function Active2::create
// which is not entirely intuitive (and not enforced).
template<typename T>
class Backgrounder2 : public jam::Active2 {
    using Base = jam::Active2;

    std::vector<T>& received_;

    struct Data {
        Data(T v) : value{std::move(v)} { }
        T value;
    };

    void bgStoreData(Data msg) {
        received_.push_back(std::move(msg.value));
        std::this_thread::sleep_for(std::chrono::microseconds{11});
    }

public:
    using jam::Active2::Callback;

    explicit Backgrounder2(std::vector<T>& received)
        : Base{}
        , received_{received}
        {}

    Backgrounder2(Backgrounder2 const&) = default;
    Backgrounder2(Backgrounder2&&) = default;

    ~Backgrounder2() = default;

    void save_data(T value) {
        Base::send([this, v=std::move(value)]{ bgStoreData(Data{std::move(v)}); });
    }
};


// Implementation using CRTP
template<typename T>
class Backgrounder3 : public jam::Active3<Backgrounder3<T>> {
    using Base = jam::Active3<Backgrounder3<T>>;

    std::vector<T>& received_;

    struct Data {
        Data(T v) : value{std::move(v)} { }
        T value;
    };

    void bgStoreData(Data msg)
    {
        received_.push_back(std::move(msg.value));
        std::this_thread::sleep_for(std::chrono::microseconds{11});
    }

public:
    using typename Base::Callback;

    explicit Backgrounder3(std::vector<T>& received)
        : Base{}
        , received_{received}
        {}

    Backgrounder3(Backgrounder3 const&) = default;
    Backgrounder3(Backgrounder3&&) = default;

    ~Backgrounder3() = default;

    void save_data(T value) {
        Base::send([this, v=std::move(value)]{ bgStoreData(Data{std::move(v)}); });
    }
};
