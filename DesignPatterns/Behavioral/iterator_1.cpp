#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <iterator>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

/**
 * The `Iterator` design pattern provides a way to access elements of an aggregate object
 * sequentially without exposing its underlying representation.
 * Iterators are one of the fundaments of the STL.
 */

class node_base {
public:
    node_base() noexcept = default;
    virtual ~node_base() noexcept = default;

    node_base* next() const { return next_.get(); }

    virtual void print(std::ostream& os) const = 0;
    virtual void double_me() = 0;

    void append(std::unique_ptr<node_base> p)
    {
        if (next_) {
            next_->append(std::move(p));
        }
        else {
            next_ = std::move(p);
        }
    }
private:
    std::unique_ptr<node_base> next_{};
};

template<typename T>
class node : public node_base {
public:
    explicit node(T value)
        : value_{std::move(value)}
    {
    }

    void print(std::ostream& os) const override { os << value_; }
    void double_me() override { value_ += value_; }

private:
    T value_;
};

inline std::ostream& operator<<(std::ostream& os, node_base const& n)
{
    n.print(os);
    return os;
}

// iterator
template<typename Value>
class node_iter
    : public boost::iterator_facade<node_iter<Value>
                                    ,Value
                                    ,std::forward_iterator_tag
                                    > {
public:
    node_iter() noexcept = default;

    explicit node_iter(Value* p)
        : node_{p}
    {
    }

    template<typename OtherValue,
             typename = std::enable_if_t<std::is_convertible_v<OtherValue*, Value*>>>
    node_iter(node_iter<OtherValue> const& other)
        : node_{other.node_}
    {
    }

private:
    friend class boost::iterator_core_access;

    template<typename OtherValue>
    bool equal(node_iter<OtherValue> const& other) const noexcept
    {
        return node_ == other.node_;
    }

    void increment() { node_ = node_->next(); }
    Value& dereference() const { return *node_; }
// --- member data
    Value* node_{nullptr};
};

using node_iterator = node_iter<node_base>;
using const_node_iterator = node_iter<node_base const>;


int main()
{
    auto nodes = node<int>{42};
    nodes.append(std::make_unique<node<std::string>>(" is greater than "));
    nodes.append(std::make_unique<node<int>>(13));

    std::copy(
        const_node_iterator(&nodes), const_node_iterator(),
        std::ostream_iterator<node_base>(std::cout, " ")
    );
    std::cout << "\n";

    std::for_each(
        node_iterator(&nodes), node_iterator(),
        [](node_base& nb) { nb.double_me(); }
    );

    std::copy(
        const_node_iterator(&nodes), const_node_iterator(),
        std::ostream_iterator<node_base>(std::cout, "/")
    );
    std::cout << "\n";
}
