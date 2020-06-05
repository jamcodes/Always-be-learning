#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <ctime>

namespace jam
{
class SemaphoreError : std::system_error
{
public:
    using std::system_error::system_error;
    using std::system_error::what;
};

class NamedSemaphoreBase
{
public:
    explicit NamedSemaphoreBase(const std::string& name, int oflags = O_CREAT, mode_t mode = 0755);
    NamedSemaphoreBase(NamedSemaphoreBase const&) = delete;
    NamedSemaphoreBase& operator=(NamedSemaphoreBase const&) = delete;
    NamedSemaphoreBase(NamedSemaphoreBase&&) noexcept = default;
    NamedSemaphoreBase& operator=(NamedSemaphoreBase&&) noexcept = default;

    template <typename Duration>
    bool try_wait_until(std::chrono::time_point<std::chrono::system_clock, Duration> timeout_time)
    {
        auto timeout{std::chrono::time_point_cast<std::chrono::system_clock::duration>(timeout_time)};
        return try_wait_until_impl(timeout);
    }

    template <typename Ratio>
    bool try_wait_for(std::chrono::duration<Ratio> const timeout)
    {
        return try_wait_until_impl(std::chrono::system_clock::now() + timeout);
    }

    void post();
    void wait();
    bool try_wait();

protected:
    ~NamedSemaphoreBase() = default;

    bool try_wait_until_impl(
        std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> const timepoint);

    static void unlink(const char* name);

    sem_t* sem_;
};

template <bool Owning>
class NamedSemaphore : private NamedSemaphoreBase
{
public:
    explicit NamedSemaphore(std::string name, int oflags = O_CREAT, mode_t mode = 0755) :
      NamedSemaphoreBase{name, oflags, mode},
      name_{std::move(name)}
    {
    }

    NamedSemaphore(NamedSemaphore const&) = delete;
    NamedSemaphore& operator=(NamedSemaphore const&) = delete;
    NamedSemaphore(NamedSemaphore&&) noexcept = default;

    NamedSemaphore& operator=(NamedSemaphore&&) noexcept;
    ~NamedSemaphore() noexcept;

    std::string const& getName() const noexcept
    {
        return name_;
    }

    using NamedSemaphoreBase::post;
    using NamedSemaphoreBase::wait;
    using NamedSemaphoreBase::try_wait;
    using NamedSemaphoreBase::try_wait_for;
    using NamedSemaphoreBase::try_wait_until;

private:
    std::string name_;
};

using OwningNamedSemaphore = NamedSemaphore<true>;
using NonowningNamedSemaphore = NamedSemaphore<false>;

}    // namespace jam
