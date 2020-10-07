#ifndef DATA_STRUCTURES_BINARY_TREE_HPP
#define DATA_STRUCTURES_BINARY_TREE_HPP

#include <utils/Assertion.h>
#include <utils/traits.h>

#include <map>

#include <cstdint>
#include <cstddef>
#include <stack>
#include <iterator>
#include <memory>
#include <utility>
#include <type_traits>

template<typename NodePointer, typename Reference> class BinaryTree_iterator_base;
template<typename,typename,typename> class BinaryTree_iterator;

struct BT_Iterator_Preorder_Tag {};
struct BT_Iterator_Postorder_Tag {};
struct BT_Iterator_Inorder_Tag {};

template<typename Key, typename T>
struct BT_Node {
    using Node = BT_Node<Key, T>;
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;

    template<typename... Args>
    explicit BT_Node(Args&&... args) noexcept(std::is_nothrow_constructible_v<value_type, Args&&...>)
        : data{std::forward<Args>(args)...} { }

    Node* left{nullptr};
    Node* right{nullptr};
    value_type data;
};

template <typename Key, typename T, typename Allocator = std::allocator<std::pair<const Key, T>>>
class BinaryTree {
    using Node = BT_Node<Key, T>;
    using alloc_traits = std::allocator_traits<Allocator>;
    using Nalloc = typename alloc_traits::template rebind_alloc<Node>;
    using nalloc_traits = std::allocator_traits<Nalloc>;
    using SwapAlloctors = SwapAllocators<typename nalloc_traits::propagate_on_container_swap>;
    using self = BinaryTree<Key, T, Allocator>;

    Nalloc alloc_{Nalloc{}};
    Node* root_{nullptr};
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
    using iterator = BinaryTree_iterator<Node, reference, BT_Iterator_Inorder_Tag>;
    using const_iterator = BinaryTree_iterator<Node, const_reference, BT_Iterator_Inorder_Tag>;
    using reverse_iterator = BinaryTree_iterator<Node, reference, BT_Iterator_Postorder_Tag>;
    using const_reverse_iterator = BinaryTree_iterator<Node, const_reference, BT_Iterator_Postorder_Tag>;


    explicit BinaryTree(Allocator const& = Allocator{});
    template<typename InputIt>
    explicit BinaryTree(InputIt first, InputIt last, Allocator const& alloc = Allocator{});
    BinaryTree(BinaryTree const& other);
    BinaryTree(BinaryTree const& other, Allocator const& alloc);
    BinaryTree(BinaryTree&& other);
    BinaryTree(BinaryTree&& other, Allocator const& alloc);
    BinaryTree& operator=(BinaryTree const&);
    BinaryTree& operator=(BinaryTree&&) noexcept;
    ~BinaryTree() noexcept { free(); }

    void swap(BinaryTree& other) noexcept;

    // allocator access
    allocator_type get_allocator() const noexcept { return alloc_; }

    // element access
    reference at(Key const& key);
    const_reference at(Key const& key) const;
    reference operator[](Key const& key);
    reference operator[](Key&& key);

    // iterators
    iterator begin() noexcept { return iterator{root_}; }
    const_iterator begin() const noexcept { return const_iterator{root_}; }
    const_iterator cbegin() const noexcept { return const_iterator{root_}; }
    iterator end() noexcept { return iterator{}; }
    const_iterator end() const noexcept { return const_iterator{}; }
    const_iterator cend() const noexcept { return const_iterator{}; }

