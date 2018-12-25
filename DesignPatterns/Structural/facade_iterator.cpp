#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>


// Crtp can be used to implement the `facade` pattern - implement most or all of the public
// interface of a class in terms of a much smaller (but easier to implement) interface exposed
// by the CRTP derived class.
// Illustraded below by an implementation of a facade for iterators
template<typename Derived, typename Value, typename Category,
         typename Reference = std::add_lvalue_reference_t<Value>,
         typename Distance = std::ptrdiff_t>
class IteratorFacade
{
public:
    using value_type = std::remove_const_t<Value>;
    using reference = Reference;
    using pointer = std::add_pointer_t<Value>;
    using difference_type = Distance;
    using iterator_category = Category;

// input iterator interface
    reference operator*() const { return derived().dereference(); }
    pointer operator->() const { return &(this->operator*()); }
    Derived& operator++()
    {
        derived().increment();
        return derived();
    }
    Derived operator++(int) const
    {
        const auto res{derived()};
        derived().increment();
        return res;
    }
    friend bool operator==(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return lhs.derived().equals(rhs.derived());
    }

    friend bool operator!=(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return !(lhs.derived().equals(rhs.derived()));
    }

// bidirectional iterator interface
    Derived& operator--()
    {
        derived().decrement();
        return derived();
    }
    Derived operator--(int) const
    {
        const auto res{derived()};
        derived().decrement();
        return res;
    }

// random access iterator interface
    reference operator[](difference_type n) const { return (derived() + n).dereference(); }
    Derived& operator+=(difference_type n) { derived().advance(n); return derived(); }
    Derived operator+(difference_type n) const { auto res{derived()}; return res += n; }
    Derived& operator-=(difference_type n) { derived().advance(-n); return derived(); }
    Derived operator-(difference_type n) const { auto res{derived()}; return res -= n; }
    friend difference_type operator-(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return lhs.derived().difference_from(rhs.derived());
    }

    friend bool operator<(IteratorFacade const& lhs, IteratorFacade const& rhs) noexcept
    {
        return lhs.derived().less_than(rhs.derived());
    }
protected:
    friend Derived;
    constexpr IteratorFacade() noexcept = default;
    Derived& derived() & noexcept { return *static_cast<Derived*>(this); }
    const Derived& derived() const& noexcept { return *static_cast<Derived const*>(this); }
    Derived&& derived() && noexcept { return std::move(*static_cast<Derived*>(this)); }
    const Derived&& derived() const&& noexcept
        { return std::move(*static_cast<Derived const*>(this)); }

};


template<typename T>
struct node {
public:
    using value_type = T;
    template<typename... Args,
        typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
    explicit node(Args&&... args)
        : value{std::forward<Args>(args)...}
    {
    }

    T value;
    std::unique_ptr<node> next{nullptr};
};

template<typename T>
class node_iterator : public IteratorFacade<node_iterator<T>, T, std::forward_iterator_tag> {
public:
    node_iterator() noexcept = default;
    explicit node_iterator(T& n) noexcept
        : pnode_{&n}
    {
    }

    T& dereference() const noexcept { return *pnode_; }
    void increment() noexcept { pnode_ = pnode_->next.get(); }
    bool equals(node_iterator const& other) const noexcept
    {
        return this->pnode_ == other.pnode_;
    }
private:
    T* pnode_{nullptr};
};

int main()
{
    using Node = node<int>;
    Node n1{1};
    Node* n2 = new Node{2};
    Node* n3 = new Node{3};
    n1.next.reset(n2);
    n2->next.reset(n3);
    for (auto it = node_iterator<Node>{n1}; it != node_iterator<Node>{}; ++it) {
        std::cerr << it->value << ", ";
    }
}
