#ifndef DATA_STRUCTURES_SINGLE_LINKED_LIST_HPP
#define DATA_STRUCTURES_SINGLE_LINKED_LIST_HPP

#include <utils/Assertion.h>
#include <utils/traits.h>

#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template<typename T, bool IsConst> class SLL_IteratorImpl;
template<typename T> using SLL_Iterator = SLL_IteratorImpl<T, false>;
template<typename T> using SLL_ConstIterator = SLL_IteratorImpl<T, true>;

// TODO: Split this into non-template base and templated class
template <typename T>
struct SLL_Node {
    T data;
    SLL_Node* next{nullptr};

    template <typename... Args>
    explicit SLL_Node(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
        : data(std::forward<Args>(args)...)
    {
    }
    // We do not own the `next` node
    ~SLL_Node() noexcept = default;

    SLL_Node(SLL_Node const&) noexcept(std::is_nothrow_copy_constructible_v<T>) = default;
    SLL_Node(SLL_Node&& other) noexcept(std::is_nothrow_move_constructible_v<T>) = default;
    SLL_Node& operator=(SLL_Node const&) noexcept(std::is_nothrow_copy_assignable_v<T>) = default;
    SLL_Node& operator=(SLL_Node&& other) noexcept(std::is_nothrow_move_constructible_v<T>) = default;
};


template <class T, class Allocator = std::allocator<T>>
class SingleLinkedList {
private:
    using Node = SLL_Node<T>;
    using self = SingleLinkedList<T, Allocator>;
    using alloc_traits = std::allocator_traits<Allocator>;
    using Nalloc = typename alloc_traits::template rebind_alloc<Node>;
    using nalloc_traits = std::allocator_traits<Nalloc>;
    using SwapAllocators = SwapAllocators<typename nalloc_traits::propagate_on_container_swap>;

    Nalloc nalloc_{};
    Node head_{};
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<const value_type>;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using iterator = SLL_Iterator<T>;
    using const_iterator = SLL_ConstIterator<T>;

    SingleLinkedList() noexcept(std::is_nothrow_constructible_v<Node>) = default;
    explicit SingleLinkedList(size_type count, value_type const& value, Allocator const& alloc = Allocator{});
    explicit SingleLinkedList(size_type count, Allocator const& alloc = Allocator{});
    template <typename InputIt, typename = RequiresInputIterator<InputIt>>
    explicit SingleLinkedList(InputIt first, InputIt last, Allocator const& alloc = Allocator{});
    explicit SingleLinkedList(std::initializer_list<value_type> il);
    SingleLinkedList(SingleLinkedList const& other);
    SingleLinkedList(SingleLinkedList const& other, Allocator const& alloc);
    SingleLinkedList(SingleLinkedList&& other) noexcept;
    SingleLinkedList(SingleLinkedList&& other, Allocator const& alloc) noexcept;
    ~SingleLinkedList() noexcept { free(); }

    SingleLinkedList& operator=(SingleLinkedList const& other);
    SingleLinkedList& operator=(SingleLinkedList&& other) noexcept;

    // assignment
    void assign(size_type count, value_type const& value);
    template <typename InputIt>
    void assign(InputIt first, InputIt last);
    // void assign(std::initializer_list<T> il);

    // allocator access
    allocator_type get_allocator() const noexcept;

    // element access
    reference front() noexcept {
        JAM_EXPECT(head_.next != nullptr, "front() called on an empty SingleLinkedList");
        return head_.next->data;
    }
    const_reference front() const noexcept
    {
        JAM_EXPECT(head_.next != nullptr, "front() called on an empty SingleLinkedList");
        return head_.next->data;
    }

    // capacity
    bool empty() const noexcept;

    // iterators
    iterator before_begin() noexcept { return iterator{&head_}; }
    const_iterator before_begin() const noexcept { return const_iterator{&head_}; }
    const_iterator before_cbegin() const noexcept { return const_iterator{&head_}; }

    iterator begin() noexcept { return iterator{head_.next}; }
    const_iterator begin() const noexcept { return const_iterator{head_.next}; }
    const_iterator cbegin() const noexcept { return const_iterator{head_.next}; }