    reverse_iterator rbegin() noexcept { return reverse_iterator{root_}; }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{root_}; }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{root_}; }
    reverse_iterator rend() noexcept { return reverse_iterator{}; }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator{}; }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator{}; }

    // capacity
    bool empty() const noexcept { return root_ == nullptr; }
    size_type size() const noexcept {
        size_type count{0};
        auto const counter{[&count](Node*) noexcept { ++count; }};
        traverse_preorder(root_, counter);
        return count;
    }

    // modifiers
    void clear() noexcept { free(); }

    std::pair<iterator, bool> insert(value_type const& value);
    template<typename P, typename = std::enable_if_t<std::is_convertible_v<P&&, value_type>>>
    std::pair<iterator, bool> insert(P&& value);
    std::pair<iterator, bool> insert(value_type&& value);
    iterator insert(const_iterator hint, value_type const& value);
    iterator insert(const_iterator hint, value_type&& value);
    template<typename P, typename = std::enable_if_t<std::is_convertible_v<P&&, value_type>>>
    iterator insert(const_iterator, P&& value);
    template<typename InputIt>
    void insert(InputIt first, InputIt last);

    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    std::pair<iterator, bool> insert_or_assign(key_type const& key, M&& value);
    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    std::pair<iterator, bool> insert_or_assign(key_type&& key, M&& value);
    // The `hint` interface is not consistent with the standard implementation - the hint here is expected to point
    // to the element just before the correct place to insert the new key-value pair. The STL interface expects
    // the iterator to point to an element just AFTER the best place to insert the new pair.
    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
    iterator insert_or_assign(const_iterator hint, key_type const& key, M&& value);
    template<typename M, typename = std::enable_if_t<std::is_convertible_v<M&&, mapped_type>>>
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

    // lookup
    size_type count(Key const& key)
    {
        return count_impl(key);
    }
    template<typename K, typename = std::enable_if_t<std::is_convertible_v<K, key_type>>>
    size_type count(K const& key)
    {
        return count_impl(key);
    }

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

private:
    void free(Node* node) noexcept
    {
        nalloc_traits::destroy(alloc_, node);
        nalloc_traits::deallocate(alloc_, node, 1);
    }

    void free_traverse(Node* node) noexcept
    {
        if (node) {
            free_traverse(node->left);
            free_traverse(node->right);
            free(node);
        }
    }

    void free() noexcept
    {
        free_traverse(root_);
    }

    template<typename... Args>
    Node* make_node(Args&&... args)
    {
        auto* const nn{nalloc_traits::allocate(alloc_, 1)};
        nalloc_traits::construct(alloc_, nn, std::forward<Args>(args)...);
        return nn;
    }

    template<typename InputIt>
    Node* make_range(InputIt first, InputIt last)
    {
        JAM_ASSERT(first != last, "range must contain at least one element");
        auto* root = make_node(*first);
        while (++first != last) {
            auto n = make_node(*first);
            insert_at(root, n);
        }
        return root;
    }

    std::pair<iterator, bool> insert_node(Node* node);

    template<typename K>
    size_type count_impl(K const& key)
    {
        size_type count{0};
        traverse_preorder_recursive(root_, [&count, &key](Node* n){
                if (n->data.first == key) { ++count; }
            }
        );
        return count;
    }

    static std::pair<Node**, Node*> find_node_with_link(Node* root, key_type const& key) noexcept;
    std::pair<Node**, Node*> find_node_with_link(key_type const& key) noexcept;

    static std::pair<Node*, bool> insert_at(Node* root, Node* n) noexcept
    {
        JAM_ASSERT(root != nullptr, "null root");
        JAM_ASSERT(n != nullptr, "null node");
        if (n->data < root->data) {
            if (root->left == nullptr) {
                root->left = n;
                return {root->left, true};
            }
            else {
                return insert_at(root->left, n);
            }
        }
        else if (n->data > root->data) {
            if (root->right == nullptr) {
                root->right = n;
                return {root->right, true};
            }
            else {
                return insert_at(root->right, n);
            }
        }
        else {
            return {nullptr, false};
        }
    }

    iterator erase_leaf(Node** link, Node* node);
    iterator erase_semibranch(Node** link, Node* node);
    iterator erase_branch(Node** link, Node* node);

    template<typename KeyType, typename ValueType>
    std::pair<Node*, bool> insert_or_assign_impl(Node* root, KeyType&& key, ValueType&& value)
    {
        auto link_node{find_node_with_link(root, key)};
        if (link_node.second != nullptr) {
            link_node.second->data.second = std::forward<ValueType>(value);
            return {link_node.second, false};
        }
        else {
            Node* nn{make_node(std::forward<KeyType>(key), std::forward<ValueType>(value))};
            *link_node.first = nn;
            return {*link_node.first, true};
        }
    }

    template<typename KeyType, typename... Args>
    std::pair<Node*, bool> try_emplace_impl(Node* root, KeyType&& key, Args&&... args)
    {
        auto link_node{find_node_with_link(root, key)};
        if (link_node.second == nullptr) {
            Node* nn{make_node(std::forward<KeyType>(key), std::forward<Args>(args)...)};
            *link_node.first = nn;
            return {*link_node.first, true};
        }
        else {
            return {link_node.second, false};
        }
    }

    template<typename Func>
    static void traverse_preorder_recursive(Node* root, Func func) noexcept(noexcept(func(root)))
    {
        if (root) {
            func(root);
            traverse_preorder_recursive(root->left, func);
            traverse_preorder_recursive(root->right, func);
        }
    }

    template<typename Func>
    static void traverse_inorder_recursive(Node* root, Func func) noexcept(noexcept(func(root)))
    {
        if (root) {
            traverse_preorder_recursive(root->left, func);
            func(root);
            traverse_preorder_recursive(root->right, func);
        }
    }

    template<typename Func>
    static void traverse_postorder_recursive(Node* root, Func func) noexcept(noexcept(func(root)))
    {
        if (root) {
            traverse_preorder_recursive(root->right, func);
            traverse_preorder_recursive(root->left, func);
            func(root);
        }
    }

    template<typename Func>
    static void traverse_preorder(Node* root, Func func) noexcept(noexcept(func(root)))
    {
        std::stack<Node*> nodes{};
        for (auto* node{root}; node != nullptr; /* */)
        {
            func(node);
            if (node->right) { nodes.push(node->right); }
            node = node->left;
            if (node == nullptr && !nodes.empty()) {
                node = nodes.top();
                nodes.pop();
            }
        }
    }

    template<typename Func>
    static void traverse_inorder(Node* root, Func func) noexcept(noexcept(func(root)))
    {
        std::stack<Node*> nodes{};
        for (auto* node{root}; node != nullptr; /*  */)
        {
            nodes.push(node);
            node = node->left;
            while (node == nullptr && !nodes.empty()) {
                node = nodes.top();
                nodes.pop();
                func(node);
                node = node->right;
                // if (node) { break; }
            }
        }
    }

    template<typename Func>
    static void traverse_postorder(Node* root, Func func) noexcept(noexcept(func(root)))
    {
        std::stack<Node*> nodes{};
        for (auto* node{root}; node != nullptr; /*  */)
        {
            nodes.push(node);
            node = node->right;
            while (node == nullptr && nodes.empty()) {
                node = nodes.top();
                nodes.pop();
                func(node);
                node = node->left;
            }
        }
    }
};


