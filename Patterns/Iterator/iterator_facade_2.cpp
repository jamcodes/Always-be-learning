#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <utility>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/iterator/iterator_facade.hpp>


class node_base
{
public:
    node_base() noexcept = default;

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


// Because the node_iterator and const_node_iterator are so similar
// the common code can be factored out into a template
// template<typename Value>
// class node_iter
//     : public boost::iterator_facade<node_iter<Value>
//                                    ,Value
//                                    ,boost::forward_traversal_tag
//                                    >
// {
// public:
//     node_iter() noexcept = default;

//     explicit node_iter(Value* p)
//         : m_node{p} { }

// private:
//     friend class boost::iterator_core_access;

//     bool equal(const node_iter<Value>& other) const
//     {
//         return this->m_node == other.m_node;
//     }

//     void increment()
//     { m_node = m_node->next(); }

//     Value& dereference() const
//     { return *m_node; }

// // --- member data
//     Value* m_node;
// };

// using node_iterator       = node_iter<node_base>;
// using const_node_iterator = node_iter<const node_base>;


// We can further improve the implementation by making the node_iterator and
// const_node_iterator interoperable - we should be able to compare
// node_iterator with a const_node_iterator - this can be done by making
// the equal() method a template
//
// We should also exclude conversion from const_node_iterator to node_iterator
// from the overload set. This is done using enable_if
template<typename Value>
class node_iter
    : public boost::iterator_facade<node_iter<Value>
                                   ,Value
                                   ,boost::forward_traversal_tag
                                   >
{
    struct enabler {};
public:
    node_iter() noexcept = default;

    explicit node_iter(Value* p)
        : m_node{p} { }

    template<typename OtherValue>
    node_iter( const node_iter<OtherValue>& other
             , typename boost::enable_if<
                    boost::is_convertible<OtherValue*,Value*>
                   ,enabler
               >::type = enabler()
             )
        : m_node(other.m_node)
        { }
private:
    friend class boost::iterator_core_access;

    template<typename OtherValue>
    bool equal(const node_iter<OtherValue>& other) const
    {
        return this->m_node == other.m_node;
    }

    void increment()
    { m_node = m_node->next(); }

    Value& dereference() const
    { return *m_node; }

// --- member data
    Value* m_node;
};

using node_iterator       = node_iter<node_base>;
using const_node_iterator = node_iter<const node_base>;

int main()
{
    auto nodes = std::make_unique<node<int>>(42);
    
    nodes->append( new node<std::string>(" is greather than ") );
    nodes->append( new node<int>(13) );

    std::copy(
        node_iterator(nodes.get()), node_iterator()
       ,std::ostream_iterator<node_base>(std::cout, " ")
    );
    std::cout << std::endl;

    std::for_each(
        node_iterator(nodes.get()), node_iterator()
       ,[](node_base& nb){ nb.double_me(); }
    );

    std::copy(
        node_iterator(nodes.get()), node_iterator()
       ,std::ostream_iterator<node_base>(std::cout, "/")
    );
    std::cout << std::endl;
}