    iterator end() noexcept { return iterator{nullptr}; }
    const_iterator end() const noexcept { return const_iterator{nullptr}; }
    const_iterator cend() const noexcept { return const_iterator{nullptr}; }

    // modifiers
    void clear() noexcept { free(); }
    iterator insert_after(const_iterator pos, T const& value);
    iterator insert_after(const_iterator pos, T&& value);
    iterator insert_after(const_iterator pos, size_type count, T const& value);
    template <class InputIt, typename = RequiresInputIterator<InputIt>>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);

    template <typename... Args>
    iterator emplace_after(const_iterator pos, Args&&... args);

    iterator erase_after(const_iterator pos);
    iterator erase_after(const_iterator first, const_iterator last);

    void push_front(T const& value);
    void push_front(T&& value);

    template <typename... Args>
    reference emplace_front(Args&&... args);

    void pop_front();

    void resize(size_type count);
    void resize(size_type count, T const& value);

    void swap(SingleLinkedList& other) noexcept;

    template<typename U, typename Alloc>
    friend void swap(SingleLinkedList<U, Alloc>& lhs, SingleLinkedList<U, Alloc>& rhs) noexcept;

    // operations
    void merge(SingleLinkedList& other);
    void merge(SingleLinkedList&& other);
    template<typename Compare> void merge(SingleLinkedList& other, Compare cmp);
    template<typename Compare> void merge(SingleLinkedList&& other, Compare cmp);

    size_type remove(T const& value);
    template<typename UnaryPredicate> size_type remove_if(UnaryPredicate pred);

    void reverse() noexcept;

    size_type unique();
    template<typename BinaryPredicate> size_type unique(BinaryPredicate pred);

    void sort();
    template<typename Compare> void sort(Compare cmp);

private:
    template<typename... Args>
    Node* make_node(Args&&... args)
    {
        Node* nn{nalloc_traits::allocate(nalloc_, 1)};
        nalloc_traits::construct(nalloc_, nn, std::forward<Args>(args)...);
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
            link_nodes(nn, next_node);
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
            link_nodes(nn, next_node);
            nn = next_node;
        }
        return {front, nn};
    }

    static constexpr inline void link_nodes(Node* const lhs, Node* const rhs) noexcept
    {
        lhs->next = rhs;
    }

    inline void free(Node* node) noexcept
    {
        nalloc_traits::destroy(nalloc_, node);
        nalloc_traits::deallocate(nalloc_, node, 1);
    }

    inline void free() noexcept
    {
        while (head_.next != nullptr)
        {
            Node* temp{head_.next};
            head_.next = temp->next;
            nalloc_traits::destroy(nalloc_, temp);
            nalloc_traits::deallocate(nalloc_, temp, 1);
        }
    }
};


template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(size_type count, value_type const& value, Allocator const& alloc)
    : nalloc_{Nalloc{alloc}}
{
    auto nodes{make_n(count, value)};
    link_nodes(&head_, nodes.first);
}

template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(size_type count, Allocator const& alloc)
    : nalloc_{Nalloc{alloc}}
{
    auto nodes{make_n(count)};
    link_nodes(&head_, nodes.first);
}

template<typename T, typename Allocator>
template <typename InputIt, typename>
SingleLinkedList<T, Allocator>::SingleLinkedList(InputIt first, InputIt last, Allocator const& alloc)
    : nalloc_{Nalloc{alloc}}
{
    auto nodes{make_range(first, last)};
    link_nodes(&head_, nodes.first);
}

template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(std::initializer_list<value_type> il)
{
    auto nodes{make_range(il.begin(), il.end())};
    link_nodes(&head_, nodes.first);
}