template <typename Key, typename T, typename Allocator>
BinaryTree<Key, T, Allocator>::BinaryTree(Allocator const& alloc)
    : alloc_{Nalloc{alloc}}
{
}

template <typename Key, typename T, typename Allocator>
template<typename InputIt>
BinaryTree<Key, T, Allocator>::BinaryTree(InputIt first, InputIt last, Allocator const& alloc)
    : alloc_{Nalloc{alloc}}
{
    if (first != last) {
        root_ = make_range(first, last);
    }
}

template <typename Key, typename T, typename Allocator>
BinaryTree<Key, T, Allocator>::BinaryTree(BinaryTree const& other)
    : alloc_{nalloc_traits::select_on_container_copy_construction(other.alloc_)}
{
    if (other.root_) {
        root_ = make_range(other.cbegin(), other.cend());
    }
}

template <typename Key, typename T, typename Allocator>
BinaryTree<Key, T, Allocator>::BinaryTree(BinaryTree const& other, Allocator const& alloc)
    : alloc_{Nalloc{alloc}}
{
    if (other.root_) {
        root_ = make_range(other.cbegin(), other.cend());
    }
}

template <typename Key, typename T, typename Allocator>
BinaryTree<Key, T, Allocator>::BinaryTree(BinaryTree&& other)
    : alloc_{std::move(other.alloc_)}, root_{std::exchange(other.root_, nullptr)}
{
}

