#ifndef DATA_STRUCTURES_HASH_MAP_HPP
#define DATA_STRUCTURES_HASH_MAP_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "hash_map_bucket.hpp"

template <typename BucketType, typename Reference>
class HashMapIterator;


template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class HashMap {
    using ValueType         = std::pair<const Key, T>;
    using ValueAlloc        = typename std::allocator_traits<Allocator>::template rebind_alloc<ValueType>;
    using ValueAllocTraits  = std::allocator_traits<ValueAlloc>;
    using Bucket            = HashMapBucket<Key, T, Allocator>;
    using BucketAlloc       = typename ValueAllocTraits::template rebind_alloc<Bucket>;
    using BucketAllocTraits = std::allocator_traits<BucketAlloc>;
    using self = HashMap<Key, T, Hash, Allocator>;

    static constexpr std::size_t Default_Bucket_Count{17};

    BucketAlloc alloc_{};
    Hash hash_{};
    std::size_t size_{Default_Bucket_Count};
    Bucket* buckets_;
    std::size_t count_{0};
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using allocator_type = Allocator;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    using pointer = typename ValueAllocTraits::pointer;
    using const_pointer = typename ValueAllocTraits::const_pointer;
    using iterator = HashMapIterator<Bucket, reference>;
    using const_iterator = HashMapIterator<Bucket, const_reference>;
    using local_iterator = typename HashMapBucket<Key, T, Allocator>::iterator;
    using const_local_iterator = typename HashMapBucket<Key, T, Allocator>::const_iterator;

    HashMap() : HashMap(Default_Bucket_Count) { }

    explicit HashMap(size_type bucket_count, Hash const& hash = Hash{}, Allocator const& alloc = Allocator{})
        : alloc_{BucketAlloc{alloc}}, hash_{hash}, size_{bucket_count}, buckets_{make_buckets(bucket_count)}
    {
    }

    explicit HashMap(Allocator const& alloc)
        : alloc_{BucketAlloc{alloc}}, buckets_{make_buckets(size_)}
    {
    }

    template<typename InputIt, typename = RequiresInputIterator<InputIt>>
    explicit HashMap(InputIt first, InputIt last, size_type bucket_count = Default_Bucket_Count,
                     Hash const& hash = Hash{}, Allocator const& alloc = Allocator{})
        : HashMap{bucket_count, hash, alloc}
    {
        // while (first != last) {
        //     auto bucket = get_bucket(first->first);
        //     bucket->push_front(*first);
        //     ++first;
        //     ++count_;
        // }
        count_ = fill_buckets(first, last);
    }

    HashMap(HashMap const& other)
        : alloc_{BucketAllocTraits::select_on_container_copy_construction(other.alloc_)},
          hash_{other.hash_},
          size_{other.size_},
          buckets_{make_buckets(size_)}
    {
        // for (auto b{other.cbegin()}; b != other.cend(); ++b) {
        //     auto bucket = get_bucket(b->first);
        //     bucket->push_front(*b);
        //     ++count_;
        // }
        count_ = fill_buckets(other.cbegin(), other.cend());
        JAM_ENSURE(count_ == other.count_, "Copy construction - failed to copy all elements");
    }

    HashMap(HashMap const& other, Allocator const& alloc)
        : alloc_{BucketAlloc{alloc}},
          hash_{other.hash_},
          size_{other.size_},
          buckets_{make_buckets(size_)},
          count_{other.count_}
    {

    }
    HashMap(HashMap&& other) noexcept
        : alloc_{std::move(other.alloc_)},
          hash_{std::move(other.hash_)},
          size_{other.size_},
          buckets_{std::exchange(other.buckets_, nullptr)},
          count_{std::exchange(other.count_, 0)}
    {
    }

    HashMap(HashMap&& other, Allocator const& alloc)
        : alloc_{BucketAlloc{alloc}},
          hash_{std::move(other.hash_)},
          size_{other.size_},
          buckets_{std::exchange(other.buckets_, nullptr)},
          count_{std::exchange(other.count_, 0)}
    {
        JAM_ENSURE(alloc_ == other.alloc_, "Move construction with incompatible allocator");
    }

    ~HashMap() noexcept { free(); }

    HashMap& operator=(HashMap const& other)
    {
        if (this != &other) {
            free();
            if (BucketAllocTraits::propagate_on_container_copy_assignment::value) {
                alloc_ = other.alloc_;
            }
            hash_ = other.hash_;
            size_ = other.size_;
            buckets_ = make_buckets(size_);
            count_ = fill_buckets(other.cbegin(), other.cend());
        }
        return *this;
    }

    HashMap& operator=(HashMap&& other) noexcept
    {
        if (this != &other) {
            free();
            if (BucketAllocTraits::propagate_on_container_move_assignment::value) {
                alloc_ = std::move(other.alloc_);
            }
            hash_ = std::move(other.hash_);
            size_ = other.size_;
            buckets_ = std::exchange(other.buckets_, nullptr);
            count_ = std::exchange(other.count_, 0);
        }
        return *this;
    }

    // allocator access
    allocator_type get_allocator() const noexcept { return alloc_; }

    // iterators
    iterator begin() noexcept { iterator{buckets_, buckets_ + size_}; }
    const_iterator begin() const noexcept { return const_iterator{buckets_, buckets_ + size_}; }
    const_iterator cbegin() const noexcept { return const_iterator{buckets_, buckets_ + size_}; }

    iterator end() noexcept { return iterator{buckets_ + size_}; }
    const_iterator end() const noexcept { return const_iterator{buckets_ + size_}; }
    const_iterator cend() const noexcept { return const_iterator{buckets_ + size_}; }

    // capacity
    bool empty() const noexcept { return count_ == 0; }
    size_type size() const noexcept { return count_; }

    // modifiers
    void clear() noexcept { free(); }

    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);
    template<typename P, typename = std::enable_if_t<std::is_convertible_v<P, value_type>>>
    std::pair<iterator, bool> insert(P&& value);
    // TODO: implement the hint'ed operations
    iterator insert(const_iterator hint, value_type const& value);
    iterator insert(const_iterator hint, value_type&& value);

    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    std::pair<iterator, bool> insert_or_assign(key_type const&, M&& value);
    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    std::pair<iterator, bool> insert_or_assign(key_type&&, M&& value);
    // TODO: implement the hinted operations
    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    iterator insert_or_assign(const_iterator hint, key_type const&, M&& value);
    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    iterator insert_or_assign(const_iterator hint, key_type&&, M&& value);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    template<typename... Args>
    std::pair<iterator, bool> emplace_hint(const_iterator hint, Args&&... args);

    template<typename... Args typename = std::enable_if_t<std::is_constructible_v<value_type, Args&&...>>>
    std::pair<iterator, bool> try_emplace(key_type const&, Args&&... value);
    template<typename... Args typename = std::enable_if_t<std::is_constructible_v<value_type, Args&&...>>>
    std::pair<iterator, bool> try_emplace(key_type&&, Args&&... value);

    // TODO: implement hinted operations
    template<typename... Args typename = std::enable_if_t<std::is_constructible_v<value_type, Args&&...>>>
    iterator try_emplace(const_iterator hint, key_type const&, Args&&... value);
    template<typename... Args typename = std::enable_if_t<std::is_constructible_v<value_type, Args&&...>>>
    iterator try_emplace(const_iterator hint, key_type&&, Args&&... value);

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
    std::pair<const_iterator, const_iterator> equal_range(Key const&) const;

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

