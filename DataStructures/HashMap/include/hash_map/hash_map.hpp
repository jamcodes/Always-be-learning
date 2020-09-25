#ifndef DATA_STRUCTURES_HASH_MAP_HPP
#define DATA_STRUCTURES_HASH_MAP_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename Key, typename T>
class BucketNode;
template <typename Key, typename T, typename Allocator>
class HashBucket;
template <typename HashMapT>
class HashMapIterator;
template <typename HashMapT>
class HashMapConstIterator;

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class HashMap {
    using alloc_traits = typename std::allocator_traits<Allocator>;
    using self = HashMap<Key, T, Hash, Allocator>;
    constexpr std::size_t Default_Bucket_Count{17};
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using allocator_type = Allocator;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t < std::add_const<t<value_type>>;
    using pointer = alloc_traits::pointer;
    using const_pointer = alloc_traits::const_pointer;
    using iterator = HashMapIterator<self>;
    using const_iterator = HashMapConstIterator<self>;
    using local_iterator = typename HashBucket<Key, T, Allocator>::iterator;
    using const_local_iterator = typename HashBucket<Key, T, Allocator>::const_iterator;

    HashMap() : HashMap(Default_Bucket_Count) { }
    explicit HashMap(size_type bucket_count, Hash const& hash = Hash{}, Allocator const& alloc = Allocator{});
    explicit HashMap(size_type bucket_count, Allocator const& alloc = Allocator{})
        : HashMap(bucket_count, Hash{}, alloc) { }
    explicit HashMap(Allocator const& alloc);
    template<typename InputIt>
    explicit HashMap(InputIt first, InputIt last, size_type bucket_count = Default_Bucket_Count,
                     Hash const& hash = Hash{}, Allocator const& alloc = Allocator{});
    HashMap(HashMap const& other);
    HashMap(HashMap const& other, Allocator const& alloc);
    HashMap(HashMap&& other);
    HashMap(HashMap&& other, Allocator const& alloc);
    ~HashMap() noexcept;

    HashMap& operator=(HashMap const& other);
    HashMap& operator=(HashMap&& other) noexcept;

    // allocator access
    allocator_type get_allocator() const noexcept;

    // iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // capacity
    bool empty() const noexcept;
    size_type size() const noexcept;

    // modifiers
    void clear() noexcept;
    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);
    template<typename P /*, typename = sfinae if convertible to value_type */>
    std::pair<iterator, bool> insert(P&& value);
    iterator insert(const_iterator hint, value_type const& value);
    iterator insert(const_iterator hint, value_type&& value);

    template<typename M /* , typename = sfinae if convertible to T */>
    std::pair<iterator, bool> insert_or_assign(key_type const&, M&& value);
    template<typename M /* , typename = sfinae if convertible to T */>
    std::pair<iterator, bool> insert_or_assign(key_type&&, M&& value);
    template<typename M /* , typename = sfinae if convertible to T */>
    iterator insert_or_assign(const_iterator hint, key_type const&, M&& value);
    template<typename M /* , typename = sfinae if convertible to T */>
    iterator insert_or_assign(const_iteraotr hint, key_type&&, M&& value);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    template<typename... Args>
    std::pair<iterator, bool> emplace_hint(const_iterator hint, Args&&... args);

    template<typename... Args /* , typename = sfinae if convertible to T */>
    std::pair<iterator, bool> try_emplace(key_type const&, Args&&... value);
    template<typename... Args /* , typename = sfinae if convertible to T */>
    std::pair<iterator, bool> try_emplace(key_type&&, Args&&... value);
    template<typename... Args /* , typename = sfinae if convertible to T */>
    iterator try_emplace(const_iterator hint, key_type const&, Args&&... value);
    template<typename... Args /* , typename = sfinae if convertible to T */>
    iterator try_emplace(const_iteraotr hint, key_type&&, Args&&... value);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(key_type const& key);

    void swap(HashMap& other) noexcept;

    // lookup
    reference at(Key const&);
    const_reference at(Key const&) const;
    reference operator[](Key const&);
    reference operator[](Key&&);

    size_type count(Key const&) const noexcept;

    iterator find(Key const&) noexcept;
    const_iterator find(Key const&) const noexcept;

    std::pair<iterator, iterator> equal_range(Key const&);
    std::pair<const_iterator, const_iterator> equal_range(Key const&);

    // bucket interface
    local_iterator begin(size_type n);
    const_local_iterator begin(size_type n) const;
    const_local_iterator cbegin(size_type n) const;
    local_iterator end(size_type n);
    const_local_iterator end(size_type n) const;
    const_local_iterator cend(size_type n) const;

    size_type bucket_count() const noexcept;
    size_type bucket_size(size_type n) const;
    size_type bucket(Key const& k) const noexcept;

    // hash policy
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml) noexcept;
    void rehash(size_type count);
    void reserve(size_type count);
};

#endif  // DATA_STRUCTURES_HASH_MAP_HPP
