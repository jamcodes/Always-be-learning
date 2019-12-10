#include <iostream>
#include <utility>
#include <optional>

class Resource { };
struct ResourceParams { };

// There are four main "class natures" (the reason to implement a class):
// * Value types
// * Polymorphic types
// * Relationship (pointer/references) types
// * Manager types
// Cannonical forms of these class natures are the patterns that should be applied to guide
// safe implementation of these types

// # Manager types
// They manage lifetime of resources in a given scope/conditions.
// Similiar to std::unique_ptr, std::lock_guard, std::unique_lock, etc.

// Scoped managers
// - Manage resources only within the scope they are created in,
// - Should have copying disallowed - only one instance should exist - std::lock_guard,

class Scoped {
private:
    Resource resource_;
public:
    Scoped() { /* acquire resource */ }
    ~Scoped() noexcept { /* release resource */ }
    Scoped& operator=(Scoped&&) = delete;   // prohibit move, copy, construction and assignment
};

// Unique managers
// - Manage a resource but allow move operations. This implies that we need to be able
// to represent the moved-from state. This is obvious for pointer-manager objects - nullptr
// is the natural moved-from state value.
// for Value-type manager objects std::optional can be a good alternative.

class UniqueManager {
private:
    Resource* resource_;
    void release() noexcept;
public:
    UniqueManager() = default;
    UniqueManager(Resource* resource) : resource_{resource} { }
    ~UniqueManager() noexcept;
    UniqueManager(UniqueManager&&) noexcept;
    UniqueManager& operator=(UniqueManager&&) & noexcept;
};

void UniqueManager::release() noexcept
{
    if (resource_) {
        // Do resource release actions here - close a file, unlock a lock, etc
        resource_ = nullptr;
    }
}

UniqueManager::~UniqueManager() noexcept
{
    this->release();
}

UniqueManager::UniqueManager(UniqueManager&& other) noexcept
    : resource_{std::exchange(other.resource_, nullptr)}
{
}

UniqueManager& UniqueManager::operator=(UniqueManager&& other) & noexcept
{
    if (this != &other) {
        this->release();
        using std::swap;
        swap(resource_, other.resource_);
    }
    return *this;
    // Alternatively - skip the check, just swap and let the other object do the cleanup?
    // If we don't strictly care when the object will be released this is a valid option?
}


class UniqueValueManager {
private:
    std::optional<Resource> resource_;
    void release() noexcept;
public:
    UniqueValueManager() = default;
    UniqueValueManager(ResourceParams p);   // acquire resource
    ~UniqueValueManager() noexcept;
    UniqueValueManager(UniqueValueManager&&) noexcept;
    UniqueValueManager& operator=(UniqueValueManager&&) & noexcept;
};

void UniqueValueManager::release() noexcept
{
    if (resource_) {
        // Do resource release actions here - close a file, unlock a lock, etc
        resource_.reset();
    }
}

UniqueValueManager::~UniqueValueManager() noexcept
{
    this->release();
}

UniqueValueManager::UniqueValueManager(UniqueValueManager&& other) noexcept
    : resource_{std::exchange(other.resource_, std::nullopt)}
{
}

UniqueValueManager& UniqueValueManager::operator=(UniqueValueManager&& other) & noexcept
{
    if (this != &other) {
        this->release();
        using std::swap;
        swap(resource_, other.resource_);
    }
    return *this;
}

int main()
{

}