protected:
    using buckets_iterator = Bucket*;
    using const_buckets_iterator = Bucket const*;
    buckets_iterator bbegin() noexcept { return buckets_; }
    const_buckets_iterator bbegin() const noexcept { return buckets_;}
    const_buckets_iterator bcbegin() const noexcept { return buckets_;}
    buckets_iterator bend() noexcept { return buckets_ + size_; }
    const_buckets_iterator bend() const noexcept { return buckets_ + size_;}
    const_buckets_iterator bcend() const noexcept { return buckets_ + size_;}

    Bucket* make_buckets(size_type n, Bucket const& value = Bucket{})
    {
        Bucket* buckets{BucketAllocTraits::allocate(alloc_, n)};
        std::uninitialized_fill_n(buckets, n, value);
        return buckets;
    }

    template<typename InputIt>
    size_type fill_buckets(InputIt first, InputIt last)
    {
        size_type count{0};
        while (first != last) {
            auto bucket{get_bucket(first->first)};
            bucket->push_front(*first);
            ++first;
            ++count;
        }
        return count;
    }

    template<typename Key, typename... Args> inline
    std::enable_if_t<std::is_convertible_v<Key&&, key_type>, buckets_iterator> get_bucket(Key&& key, Args&&... args)
    {
        auto bucket{bbegin()};
        std::advance(bucket, bucket_index(key));
        return bucket;
    }

    constexpr inline size_type bucket_index(key_type const& key) const noexcept
    {
        return hash_(key) % size_;
    }

    inline buckets_iterator get_bucket(key_type const& key) noexcept
    {
        auto bucket{bbegin()};
        std::advance(bucket, bucket_index(key));
        return bucket;
        // return std::next(bbegin(), bucket_index(key));
    }

    inline const_buckets_iterator get_bucket(key_type const& key) const noexcept
    {
        auto bucket{bbegin()};
        std::advance(bucket, bucket_index(key));
        return bucket;
        // return std::next(bbegin(), bucket_index(key));
    }

    void free() noexcept
    {
        if (buckets_)
        {
            for (auto* p{buckets_ + size_}; p != buckets_; ) {
                BucketAllocTraits::destroy(alloc_, --p);
            }
            BucketAllocTraits::deallocate(alloc_, buckets_, size_);
        }
    }
};


