#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <utility>
#include <cassert>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/utility/enable_if.hpp>


class node_base
{
public:
    node_base() noexcept = default;
    node_base(const node_base&) = delete;
    node_base& operator=(const node_base&) = delete;

    // Each node manages all of its tail nodes
    virtual ~node_base() noexcept { delete m_next; }

    // Access the rest of the list
    node_base* next() const { return m_next; }

    // print to the stream
    virtual void print(std::ostream& os) const = 0;

    // double the value
    virtual void double_me() = 0;

    void append(node_base* p)
    {
        if(m_next)
            m_next->append(p);
        else
            m_next = p;
    }
private:
    node_base* m_next{nullptr};
};


template<typename T>
class node : public node_base
{
public:
    node(T x)
        : m_value{std::move(x)} { }
    
    void print(std::ostream& os) const override { os << this->m_value; }
    void double_me() override { m_value += m_value; }

private:
    T m_value;
};

inline std::ostream& operator<<(std::ostream& os, node_base const& n)
{
    n.print(os);
    return os;
}


// If the type we're basing our iterator on is itself an iterator then we can
// use an iterator_adaptor to overload only the desired operators that
// don't meet our expectations.
template<typename Value>
class node_iter
    : public boost::iterator_adaptor<
                node_iter<Value>    // Derived
            ,   Value*              // Base
            ,   boost::use_default  // Value
            ,   boost::forward_traversal_tag
        >
{
    struct enabler { };

public:
    node_iter() noexcept
        : node_iter::iterator_adaptor_{nullptr} { }
    
    explicit node_iter(Value* p)
        : node_iter::iterator_adaptor_{p} { }

    template<typename OtherValue>
    node_iter(
            const node_iter<OtherValue>& other
           ,typename boost::enable_if<
                boost::is_convertible<OtherValue*,Value*>
            ,   enabler
            >::type = enabler()
    )
        : node_iter::iterator_adaptor_(other.base()) { }

private:
    friend boost::iterator_core_access;
    void increment() { this->base_reference() = this->base()->next(); }
};

using node_iterator = node_iter<node_base>;
using node_const_iterator = node_iter<const node_base>;

int main()
{

    auto nodes = std::make_unique<node<int>>(42);
    nodes->append(new node<std::string>(" is greater than "));
    nodes->append(new node<int>(13));

    // Check interoperability
    assert(node_iterator(nodes.get()) == node_const_iterator(nodes.get()));
    assert(node_const_iterator(nodes.get()) == node_iterator(nodes.get()));
    
    assert(node_iterator(nodes.get()) != node_const_iterator());
    assert(node_const_iterator(nodes.get()) != node_iterator());
    
    std::copy(
        node_iterator(nodes.get()), node_iterator()
      , std::ostream_iterator<node_base>(std::cout, " ")
    );
    std::cout << std::endl;
    
    std::for_each(
        node_iterator(nodes.get()), node_iterator()
      , [](node_base& nb){nb.double_me();}
    );

    std::copy(
        node_const_iterator(nodes.get()), node_const_iterator()
      , std::ostream_iterator<node_base>(std::cout, "/")
    );
    std::cout << std::endl;
    return 0;
}
