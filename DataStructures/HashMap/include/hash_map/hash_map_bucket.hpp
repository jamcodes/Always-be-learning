#ifndef DATA_STRUCTURES_HASH_MAP_BUCKET_HPP
#define DATA_STRUCTURES_HASH_MAP_BUCKET_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include <forward_list>

#include "utils/Assertion.h"
#include "utils/traits.h"

// template<typename Derived>
struct BucketNodeBase {
    mutable BucketNodeBase* next{nullptr};

    constexpr BucketNodeBase() noexcept = default;
    constexpr BucketNodeBase(BucketNodeBase&& other) noexcept : next{std::move(other.next)}
    {
        other.next = nullptr;
    }
    constexpr BucketNodeBase& operator=(BucketNodeBase&& other) noexcept
    {
        next = std::move(other.next);
        other.next = nullptr;
        return *this;
    }
    BucketNodeBase(BucketNodeBase const&) = delete;
    BucketNodeBase& operator=(BucketNodeBase const&) = delete;
    ~BucketNodeBase() noexcept = default;

    // decltype(auto) data() & noexcept { return derived().data; }
    // decltype(auto) data() const& noexcept { return derived().data; }
    // decltype(auto) data() && noexcept { return derived().data; }
    // decltype(auto) data() const&& noexcept { return derived().data; }

    // Derived& derived() & noexcept               { return static_cast<Derived&>(*this); }
    // Derived const& derived() const& noexcept    { return static_cast<Derived const&>(*this); }
    // Derived&& derived() && noexcept             { return static_cast<Derived&&>(*this); }
    // Derived const&& derived() const&& noexcept  { return static_cast<Derived const&&>(*this); }
};

template <typename Key, typename T>
struct BucketNode : public BucketNodeBase
{
    using value_type    = std::pair<const Key, T>;
    using key_type      = Key;
    using mapped_type   = T;

    constexpr BucketNode() noexcept(std::is_nothrow_default_constructible_v<value_type>) = default;
    template<typename... Args>
    constexpr BucketNode(Args&&... args) noexcept(std::is_nothrow_constructible_v<value_type, Args&&...>)
        : data{std::forward<Args>(args)...} { }

    value_type data;
};

template<typename NodeType, typename Reference>
class HashMapBucketIterator;

template<typename Key, typename T, typename Allocator = std::allocator<BucketNode<Key, T>>>
class HashMapBucket {
public:
    using NodeBase      = BucketNodeBase;
    using Node          = BucketNode<Key, T>;
private:
    using AllocatorTraits = std::allocator_traits<Allocator>;
    using Alloc         = typename AllocatorTraits::template rebind_alloc<Node>;
    using AllocTraits   = std::allocator_traits<Alloc>;
    using Self          = HashMapBucket;

    Alloc alloc_{};
    NodeBase head_{};
public:
    using value_type        = typename Node::value_type;
    using key_type          = typename Node::key_type;
    using mapped_type       = typename Node::mapped_type;
    using reference         = std::add_lvalue_reference_t<value_type>;
    using const_reference   = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    using pointer           = std::add_pointer_t<value_type>;
    using const_pointer     = std::add_pointer_t<std::add_const_t<value_type>>;
    using difference_type   = typename AllocTraits::difference_type;
    using size_type         = std::size_t;
    using iterator          = HashMapBucketIterator<Node, reference>;
    using const_iterator    = HashMapBucketIterator<Node const, const_reference>;

// --- constructors
    HashMapBucket() noexcept = default;
    template<typename InputIt, typename = RequiresInputIterator<InputIt>>
    HashMapBucket(InputIt first, InputIt last)
    {
        if (first != last) {
            auto nodes{make_range(first, last)};
            head_.next = nodes.first;
        }
    }

    HashMapBucket(HashMapBucket const& other)
        : alloc_{AllocTraits::select_on_container_copy_construction(other.alloc_)}
    {
        if (!other.empty()) {
            auto nodes{make_range(other.cbegin(), other.cend())};
            head_.next = nodes.first;
        }
    }

    HashMapBucket(HashMapBucket&& other) noexcept
        : alloc_{std::move(other.alloc_)}, head_{std::move(other.head_)} { }

    // HashMapBucket& operator=(HashMapBucket const&);

    HashMapBucket& operator=(HashMapBucket&& other) noexcept
    {
        if (AllocTraits::propagate_on_container_move_assignment::value) {
            alloc_ = std::move(other.alloc_);
        }
        head_ = std::move(other.head_);
        return *this;
    }