template <typename Key, typename T, typename Hash, typename Allocator>
auto HashMap<Key, T, Hash, Allocator>::insert(const value_type& value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(value.first)};
    auto res{bucket->insert_unique(value)};
    return {iterator{bucket, bend(), res.first}};
}

template <typename Key, typename T, typename Hash, typename Allocator>
auto HashMap<Key, T, Hash, Allocator>::insert(value_type&& value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(value.first)};
    auto res{bucket->insert_unique(std::move(value))};
    return {iterator{bucket, bend(), res.first}, res.second};
}

template <typename Key, typename T, typename Hash, typename Allocator>
template<typename P, typename>
auto HashMap<Key, T, Hash, Allocator>::insert(P&& value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(value.first)};
    auto res{bucket->insert_unique(std::forward<P>(value))};
    return {iterator{bucket, bend(), res.first}, res.second};
}

template <typename Key, typename T, typename Hash, typename Allocator>
template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
auto HashMap<Key, T, Hash, Allocator>::insert_or_assign(key_type const& key, M&& value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(key)};
    auto res{bucket->insert_or_assign(key, std::forward<M>(value))};
    return {iterator{bucket, bend(), res.first}, res.second};
}

template <typename Key, typename T, typename Hash, typename Allocator>
template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
auto HashMap<Key, T, Hash, Allocator>::insert_or_assign(key_type&& key, M&& value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(key)};
    auto res{bucket->insert_or_assign(std::move(key), std::forward<M>(value))};
    return {iterator{bucket, bend(), res.first}, res.second};
}

template <typename Key, typename T, typename Hash, typename Allocator>
template<typename... Args>
auto HashMap<Key, T, Hash, Allocator>::emplace(Args&&... args) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(args...)};
    auto res{bucket->emplace_unique(std::forward<Args>(args)...)};
    return {iterator{bucket, bend(), res.first}, res.second};
}

