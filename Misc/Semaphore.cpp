#include "named_semaphore.h"

#include <cstring>
#include <iostream>
#include <utility>

namespace jam
{
NamedSemaphoreBase::NamedSemaphoreBase(const std::string& name, int oflags /* = O_CREAT */, mode_t mode /* = 0755 */)
{
    errno = 0;
    sem_ = sem_open(name.c_str(), oflags, mode, 0);
    if (SEM_FAILED == sem_)
    {
        throw SemaphoreError{errno, std::system_category(), "sem_open failed"};
    }
}

void NamedSemaphoreBase::unlink(const char* name)
{
    errno = 0;
    if (sem_unlink(name) != 0)
    {
        throw SemaphoreError{errno, std::system_category(), "sem_unlink failed"};
    }
}

void NamedSemaphoreBase::wait()
{
    errno = 0;
    if (sem_wait(sem_) != 0)
    {
        throw SemaphoreError{errno, std::system_category(), "wait failed"};
    }
}

bool NamedSemaphoreBase::try_wait()
{
    errno = 0;
    if (sem_trywait(sem_) != 0)
    {
        if (errno != EAGAIN)
        {
            throw SemaphoreError{errno, std::system_category(), "trywait failed"};
        }
        return false;
    }
    return true;
}

void NamedSemaphoreBase::post()
{
    errno = 0;
    if (sem_post(sem_) != 0)
    {
        throw SemaphoreError{errno, std::system_category(), "post failed"};
    }
}

bool NamedSemaphoreBase::try_wait_until_impl(
    std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> const timepoint)
{
    auto const seconds{std::chrono::time_point_cast<std::chrono::seconds>(timepoint)};
    auto const nanoseconds{std::chrono::time_point_cast<std::chrono::nanoseconds>(timepoint) -
                           std::chrono::time_point_cast<std::chrono::nanoseconds>(seconds)};
    struct timespec ts
    {
    };
    ts.tv_sec = seconds.time_since_epoch().count();
    ts.tv_nsec = nanoseconds.count();
    errno = 0;
    if (sem_timedwait(sem_, &ts) != 0)
    {
        if (errno != ETIMEDOUT)
        {
            throw SemaphoreError{errno, std::system_category(), "timedwait failed"};
        }
        return false;
    }
    return true;
}

template <>
NamedSemaphore<true>::~NamedSemaphore() noexcept
{
    try
    {
        NamedSemaphoreBase::unlink(name_.c_str());
    }
    catch (SemaphoreError const& er)
    {
        std::cerr << "Error: Failed to unlink semaphore: '" << er.what() << "'\n"
                  << "errno = '" << std::strerror(errno) << "'\n";
        std::terminate();
    }
}

template <>
auto NamedSemaphore<true>::operator=(NamedSemaphore&& other) noexcept -> NamedSemaphore&
{
    if (this == &other)
    {
        return *this;
    }
    NamedSemaphoreBase::unlink(name_.c_str());
    name_ = std::move(other.name_);
    sem_ = other.sem_;
    return *this;
}

template <>
NamedSemaphore<false>::~NamedSemaphore() noexcept = default;

template <>
auto NamedSemaphore<false>::operator=(NamedSemaphore&&) noexcept -> NamedSemaphore& = default;

}    // namespace jam