    ~HashMapBucket() noexcept { free(); }

// --- iterators
    iterator before_begin() noexcept { return iterator{static_cast<Node*>(&head_)}; }
    const_iterator before_begin() const noexcept { return const_iterator{static_cast<Node const*>(&head_)}; }
    const_iterator before_cbegin() const noexcept { return const_iterator{static_cast<Node const*>(&head_)}; }
    iterator begin() noexcept { return iterator{static_cast<Node*>(head_.next)}; }
    const_iterator begin() const noexcept { return const_iterator{static_cast<Node*>(head_.next)}; }
    const_iterator cbegin() const noexcept { return const_iterator{static_cast<Node*>(head_.next)}; }
    iterator end() noexcept { return iterator{}; }
    const_iterator end() const noexcept { return const_iterator{}; }
    const_iterator cend() const noexcept { return const_iterator{}; }

// --- capacity
    bool empty() const noexcept { return head_.next == nullptr; }

    size_type size() const noexcept
    {
        size_type count{0};
        for (NodeBase* n{head_.next}; n != nullptr; n = n->next) {
            ++count;
        }
        return count;
    }

    void clear() noexcept { free(); }

    void push_front(value_type const& value)
    {
        Node* nn{make_node(value)};
        insert_front(nn);
    }

    void push_front(value_type&& value)
    {
        Node* nn{make_node(std::move(value))};
        insert_front(nn);
    }

    template<typename P, typename = std::enable_if_t<std::is_convertible_v<P&&, value_type>>>
    std::pair<iterator, bool> insert_unique(P&& value)
    {
        auto const it{std::find_if(begin(), end(),
            [&key=value.first](auto const& v)noexcept { return v.first == key; })
        };
        if (it != end()) { return {it, false}; }
        else {
            push_front(std::forward<P>(value));
            return {begin(), true};
        }
    }

    template<typename Key, typename... Args>
    iterator find_iter(Key&& key, Args&&... args) const noexcept
    {
        return std::find_if(begin(), end(), [&key](auto const& v)noexcept { return v.first == key; });
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace_unique(Args&&... args)
    {
        auto const it{find_iter(args...)};
        if (it != end()) { return {it, false}; }
        else {
            emplace_front(std::forward<Args>(args)...);
            return {begin(), true};
        }
    }

    template<typename M, typename = std::enalbe_if_t<std::is_convertible_v<M&&, mapped_type>>>
    std::pair<iterator, bool> insert_or_assign(key_type const& key, M&& value)
    {
        auto const it{std::find_if(begin(), end(),
            [&key=value.first](auto const& v)noexcept { return v.first == key; })
        };
        if (it != end()) {
            it->second = std::forward<M>(value);
            return {it, false};
        }
        else {
            push_front({key, std::forward<M>(value)});
            return {begin(), true};
        }
    }

    template<typename M, typename = std::enalbe_if_t<std::is_convertible_v<M&&, mapped_type>>>
    std::pair<iterator, bool> insert_or_assign(key_type&& key, M&& value)
    {
        auto const it{std::find_if(begin(), end(),
            [&key=value.first](auto const& v)noexcept { return v.first == key; })
        };
        if (it != end()) {
            it->second = std::forward<M>(value);
            return {it, false};
        }
        else {
            push_front({std::move(key), std::forward<M>(value)});
            return {begin(), true};
        }
    }

    template<typename... Args>
    void emplace_front(Args&&... args)
    {
        Node* nn{make_node(std::forward<Args>(args)...)};
        insert_front(nn);
    }

    iterator insert_after(const_iterator pos, value_type const& value)
    {
        Node* nn{make_node(value)};
        return insert_after(pos.node_, nn);
    }

    iterator insert_after(const_iterator pos, value_type&& value)
    {
        Node* nn{make_node(std::move(value))};
        return insert_after(pos.node_, nn);
    }

    template<typename InputIt>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last)
    {
        Node* p{const_cast<Node*>(pos.node_)};
        while (first != last) {
            Node* nn{make_node(*first)};
            insert_after(p, nn);
            p = nn;
            ++first;
        }
        return iterator{p};
    }

    template<typename... Args>
    iterator emplace_after(const_iterator pos, Args&&... args)
    {
        Node* nn{make_node(std::forward<Args>(args)...)};
        return insert_after(pos.node_, nn);
    }

    size_type erase(key_type const& key) noexcept
    {
        size_type count{0};
        for (Node* n{static_cast<Node*>(&head_)}; n != nullptr && n->next != nullptr; n = static_cast<Node*>(n->next)) {
            if (static_cast<Node*>(n->next)->data.first == key) {
                erase_after(n);
                ++count;
            }
        }
        return count;
    }

    iterator erase_after(const_iterator pos) noexcept
    {
        erase_after(pos->node_);
        return iterator{pos->node_->next};
    }

protected:
    template<typename... Args>
    Node* make_node(Args&&... args)
    {
        Node* nn{AllocTraits::allocate(alloc_, 1)};
        AllocTraits::construct(alloc_, nn, std::forward<Args>(args)...);
        return nn;
    }