template <typename Key, typename T, typename Allocator>
BinaryTree<Key, T, Allocator>::BinaryTree(BinaryTree&& other, Allocator const& alloc)
    : alloc_{Nalloc{alloc}}, root_{std::exchange(other.root_, nullptr)}
{
    JAM_EXPECT(get_allocator() == other.get_allocator(),
        "Move constructed BinaryTree instance with incompatible allocator");
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::operator=(BinaryTree const& other) -> BinaryTree&
{
    if (this != &other) {
        free();
        if (nalloc_traits::propagte_on_container_copy_assignment) {
            alloc_ = other.alloc_;
        }
        if (other.root_) {
            root_ = make_range(other.cbegin(), other.cend());
        }
    }
    return *this;
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::operator=(BinaryTree&& other) noexcept -> BinaryTree&
{
    if (this != &other) {
        free();
        if (nalloc_traits::propagate_on_container_move_assignment) {
            alloc_ = std::move(other.alloc_);
        }
        root_ = std::exchange(other.root_, nullptr);
    }
    return *this;
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::find_node_with_link(Node* root, key_type const& key) noexcept -> std::pair<Node**, Node*>
{
    Node* node{root};
    Node** link{&root};
    while (node != nullptr) {
        if (node->data.first < key) {
            link = &node->right;
            node = node->right;
        }
        else if (key < node->data.first) {
            link = &node->left;
            node = node->left;
        }
        else break;
    }
    return {link, node};
}


template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::find_node_with_link(key_type const& key) noexcept -> std::pair<Node**, Node*>
{
    Node* node{root_};
    Node** link{&root_};
    while (node != nullptr) {
        if (node->data.first < key) {
            link = &node->right;
            node = node->right;
        }
        else if (key < node->data.first) {
            link = &node->left;
            node = node->left;
        }
        else break;
    }
    return {link, node};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::insert_node(Node* node) -> std::pair<iterator, bool>
{
    auto node_inserted{insert_at(root_, node)};
    if (node_inserted.second) {
        return {iterator{root_, node->data.first}, node_inserted.second};
    }
    else {
        return {end(), node_inserted.second};
    }
}


template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::insert(value_type const& value) -> std::pair<iterator, bool>
{
    Node* node{make_node(value)};
    return insert_node(node);
}

template <typename Key, typename T, typename Allocator>
template<typename P, typename>
auto BinaryTree<Key, T, Allocator>::insert(P&& value) -> std::pair<iterator, bool>
{
    Node* node{make_node(std::forward<P>(value))};
    return insert_node(node);
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::insert(value_type&& value) -> std::pair<iterator, bool>
{
    Node* node{make_node(std::move(value))};
    return insert_node(node);
}

template <typename Key, typename T, typename Allocator>
template<typename InputIt>
void BinaryTree<Key, T, Allocator>::insert(InputIt first, InputIt last)
{
    while (first != last) {
        Node* node{make_node(*first)};
        insert_node(node);
        ++first;
    }
}

template <typename Key, typename T, typename Allocator>
template<typename M, typename>
auto BinaryTree<Key, T, Allocator>::insert_or_assign(key_type const& key, M&& value) -> std::pair<iterator, bool>
{
    auto node_inserted{insert_or_assign_impl(root_, key, std::forward<M>(value))};
    return {iterator{root_, node_inserted.first->data.first}, node_inserted.second};
}

template <typename Key, typename T, typename Allocator>
template<typename M, typename>
auto BinaryTree<Key, T, Allocator>::insert_or_assign(key_type&& key, M&& value) -> std::pair<iterator, bool>
{
    auto node_inserted{insert_or_assign_impl(root_, std::move(key), std::forward<M>(value))};
    return {iterator{root_, node_inserted.first->data.first}, node_inserted.second};
}

template <typename Key, typename T, typename Allocator>
template<typename M, typename>
auto BinaryTree<Key, T, Allocator>::insert_or_assign(const_iterator hint, key_type const& key, M&& value) -> iterator
{
    auto node_inserted{insert_or_assign_impl(hint.current, key, std::forward<M>(value))};
    return iterator{root_, node_inserted.first->data.first};
}

template <typename Key, typename T, typename Allocator>
template<typename M, typename>
auto BinaryTree<Key, T, Allocator>::insert_or_assign(const_iterator hint, key_type&& key, M&& value) -> iterator
{
    auto node_inserted{insert_or_assign_impl(hint.current, std::move(key), std::forward<M>(value))};
    return iterator{root_, node_inserted.first->data.first};
}

template <typename Key, typename T, typename Allocator>
template<typename... Args>
auto BinaryTree<Key, T, Allocator>::emplace(Args&&... args) -> std::pair<iterator, bool>
{
    Node* node{make_node(std::forward<Args>(args)...)};
    return insert_node(node);
}

template <typename Key, typename T, typename Allocator>
template<typename... Args>
auto BinaryTree<Key, T, Allocator>::try_emplace(key_type const& key, Args&&... args) -> std::pair<iterator, bool>
{
    auto node_emplaced{try_emplace_impl(root_, key, std::forward<Args>(args)...)};
    return {iterator{root_, node_emplaced.first->data.first}, node_emplaced.second};
}

template <typename Key, typename T, typename Allocator>
template<typename... Args>
auto BinaryTree<Key, T, Allocator>::try_emplace(key_type&& key, Args&&... args) -> std::pair<iterator, bool>
{
    auto node_emplaced{try_emplace_impl(root_, std::move(key), std::forward<Args>(args)...)};
    return {iterator{root_, node_emplaced.first->data.first}, node_emplaced.second};
}

template <typename Key, typename T, typename Allocator>
template<typename... Args>
auto BinaryTree<Key, T, Allocator>::try_emplace(const_iterator hint, key_type const& key, Args&&... args) -> iterator
{
    auto node_emplaced{try_emplace_impl(hint.current, key, std::forward<Args>(args)...)};
    return {iterator{root_, node_emplaced.first->data.first}, node_emplaced.second};
}

template <typename Key, typename T, typename Allocator>
template<typename... Args>
auto BinaryTree<Key, T, Allocator>::try_emplace(const_iterator hint, key_type&& key, Args&&... args) -> iterator
{
    auto node_emplaced{try_emplace_impl(hint.current, std::move(key), std::forward<Args>(args)...)};
    return {iterator{root_, node_emplaced.first->data.first}, node_emplaced.second};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase_leaf(Node** link, Node* node) -> iterator
{
    // erase leaf
    *link = nullptr;
    free(node);
    return iterator{};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase_semibranch(Node** link, Node* node) -> iterator
{
    // whichever link is not null, is the correct child
    Node* const new_child = [to_erase=node]() {
        if (to_erase->left != nullptr) {
            Node* const child{to_erase->left};
            to_erase->left = nullptr;
            return child;
        }
        else {
            Node* const child{to_erase->right};
            to_erase->right = nullptr;
            return child;
        }
    }();

    *link = new_child;
    free(node);
    return iterator{root_, new_child->data.first};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase_branch(Node** link, Node* node) -> iterator
{
    // find smallest node following the right link (the appropriate parent for the left link node)
    Node* const successor = [cur=node->right]() mutable {
        auto* pred{cur};
        while (cur->left != nullptr) {
            pred = cur;
            cur = cur->left;
        }
        if (pred != cur) { pred->left = cur->right; }
        return cur;
    }();

    // left successor link needs to point at the node the erase target points at
    successor->left = node->left;
    // point the right successor link at the right erase target link, but only if
    // successor is not the node on the right of the erase target - this would cause the right successor
    // link to point at the successor (circular)
    if (successor != node->right) {
        successor->right = node->right;
    }
    *link = successor;
    free(node);
    return iterator{root_, successor->data.first};
}


template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase(const_iterator pos) -> iterator
{
    auto link_node{find_node_with_link(pos->first)};
    if (link_node.second != nullptr) {
        if (link_node.second->left != nullptr && link_node.second->right != nullptr) {
            return erase_branch(link_node.first, link_node.second);
        }
        else if (link_node.second->left != nullptr || link_node.second->right != nullptr) {
            return erase_semibranch(link_node.first, link_node.second);
        }
        else {
            return erase_leaf(link_node.first, link_node.second);
        }
    }
    else { return iterator{}; }
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase(iterator pos) -> iterator
{
    return erase(const_iterator{pos});
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase(const_iterator first, const_iterator last) -> iterator
{
    iterator it;
    while (first != last) {
        it = erase(first->first);
    }
    return it;
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::erase(key_type const& key) -> size_type
{
    auto link_node{find_node_with_link(key)};
    if (link_node.second != nullptr) {
        if (link_node.second->left != nullptr && link_node.second->right != nullptr) {
            erase_branch(link_node.first, link_node.second);
            return 1u;
        }
        else if (link_node.second->left != nullptr || link_node.second->right != nullptr) {
            erase_semibranch(link_node.first, link_node.second);
            return 1u;
        }
        else {
            erase_leaf(link_node.first, link_node.second);
            return 1u;
        }
    }
    else { return 0u; }
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::find(key_type const& key) noexcept -> iterator
{
    // If we had an implementation that allowed for O(1) iterators this would be ideal implementation:
    // Node* node{root_};
    // while (node != nullptr && node->data.first != key) {
    //     if (node->data.first < key) { node = node->right; }
    //     else if (key < node->data.first) { node = node->left; }
    // }
    // return iterator{node};

    // But with the current implementation we can't do better than this:
    return iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::find(key_type const& key) const noexcept -> const_iterator
{
    return const_iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
template<typename K> auto BinaryTree<Key, T, Allocator>::find(K const& key) noexcept -> iterator
{
    return iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
template<typename K> auto BinaryTree<Key, T, Allocator>::find(K const& key) const noexcept -> const_iterator
{
    return const_iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
bool BinaryTree<Key, T, Allocator>::contains(Key const& key) const noexcept
{
    Node* node{root_};
    while (node != nullptr && node->data.first != key) {
        if (node->data.first < key) { node = node->right; }
        else if (key < node->data.first) { node = node->left; }
    }
    return node != nullptr;
}

template <typename Key, typename T, typename Allocator>
template<typename K>
bool BinaryTree<Key, T, Allocator>::contains(K const& key) const noexcept
{
    Node* node{root_};
    while (node != nullptr && node->data.first != key) {
        if (node->data.first < key) { node = node->right; }
        else if (key < node->data.first) { node = node->left; }
    }
    return node != nullptr;
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::equal_range(Key const& key) noexcept -> std::pair<iterator, iterator>
{
    Node* node{root_};
    while (node != nullptr && node->data.first != key) {
        if (node->data.first < key) { node = node->right; }
        else if (key < node->data.first) { node = node->left; }
    }
    return node != nullptr;
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::equal_range(Key const& key) const noexcept -> std::pair<const_iterator, const_iterator>
{
    return {const_iterator{root_, key}, std::next(const_iterator{root_, key})};
}

template <typename Key, typename T, typename Allocator>
template<typename K>
auto BinaryTree<Key, T, Allocator>::equal_range(K const& key) noexcept -> std::pair<iterator, iterator>
{
    return {iterator{root_, key}, std::next(iterator{root_, key})};
}

template <typename Key, typename T, typename Allocator>
template<typename K>
auto BinaryTree<Key, T, Allocator>::equal_range(K const& key) const noexcept -> std::pair<const_iterator, const_iterator>
{
    return {const_iterator{root_, key}, std::next(const_iterator{root_, key})};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::lower_bound(Key const& key) noexcept -> iterator
{
    return {iterator{root_, key}, std::next(iterator{root_, key})};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::lower_bound(Key const& key) const noexcept -> const_iterator
{
    return const_iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
template<typename K>
auto BinaryTree<Key, T, Allocator>::lower_bound(K const& key) noexcept -> iterator
{
    return iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
template<typename K>
auto BinaryTree<Key, T, Allocator>::lower_bound(K const& key) const noexcept -> const_iterator
{
    return const_iterator{root_, key};
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::upper_bound(Key const& key) noexcept -> iterator
{
    return std::next(iterator{root_, key});
}

template <typename Key, typename T, typename Allocator>
auto BinaryTree<Key, T, Allocator>::upper_bound(Key const& key) const noexcept -> const_iterator
{
    return std::next(const_iterator{root_, key});
}

template <typename Key, typename T, typename Allocator>
template<typename K>
auto BinaryTree<Key, T, Allocator>::upper_bound(K const& key) noexcept -> iterator
{
    return std::next(iterator{root_, key});
}

template <typename Key, typename T, typename Allocator>
template<typename K>
auto BinaryTree<Key, T, Allocator>::upper_bound(K const& key) const noexcept -> const_iterator
{
    return std::next(const_iterator{root_, key});
}


template<typename NodeType, typename Reference>
class BinaryTree_iterator_base
{
protected:
    using self              = BinaryTree_iterator_base;
    using Node              = NodeType;
    using key_type          = typename Node::key_type;
    using stack_type        = std::stack<Node*>;
    template<typename Key, typename T, typename Alloc> friend class BinaryTree;
public:
    using value_type        = std::remove_cv_t<std::remove_reference_t<Reference>>;
    using reference         = Reference;
    using pointer           = std::add_pointer_t< std::remove_reference_t<Reference> >;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    BinaryTree_iterator_base() = default;

    explicit BinaryTree_iterator_base(Node* n)
        : current{n} { }

    template<typename OtherNodePointer, typename OtherReference,
        typename = std::enable_if_t<std::is_convertible_v<OtherNodePointer, NodeType>>
        >
    BinaryTree_iterator_base(BinaryTree_iterator_base<OtherNodePointer, OtherReference> const& other)
        : node_stack{other.node_stack}, current{other.current} { }

    template<typename OtherNodePointer, typename OtherReference,
        typename = std::enable_if_t<std::is_convertible_v<OtherNodePointer, NodeType>>
        >
    BinaryTree_iterator_base(BinaryTree_iterator_base<OtherNodePointer, OtherReference>&& other) noexcept
        : node_stack{std::move(other.node_stack)}, current{std::move(other.current)} { }

    BinaryTree_iterator_base(BinaryTree_iterator_base const&) = default;
    BinaryTree_iterator_base(BinaryTree_iterator_base&&) = default;
    BinaryTree_iterator_base& operator=(BinaryTree_iterator_base const&) = default;
    BinaryTree_iterator_base& operator=(BinaryTree_iterator_base&&) = default;
    ~BinaryTree_iterator_base() noexcept = default; // iterator does not own the node

    reference operator*() const noexcept
    { return current->data; }

    pointer operator->() const noexcept
    { return &(self::operator*()); }

// --- implement operator++ in the derived classes

template<typename T1, typename NP1, typename R1, typename T2, typename NP2, typename R2>
friend bool operator==( const BinaryTree_iterator<T1,NP1,R1>& lhs,
                        const BinaryTree_iterator<T2,NP2,R2>& rhs ) noexcept;
template<typename T1, typename NP1, typename R1, typename T2, typename NP2, typename R2>
friend bool operator!=( const BinaryTree_iterator<T1,NP1,R1>& lhs,
                        const BinaryTree_iterator<T2,NP2,R2>& rhs ) noexcept;

protected:
    inline Node* pop_stack()
    {
        Node* node{node_stack.top()};
        node_stack.pop();
        return node;
    }

    inline void push_stack(Node* node)
    {
        node_stack.push(node);
    }

    stack_type node_stack{};
    Node*      current{nullptr};
};

template<typename NP1, typename R1, typename T1, typename NP2, typename R2, typename T2>
bool operator==( const BinaryTree_iterator<NP1,R1,T1>& lhs,
                        const BinaryTree_iterator<NP2,R2,T2>& rhs ) noexcept
{
    return lhs.current == rhs.current && lhs.node_stack.empty() == rhs.node_stack.empty();
}

template<typename NP1, typename R1, typename T1, typename NP2, typename R2, typename T2>
bool operator!=( const BinaryTree_iterator<NP1,R1,T1>& lhs,
                        const BinaryTree_iterator<NP2,R2,T2>& rhs ) noexcept
{
    return lhs.current != rhs.current /* && lhs.node_stack.empty() == rhs.node_stack.empty() */;
}


template<typename NodeType, typename Reference, typename = BT_Iterator_Preorder_Tag>
class BinaryTree_iterator : public BinaryTree_iterator_base<NodeType, Reference>
{
    using self = BinaryTree_iterator;
    using base = BinaryTree_iterator_base<NodeType,Reference>;
    using Node = typename base::Node;
    using key_type = typename Node::key_type;

public:
    using BinaryTree_iterator_base<NodeType,Reference>::BinaryTree_iterator_base;

    // explicit BinaryTree_iterator(Node* first, Node* target)
    //     : base{first}
    //     {
    //         while(this->current != target)
    //             ++*this;
    //     }

    explicit BinaryTree_iterator(Node* first, key_type const& key)
        : base{first}
        {
            while(this->current && this->current->data.first != key)
                ++*this;
        }

    self& operator++() noexcept
    {
        if(this->current->right)
            this->node_stack.push(this->current->right);
            // push_stack(this->current->right);
        this->current = this->current->left;
        if(!this->current && !this->node_stack.empty()){
            this->current = this->node_stack.top();
            this->node_stack.pop();
            // this->current = pop_stack();
        } // else == end iterator
        return *this;
    }

    self operator++(int) noexcept
    {
        auto ret = *this;
        ++*this;
        return ret;
    }
};


template<typename NodeType, typename Reference>
class BinaryTree_iterator<NodeType, Reference, BT_Iterator_Inorder_Tag>
    : public BinaryTree_iterator_base<NodeType, Reference>
{
    using self = BinaryTree_iterator;
    using base = BinaryTree_iterator_base<NodeType,Reference>;
    using Node = typename base::Node;
    using key_type = typename Node::key_type;
public:
    using BinaryTree_iterator_base<NodeType,Reference>::BinaryTree_iterator_base;

    explicit BinaryTree_iterator(Node* n)
        : base{n}
        {
            if(!this->current) return;
            traverse_minimum();
        }

    // BinaryTree_iterator(Node* first, Node* target)
    //     : base{first}
    //     {
    //         while(this->current != target)
    //             ++*this;
    //     }

    explicit BinaryTree_iterator(Node* first, key_type const& key)
        : BinaryTree_iterator{first}
    {
        while(this->current && this->current->data.first != key)
            ++*this;
    }

    self& operator++() noexcept
    {
        this->current = this->current->right;
        if(this->current){
            traverse_minimum();
        } else if(!this->node_stack.empty()){
            this->current = this->node_stack.top();
            this->node_stack.pop();
            // this->current = pop_stack();
        } // else == end iterator

        return *this;
    }

    self operator++(int) noexcept
    {
        auto ret = *this;
        ++*this;
        return ret;
    }

private:
    void traverse_minimum()
    {
        while(this->current->left){
            // push_stack(this->current);
            this->node_stack.push(this->current);
            this->current = this->current->left;
        }
    }
};


template<typename NodeType, typename Reference>
class BinaryTree_iterator<NodeType, Reference, BT_Iterator_Postorder_Tag>
    : public BinaryTree_iterator_base<NodeType,Reference>
{
    using self = BinaryTree_iterator;
    using base = BinaryTree_iterator_base<NodeType,Reference>;
    using Node = typename base::Node;
    using key_type = typename Node::key_type;
public:
    using base::BinaryTree_iterator_base;

    explicit BinaryTree_iterator(Node* n)
        : base{n}
        {
            if(!this->current) return;
            traverse_maximum();
        }

    // BinaryTree_iterator(Node* first, Node* target)
    //     : base{first}
    //     {
    //         while(this->current != target)
    //             ++*this;
    //     }

    explicit BinaryTree_iterator(Node* first, key_type const& key)
        : BinaryTree_iterator{first}
    {
        traverse_maximum();
        while(this->current && this->current->data.first != key)
            ++*this;
    }

    self& operator++() noexcept
    {
        this->current = this->current->left;
        if(this->current){
            traverse_maximum();
        } else if(!this->node_stack.empty()){
            this->current = this->node_stack.top();
            this->node_stack.pop();
            // this->current = pop_stack();
        } // else == end_iterator
        return *this;
    }

    self operator++(int) noexcept
    {
        auto ret = *this;
        ++*this;
        return ret;
    }

private:
    void traverse_maximum()
    {
        while(this->current->right){
            // push_stack(this->current);
            this->node_stack.push(this->current);
            this->current = this->current->right;
        }
    }
};

#endif  // DATA_STRUCTURES_BINARY_TREE_HPP