template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(SingleLinkedList const& other)
    : nalloc_{nalloc_traits::select_on_container_copy_construction(other.nalloc_)}
{
    auto nodes{make_range(other.cbegin(), other.cend())};
    link_nodes(&head_, nodes.first);
}
template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(SingleLinkedList const& other, Allocator const& alloc)
    : nalloc_{Nalloc{alloc}}
{
    auto nodes{make_range(other.cbegin(), other.cend())};
    link_nodes(&head_, nodes.first);
}
template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(SingleLinkedList&& other) noexcept
    : nalloc_{std::move(other.nalloc_)}
{
    head_.next = std::exchange(other.head_.next, nullptr);
}
template<typename T, typename Allocator>
SingleLinkedList<T, Allocator>::SingleLinkedList(SingleLinkedList&& other, Allocator const& alloc) noexcept
    : nalloc_{Nalloc{alloc}}
{
    // Need to move node-by-node, since the allocator is different?
    auto nodes(make_range(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end())));
    link_nodes(&head_, nodes.first);
}

template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::operator=(SingleLinkedList const& other) -> SingleLinkedList&
{
    if (this != &other) {
        free();
        auto nodes{[this, &other]() -> std::pair<Node*, Node*> {
            if (!other.empty()) {
                return make_range(other.cbegin(), other.cend());
            }
                return {nullptr, nullptr};
            }()
        };
        link_nodes(&head_, nodes.first);
    }
    return *this;
}