    template<typename... Args>
    std::pair<Node*, Node*> make_n(size_type count, Args&&... args)
    {
        // keep allocating and linking nodes until count == 0;
        Node* nn{make_node(args...)};
        Node* front{nn};
        while (--count) {
            Node* next_node{make_node(args...)};
            // link_nodes(nn, next_node);
            nn->next = next_node;
            nn = next_node;
        }
        return {front, nn};
    }
    template<typename Iter>
    std::pair<Node*, Node*> make_range(Iter first, Iter last)
    {
        JAM_EXPECT(first != last, "Range must contain at least one value");
        Node* nn{make_node(*first)};
        Node* front{nn};
        while (++first != last) {
            Node* next_node{make_node(*first)};
            nn->next = next_node;
            nn = next_node;
        }
        return {front, nn};
    }

    void insert_front(NodeBase* node) noexcept
    {
        node->next = head_.next;
        head_.next = node;
    }

    static iterator insert_after(const NodeBase* pos, NodeBase* node) noexcept
    {
        node->next = pos->next;
        pos->next = node;
        return iterator{static_cast<Node*>(node)};
    }

    void erase_after(NodeBase* node) noexcept
    {
        JAM_EXPECT(node != nullptr, "nullptr node");
        Node* to_free{static_cast<Node*>(node->next)};
        node->next = to_free->next;
        free(to_free);
    }

    void free(Node* node) noexcept
    {
        AllocTraits::destroy(alloc_, node);
        AllocTraits::deallocate(alloc_, node, 1);
    }

    void free() noexcept
    {
        Node* node{static_cast<Node*>(head_.next)};
        while (node != nullptr) {
            Node* temp{node};
            node = static_cast<Node*>(node->next);
            free(temp);
        }
    }
};


template<typename NodeType, typename Reference>
class HashMapBucketIterator
{
    using Node = NodeType;
    using Self = HashMapBucketIterator;
    template<typename K, typename T, typename Alloc> friend class HashMapBucket;
    template<typename N, typename R> friend class HashMapBucketIterator;

    Node* node_{nullptr};
public:
    using iterator_category     = std::forward_iterator_tag;
    using value_type            = std::remove_reference_t<std::remove_const_t<Reference>>;
    using reference             = Reference;
    using pointer               = std::add_pointer_t<std::remove_reference_t<Reference>>;
    using difference_type       = std::ptrdiff_t;
    using const_iterator        = HashMapBucketIterator<Node, std::add_lvalue_reference_t<std::add_const_t<value_type>>>;

    constexpr HashMapBucketIterator() noexcept = default;
    constexpr explicit HashMapBucketIterator(Node* node) noexcept : node_{node} { }

    template<typename N2, typename R2>
    constexpr explicit HashMapBucketIterator(HashMapBucketIterator<N2, R2> other)
        : node_{const_cast<Node*>(other.node_)}
    {
    }

    constexpr HashMapBucketIterator(HashMapBucketIterator const& other) noexcept
        : node_{other.node_}
    {
    }

    constexpr HashMapBucketIterator(HashMapBucketIterator&& other) noexcept
        : node_{std::move(other.node_)}
    {
    }

    HashMapBucketIterator& operator=(HashMapBucketIterator const& other) noexcept = default;

    HashMapBucketIterator& operator=(HashMapBucketIterator&& other) noexcept = default;

    template<typename N2, typename R2>
    HashMapBucketIterator& operator=(HashMapBucketIterator<N2, R2> other) noexcept
    {
        node_ = const_cast<Node*>(other.node_);
        return *this;
    }

    constexpr reference operator*() const noexcept { return node_->data; }
    constexpr pointer operator->() const noexcept { return &(operator*()); }
    constexpr Self& operator++() noexcept
    {
        node_ = static_cast<Node*>(node_->next);
        return *this;
    }
    constexpr Self operator++(int) noexcept
    {
        auto res{*this};
        operator++();
        return res;
    }

    constexpr operator const_iterator() const noexcept { return const_iterator{node_}; }

    constexpr bool operator==(Self const& other) const noexcept { return node_ == other.node_; }
    constexpr bool operator!=(Self const& other) const noexcept { return node_ != other.node_; }

    template<typename N1, typename N2, typename R1, typename R2> friend constexpr
    bool operator==(HashMapBucketIterator<N1, R1> const& lhs, HashMapBucketIterator<N2, R2> const& rhs) noexcept;
    template<typename N1, typename N2, typename R1, typename R2> friend constexpr
    bool operator!=(HashMapBucketIterator<N1, R1> const& lhs, HashMapBucketIterator<N2, R2> const& rhs) noexcept;
};

template<typename N1, typename N2, typename R1, typename R2> constexpr
bool operator==(HashMapBucketIterator<N1, R1> const& lhs, HashMapBucketIterator<N2, R2> const& rhs) noexcept
{
    return lhs.node_ == rhs.node_;
}

template<typename N1, typename N2, typename R1, typename R2> constexpr
bool operator!=(HashMapBucketIterator<N1, R1> const& lhs, HashMapBucketIterator<N2, R2> const& rhs) noexcept
{
    return lhs.node_ != rhs.node_;
}

#endif // DATA_STRUCTURES_HASH_MAP_BUCKET_HPP
