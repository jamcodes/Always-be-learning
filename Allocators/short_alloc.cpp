#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <unordered_set>
#include <new>
#include <cstdint>
#include <utility>

#include "short_alloc.h"

// Overload new and delete to show they're not called

std::size_t memory{0};
std::size_t sized_delete{0};
std::size_t alloc{0};

void* operator new(std::size_t s)
{
    memory += s;
    ++alloc;
    return std::malloc(s);
}

void* operator new[](std::size_t s)
{
    memory += s;
    ++alloc;
    return std::malloc(s);
}

void operator delete(void* p)
{
    --alloc;
    std::free(p);
}

void operator delete(void* p, std::size_t size)
{
    --alloc;
    sized_delete += size;
    std::free(p);
}

void operator delete[](void* p)
{
    --alloc;
    std::free(p);
}

void operator delete[](void* p, std::size_t size)
{
    --alloc;
    sized_delete += size;
    std::free(p);
}

void memory_use()
{
    std::cout << "mem: " << memory << ", " << "allocs: " << alloc << ", "
        << "dels: " << sized_delete << "\n";
}

// Create a vector<T> template with a small buffer of 200 bytes.
//   Note for vector it is possible to reduce the alignment requirements
//   down to alignof(T) because vector doesn't allocate anything but T's.
//   And if we're wrong about that guess, it is a compile-time error, not
//   a run time error.
template <class T, std::size_t BufSize = 200>
using SmallVector = std::vector<T, short_alloc<T, BufSize, alignof(T)>>;

// Create a small list - use default (max) alignment, because list allocates nodes internally
// which are larger then sizeof(T) and may have different allignment. Could experiment
// with smaller allignemtn - it likely won't be portable
template<typename T, std::size_t BufSize = 256>
using SmallList = std::list<T, short_alloc<T, BufSize>>;

// Create a small unordered set - same as with list - alignment either needs to be default
// or adjusted for the given platform
template<typename T, std::size_t BufSize = 256>
using SmallSet = std::unordered_set<T, std::hash<T>, std::equal_to<T>,
                                    short_alloc<T, BufSize, alignof(T) < 8 ? 8 : alignof(T)>>;


// A string of up to 64 characters
template<typename CharT, std::size_t BufSize = 64>
using SmallBasicString =
    std::basic_string<CharT, std::char_traits<CharT>, short_alloc<CharT, BufSize, alignof(CharT)>>;
using SmallString = SmallBasicString<char>;

int main()
{
    // Create a stack-based arena from which to allocate
    SmallVector<int>::allocator_type::arena_type small_arena;
    // Create a vector which uses the arena;
    SmallVector<int> vec{small_arena};
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    memory_use();
    for (auto i : vec) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    vec.erase(--vec.end());
    memory_use();
    for (auto i : vec) {
        std::cout << i << " ";
    }

    std::cout << "\n\nSmall List:\n";
    // Use the small list with its own arena
    SmallList<int>::allocator_type::arena_type list_arena;
    SmallList<int> lst{list_arena};
    lst.push_back(1);
    lst.push_front(2);
    lst.push_back(3);
    lst.push_front(4);
    memory_use();
    for (auto i : lst) { std::cout << i << " "; }
    std::cout << "\n";

    lst.erase(lst.begin());
    for (auto i : lst) { std::cout << i << " "; }
    std::cout << "\n";

    memory_use();


    std::cout << "\n\nSmall Set:\n";
    SmallSet<int>::allocator_type::arena_type set_arena;
    SmallSet<int> sset{set_arena};
    sset.insert(1);
    sset.insert(2);
    sset.insert(4);
    sset.insert(1);
    sset.insert(2);
    memory_use();
    for (auto i : sset) { std::cout << i << " "; }
    std::cout << "\n";

    sset.erase(sset.begin());
    for (auto i : sset) { std::cout << i << " "; }
    std::cout << "\n";
    memory_use();

    std::cout << "\n\nSmall String:\n";
    SmallString::allocator_type::arena_type string_arena;
    SmallString str{string_arena};
    str.assign("This is quite long, but it still won't allocate");
    memory_use();
    std::cout << "str: " << str << "\n";
}