template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::operator=(SingleLinkedList&& other) noexcept -> SingleLinkedList&
{
    if (this != &other) {
        free();
        head_.next = std::exchange(other.head_.next, nullptr);
    }
    return *this;
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::swap(SingleLinkedList& other) noexcept
{
    // always swap the data
    using std::swap;
    swap(head_.next, other.head_.next);
    // swap allocators if nalloc_traits::propagate_on_container_swap == std::true_type,
    // if this is false and allocators for this and other don't compare equal, swapping is UB
    SwapAllocators{}(nalloc_, other.nalloc_);
}

template<typename T, typename Allocator>
void swap(SingleLinkedList<T, Allocator>& lhs, SingleLinkedList<T, Allocator>& rhs) noexcept
{
    lhs.swap(rhs);
}


// capacity
template<typename T, typename Allocator>
bool SingleLinkedList<T, Allocator>::empty() const noexcept
{
    return head_.next == nullptr;
}

template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::insert_after(const_iterator pos, T const& value) -> iterator
{
    JAM_EXPECT(pos != cend(), "Can't insert after end iterator");
    Node* nn{make_node(value)};
    // STL uses a const_cast here as well...
    // TODO: consider making the `next` member mutable?
    link_nodes(nn, pos.node_->next);
    link_nodes(const_cast<Node*>(pos.node_), nn);
    return iterator{nn};
}
template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::insert_after(const_iterator pos, T&& value) -> iterator
{
    JAM_EXPECT(pos != cend(), "Can't insert after end iterator");
    Node* nn{make_node(std::move(value))};
    link_nodes(nn, pos.node_->next);
    link_nodes(const_cast<Node*>(pos.node_), nn);
    return iterator{nn};
}
template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::insert_after(const_iterator pos, size_type count, T const& value) -> iterator
{
    JAM_EXPECT(pos != cend(), "Can't insert after end iterator");
    if (count == 0) {
        return iterator(const_cast<Node*>(pos.node_));
    }
    auto nodes{make_n(count, value)};
    link_nodes(nodes.second, pos.node_->next);
    link_nodes(const_cast<Node*>(pos.node_), nodes.first);
    return iterator{nodes.second};
}
template<typename T, typename Allocator>
template <class InputIt, typename>
auto SingleLinkedList<T, Allocator>::insert_after(const_iterator pos, InputIt first, InputIt last) -> iterator
{
    JAM_EXPECT(pos != cend(), "Can't insert after end iterator");
    if (first == last) {
        return iterator{const_cast<Node*>(pos.node_)};
    }
    auto nodes{make_range(first, last)};
    link_nodes(nodes.second, pos.node_->next);
    link_nodes(const_cast<Node*>(pos.node_), nodes.first);
    return iterator{nodes.second};
}

template<typename T, typename Allocator>
template <typename... Args>
auto SingleLinkedList<T, Allocator>::emplace_after(const_iterator pos, Args&&... args) -> iterator
{
    JAM_EXPECT(pos != cend(), "Can't insert after end iterator");
    auto nn{make_node(std::forward<Args>(args)...)};
    link_nodes(nn, pos.node_->next);
    link_nodes(const_cast<Node*>(pos.node_), nn);
    return iterator{nn};
}

template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::erase_after(const_iterator pos) -> iterator
{
    if (pos == cend()) { return end(); }
    auto p{const_cast<Node*>(pos.node_)};
    if (p->next) {
        auto pn{p->next};
        link_nodes(p, pn->next);
        free(pn);
        return iterator{p->next};
    } else {
        return end();
    }
}
template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::erase_after(const_iterator first, const_iterator last) -> iterator
{
    auto b{const_cast<Node*>(first.node_)};
    auto e{const_cast<Node*>(last.node_)};
    if (b == e) { return iterator{e}; }
    auto p{b->next};
    link_nodes(b, e);
    while (p != e) {
        auto tmp{p};
        p = p->next;
        free(tmp);
    }
    return iterator{p};
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::push_front(T const& value)
{
    auto nn{make_node(value)};
    link_nodes(nn, head_.next);
    link_nodes(&head_, nn);
}
template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::push_front(T&& value)
{
    auto nn{make_node(std::move(value))};
    link_nodes(nn, head_.next);
    link_nodes(&head_, nn);
}

template<typename T, typename Allocator>
template <typename... Args>
auto SingleLinkedList<T, Allocator>::emplace_front(Args&&... args) -> reference
{
    auto nn{make_node(std::forward<Args>(args)...)};
    link_nodes(nn, head_.next);
    link_nodes(&head_, nn);
    return nn->data;
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::pop_front()
{
    if (head_.next) {
        auto tmp{head_.next};
        link_nodes(&head_, tmp->next);
        free(tmp);
    }
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::resize(size_type count)
{
    resize(count, T{});
}
template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::resize(size_type count, T const& value)
{
    // iterate until either the end() or `count'th` element is reached
    auto before_it{before_cbegin()};
    auto it{cbegin()};
    while (count && it != cend()) {
        ++it;
        ++before_it;
        --count;
    }
    if (count != 0 && it == cend()) {
        insert_after(before_it, count, value);
    }
    else if (count == 0 && it != cend()) {
        erase_after(before_it, cend());
    }
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::merge(SingleLinkedList& other)
{
    merge(std::move(other));
}
template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::merge(SingleLinkedList&& other)
{
    merge(std::move(other), [](auto const& lhs, auto const& rhs) noexcept { return lhs < rhs; });
}
template<typename T, typename Allocator>
template<typename Compare> void SingleLinkedList<T, Allocator>::merge(SingleLinkedList& other, Compare cmp)
{
    merge(std::move(other), cmp);
}
template<typename T, typename Allocator>
template<typename Compare> void SingleLinkedList<T, Allocator>::merge(SingleLinkedList&& other, Compare cmp)
{
    auto lbb{&head_};
    auto lb{head_.next};
    auto rbb{&other.head_};
    auto rb{other.head_.next};
    while (rb != nullptr) {
        while (lb != nullptr && !cmp(rb->data, lb->data)) {
            lbb = lb;
            lb = lb->next;
        }
        auto rend{rb->next};
        while (rend != nullptr && (lb == nullptr || !cmp(lb->data, rend->data))) {
            rend = rend->next;
        }
        lbb->next = rb;
        rbb->next = rend;
        rb->next = lb;
        rb = rend;
    }
}

template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::remove(T const& value) -> size_type
{
    return remove_if([&value](auto const& elem) noexcept { return elem == value; });
}
template<typename T, typename Allocator>
template<typename UnaryPredicate>
auto SingleLinkedList<T, Allocator>::remove_if(UnaryPredicate pred) -> size_type
{
    Node* bbegin{&head_};
    Node* begin{head_.next};
    size_type count{0};
    while (begin != nullptr) {
        if (pred(begin->data)) {
            bbegin->next = begin->next;
            free(begin);
            begin = bbegin->next;
            ++count;
        } else {
            bbegin = begin;
            begin = begin->next;
        }
    }
    return count;
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::reverse() noexcept
{
    Node* begin{head_.next};
    head_.next = nullptr;
    while (begin != nullptr)
    {
        auto next = begin->next;
        begin->next = head_.next;
        head_.next = begin;
        begin = next;
    }
    
}

template<typename T, typename Allocator>
auto SingleLinkedList<T, Allocator>::unique() -> size_type
{
    return unique([](auto const& lhs, auto const& rhs)noexcept{return lhs == rhs;});
}

template<typename T, typename Allocator>
template<typename BinaryPredicate>
auto SingleLinkedList<T, Allocator>::unique(BinaryPredicate pred) -> size_type
{
    auto begin{head_.next};
    size_type count{0};
    while (begin != nullptr && begin->next != nullptr) {
        auto next = begin->next;
        while (next != nullptr && pred(begin->data, next->data)) {
            begin->next = next->next;
            free(next);
            next = begin->next;
            ++count;
        }
        begin = next;
    }
    return count;
}

template<typename T, typename Allocator>
void SingleLinkedList<T, Allocator>::sort()
{
    sort([](auto const& lhs, auto const& rhs)noexcept { return lhs < rhs; });
}
template<typename T, typename Allocator>
template<typename Compare>
void SingleLinkedList<T, Allocator>::sort(Compare cmp)
{
    auto const swap_nodes = [](Node* blhs, Node* lhs, Node* rhs) noexcept {
        blhs->next = rhs;
        lhs->next = rhs->next;
        rhs->next = lhs;
    };

    if (head_.next == nullptr) return;

    bool sorted{false};
    while (!sorted) {
        sorted = true;
        Node* blhs{&head_};
        Node* lhs{head_.next};
        while (lhs->next != nullptr) {
            if (cmp(lhs->next->data, lhs->data)) {
                swap_nodes(blhs, lhs, lhs->next);
                sorted = false;
            } else {
                lhs = lhs->next;
            }
            blhs = blhs->next;
        }
    }
}

// Iterators
template<typename T, bool IsConst> class SLL_IteratorImpl
{
    using Container = SingleLinkedList<T>;
    using Node = std::conditional_t<IsConst, const SLL_Node<T>, SLL_Node<T>>;
    using self = SLL_IteratorImpl<T, IsConst>;
    friend SingleLinkedList<T>;

    Node* node_{nullptr};
public:
    using value_type = std::conditional_t<IsConst, const T, T>;
    using reference = std::conditional_t<IsConst, typename Container::const_reference, typename Container::reference>;
    using pointer = std::conditional_t<IsConst, typename Container::const_pointer, typename Container::pointer>;
    using difference_type = typename Container::difference_type;
    using iterator_category = std::forward_iterator_tag;
    using const_iterator = SLL_IteratorImpl<T, true>;

    explicit SLL_IteratorImpl(Node* node) noexcept
        : node_{node} {}

    constexpr pointer operator->() const noexcept { return &(node_->data); }

    constexpr reference operator*() const noexcept { return node_->data; }

    constexpr self& operator++() noexcept
    {
        node_ = node_->next;
        return *this;
    }

    constexpr self operator++(int) noexcept
    {
        auto result{*this};
        node_ = node_->next;
        return result;
    }

    constexpr operator const_iterator() const noexcept { return const_iterator{node_}; }

    constexpr bool operator==(self const& other) const noexcept { return node_ == other.node_; }
    constexpr bool operator!=(self const& other) const noexcept { return node_ != other.node_; }

    template<typename U, bool Const> constexpr friend
    bool operator==(SLL_IteratorImpl<U, Const> const& lhs, SLL_IteratorImpl<U, !Const> const& rhs) noexcept;
    template<typename U, bool Const> constexpr friend
    bool operator!=(SLL_IteratorImpl<U, Const> const& lhs, SLL_IteratorImpl<U, !Const> const& rhs) noexcept;
};

template<typename U, bool Const> constexpr
bool operator==(SLL_IteratorImpl<U, Const> const& lhs, SLL_IteratorImpl<U, !Const> const& rhs) noexcept
{
    return lhs.node_ == rhs.node_;
}
template<typename U, bool Const> constexpr
bool operator!=(SLL_IteratorImpl<U, Const> const& lhs, SLL_IteratorImpl<U, !Const> const& rhs) noexcept
{
    return lhs.node_ != rhs.node_;
}


#endif  // DATA_STRUCTURES_SINGLE_LINKED_LIST_HPP
