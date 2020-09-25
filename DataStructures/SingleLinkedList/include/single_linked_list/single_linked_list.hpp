#ifndef DATA_STRUCTURES_SINGLE_LINKED_LIST_HPP
#define DATA_STRUCTURES_SINGLE_LINKED_LIST_HPP

#include <utils/Assertion.h>

#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
struct SLL_Node {
    T data;
    SLL_Node* next{nullptr};

    template <typename... Args>
    explicit SLL_Node(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
        : data{std::forward<Args>(args)}
    {
    }
    // We do not own the `next` node
    ~SLL_Node() noexcept = default;

    SLL_Node(SLL_Node const&) = default;
    SLL_Node(SLL_Node&& other) noexcept = default;
    SLL_Node& operator=(SLL_Node const&) = default;
    SLL_Node& operator=(SLL_Node&& other) noexcept = default;

    // SLL_Node(SLL_Node const&) = delete;
    // SLL_Node(SLL_Node&& other) noexcept
    //     : data{std::move(other.data)}, next{std::exchange(other.next, nullptr)}
    // {
    // }

    // SLL_Node& operator=(SLL_Node const&) = delete;
    // SLL_Node& operator=(SLL_Node&& other) noexcept
    // {
    //     data = std::move(other.data);
    //     next = std::exchange(other.next, nullptr);
    // }
};

template <typename T>
class SLL_Iterator;
template <typename T>
class SLL_ConstIterator;

template <class T, class Allocator = std::allocator<T>>
class SingleLinkedList {
private:
    using alloc_traits = typename std::allocator_traits<Allocator>;
    using node_allocator = typename alloc_traits::rebind_alloc<SLL_Node<T>>;
    SLL_Node<T>* head_;

public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<const value_type>;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using iterator = typename SLL_Iterator<T>;
    using const_iterator = typename SLL_ConstIterator<T>;

    SingleLinkedList() noexcept = default;
    explicit SingleLinkedList(size_type count, value_type const& value, Allocator const& alloc = Allocator{});
    explicit SingleLinkedList(size_type count, Allocator const& alloc = Allocator{});
    template <typename InputIt>
    explicit SingleLinkedList(InputIt first, InputIt last, Allocator const& alloc = Allocator{});
    SingleLinkedList(SingleLinkedList const& other);
    SingleLinkedList(SingleLinkedList const& other, Allocator const& alloc);
    SingleLinkedList(SingleLinkedList&& other) noexcept;
    SingleLinkedList(SingleLinkedList&& other, Allocator const& alloc) noexcept;
    ~SingleLinkedList() noexcept;

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
    reference front() noexcept;
    const_reference front() const noexcept;

    // capacity
    bool empty() const noexcept;

    // iterators
    iterator before_begin() noexcept;
    const_iterator before_begin() const noexcept;
    const_iterator before_cbegin() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // modifiers
    void clear() noexcept;
    iterator insert_after(const_iterator pos, T const& value);
    iterator insert_after(const_iterator pos, T&& value);
    iterator insert_after(const_iterator pos, size_type count, T const& value);
    template <class InputIt>
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

    // operations
    void merge(SingleLinkedList& other);
    void merge(SingleLinkedList&& other);
    template<typename Compare> void merge(forward_list& other, Compare cmp);
    template<typename Compare> void merge(forward_list&& other, Compare cmp);

    size_type remove(T const& value);
    template<typename UnaryPredicate> size_type remove_if(UnaryPredicate pred);

    void reverse() noexcept;

    size_type unique();
    template<typename BinaryPredicate> size_type unique();

    void sort();
    template<typename Compare> void sort(Compare cmp);
};

#endif  // DATA_STRUCTURES_SINGLE_LINKED_LIST_HPP
