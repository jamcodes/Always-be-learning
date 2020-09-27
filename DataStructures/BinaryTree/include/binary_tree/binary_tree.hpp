#ifndef DATA_STRUCTURES_BINARY_TREE_HPP
#define DATA_STRUCTURES_BINARY_TREE_HPP

#include <cstdint>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <type_traits>

template<typename BT> class BT_Iterator;
template<typename BT> class BT_ConstIterator;

template <typename Key, typename T, typename Allocator = std::allocator<std::pair<const Key, T>>>
class BinaryTree {
    using alloc_traits = std::allocator_traits<Allocator>;
    using self = BinaryTree<Key, T, Allocator>;
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type = Allocator;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using iterator = BT_Iterator<self>;
    using const_iterator = BT_ConstIterator<self>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    BinaryTree();
    explicit BinaryTree(Allocator const& = Allocator{});
    template<typename InputIt>
    explicit BinaryTree(InputIt first, InputIt last, Allocator const& alloc = Allocator{});
    BinaryTree(BinaryTree const& other);
    BinaryTree(BinaryTree const& other, Allocator const& alloc);
    BinaryTree(BinaryTree&& other);
    BinaryTree(BinaryTree&& other, Allocator const& alloc);
    BinaryTree& operator=(BinaryTree const&);
    BinaryTree& operator=(BinaryTree&&) noexcept;
    ~BinaryTree() noexcept;

    // allocator access
    allocator_type get_allocator() const noexcept;

    // element access
    reference at(Key const& key);
    const_reference at(Key const& key) const;
    reference operator[](Key const& key);
    reference operator[](Key&& key);

    // iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // capacity
    bool empty() const noexcept;
    size_type size() const noexcept;

    // modifiers
    void clear() noexcept;
    
    std::pair<iterator, bool> insert(value_type const& value);
    template<typename P /* , typename = SFINAE if P is convertible to value_Type */>
    std::pair<iterator, bool> insert(P&& value);
    std::pair<iterator, bool> insert(value_type&& value);
    iterator insert(const_iterator hint, value_type const& value);
    iterator insert(const_iterator hint, value_type&& value);
    template<typename P /* , typename = SFINAE if P is convertible to value_type */>
    iterator insert(const_iterator, P&& value);
    template<typename InputIt>
    void insert(InputIt first, InputIt last);

    template<typename M /* , typename = SFINAE if M is convertible to mapped_type */>
    std::pair<iterator, bool> insert_or_assign(key_type const& key, M&& value);
    template<typename M /* , typename = SFINAE if M is convertible to mapped_type */>
    std::pair<iterator, bool> insert_or_assign(key_type&& key, M&& value);
    template<typename M /* , typename = SFINAE if M is convertible to mapped_type */>
    iterator insert_or_assign(const_iterator hint, key_type const& key, M&& value);
    template<typename M /* , typename = SFINAE if M is convertible to mapped_type */>
    iterator insert_or_assign(const_iterator hint, key_type&& key, M&& value);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);

    template<typename... Args>
    std::pair<iterator, bool> emplace_hint(const_iterator hint, Args&&... args);

    template<typename... Args>
    std::pair<iterator, bool> try_emplace(key_type const& key, Args&&... args);
    template<typename... Args>
    std::pair<iterator, bool> try_emplace(key_type&& key, Args&&... args);
    template<typename... Args>
    iterator try_emplace(const_iterator hint, key_type const& key, Args&&... args);
    template<typename... Args>
    iterator try_emplace(const_iterator hint, key_type&& key, Args&&... args);

    iterator erase(const_iterator pos);
    iterator erase(iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(key_type const& key);

    void swap(BinaryTree& other) noexcept;

    // lookup
    size_type count(Key const& key);
    template<typename K>
    size_type count(K const& key);

    iterator find(key_type const& key) noexcept;
    const_iterator find(key_type const& key) const noexcept;
    template<typename K> iterator find(K const& key) noexcept;
    template<typename K> const_iterator find(K const& key) const noexcept;

    bool contains(Key const& key) const noexcept;
    template<typename K> bool contains(K const& key) const noexcept;

    std::pair<iterator, iterator> equal_range(Key const& key) noexcept;
    std::pair<const_iterator, const_iterator> equal_range(Key const& key) const noexcept;
    template<typename K>
    std::pair<iterator, iterator> equal_range(K const& key) noexcept;
    template<typename K>
    std::pair<const_iterator, const_iterator> equal_range(K const& key) const noexcept;

    iterator lower_bound(Key const& key) noexcept;
    const_iterator lower_bound(Key const& key) const noexcept;
    template<typename K>
    iterator lower_bound(K const& key) noexcept;
    template<typename K>
    const_iterator lower_bound(K const& key) const noexcept;

    iterator upper_bound(Key const& key) noexcept;
    const_iterator upper_bound(Key const& key) const noexcept;
    template<typename K>
    iterator upper_bound(K const& key) noexcept;
    template<typename K>
    const_iterator upper_bound(K const& key) const noexcept;

};

#endif  // DATA_STRUCTURES_BINARY_TREE_HPP