template <typename Key, typename T, typename Hash, typename Allocator>
template<typename... Args typename = std::enable_if_t<std::is_constructible_v<value_type, Args&&...>>>
auto HashMap<Key, T, Hash, Allocator>::try_emplace(key_type const& key, Args&&... value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(key)};
    auto res{bucket->try_emplace(key, std::forward<Args>(args)...)};
    return {iterator{bucket, bend(), res.first}, res.second};
}

template <typename Key, typename T, typename Hash, typename Allocator>
template<typename... Args typename = std::enable_if_t<std::is_constructible_v<value_type, Args&&...>>>
auto HashMap<Key, T, Hash, Allocator>::try_emplace(key_type&& key, Args&&... value) -> std::pair<iterator, bool>
{
    auto bucket{get_bucket(key)};
    auto res{bucket->try_emplace(std::move(key), std::forward<Args>(args)...)};
    return {iterator{bucket, bend(), res.first}, res.second};
}


template <typename BucketType, typename Reference>
class HashMapIterator
{
    using Bucket = BucketType;
    using CvValue = std::remove_reference_t<Reference>;
    using BucketIterator = std::conditional_t<
                            std::is_const_v<CvValue>, typename Bucket::const_iterator, typename Bucket::iterator>;
    using Self = HashMapIterator;

    Bucket* bucket_;
    Bucket* end_bucket_;
    BucketIterator node_{bucket_->begin()};
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::remove_const_t<CvValue>;
    using reference = Reference;
    using pointer = std::add_pointer_t<CvValue>;
    using difference_type = typename BucketType::difference_type;
    using const_iterator = HashMapIterator<BucketType, std::add_lvalue_reference_t<std::add_const_t<value_type>>>;

    // Ctor only for the end() iterator
    explicit HashMapIterator(Bucket* last) noexcept
        : bucket_{last}, end_bucket_{last}
    {
    }

    explicit HashMapIterator(Bucket* first, Bucket* last) noexcept
        : bucket_{first}, end_bucket_{last}
    {
        if (node_ == bucket_->end()) {
            next_node();
        }
    }

    explicit HashMapIterator(Bucket* first, Bucket* last, BucketIterator node) noexcept
        : bucket_{first}, end_bucket_{last}, node_{node}
    {
        if (node_ == bucket_->end()) {
            next_node();
        }
    }

    reference operator*() const noexcept { return *node_; }
    pointer operator->() const noexcept { return &(this->operator*()); }

    Self& operator++() noexcept
    {
        ++node_;
        next_node();
        return *this;
    }

    Self operator++(int) noexcept
    {
        auto result{*this};
        this->operator++();
        return result;
    }

    operator const_iterator() const noexcept { return const_iterator{*this}; }

    template<typename B1, typename R1, typename B2, typename R2>
    friend bool operator==(HashMapIterator<B1, R1> const& lhs, HashMapIterator<B2, R2> const& rhs) noexcept;
    template<typename B1, typename R1, typename B2, typename R2>
    friend bool operator!=(HashMapIterator<B1, R1> const& lhs, HashMapIterator<B2, R2> const& rhs) noexcept;

private:
    void next_node() noexcept
    {
        while (node_ == bucket_->end() && bucket_ != end_bucket_) {
            ++bucket_;
            node_ = bucket_->begin();
        }
    }
};

template<typename B1, typename R1, typename B2, typename R2>
bool operator==(HashMapIterator<B1, R1> const& lhs, HashMapIterator<B2, R2> const& rhs) noexcept
{
    return lhs.bucket_ == rhs.bucket_ && lhs.node_ == rhs.node_;
}

template<typename B1, typename R1, typename B2, typename R2>
bool operator!=(HashMapIterator<B1, R1> const& lhs, HashMapIterator<B2, R2> const& rhs) noexcept
{
    return !(lhs == rhs);
}


#endif  // DATA_STRUCTURES_HASH_MAP_HPP
