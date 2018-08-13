#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <utility>

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


// --- node_iterator class using inheritance from iterator_facade
class node_iterator
    : public boost::iterator_facade<node_iterator
                                   ,node_base
                                   ,boost::forward_traversal_tag
                                   >
{
public:
    node_iterator() noexcept = default;

    explicit node_iterator(node_base* p)
        : m_node{p} { }

    
private:
    friend class boost::iterator_core_access;   // necessary so that we can keep the impelementation private

    void increment() { m_node = m_node->next(); }

    bool equal(const node_iterator& other) const
    {
        return this->m_node == other.m_node;
    }

    node_base& dereference() const { return *m_node; }

// --- data members
    node_base* m_node{nullptr};
};


// --- const_node_iterator - same, but witha a different value_type
class const_node_iterator
    : public boost::iterator_facade<const_node_iterator
                                   ,const node_base
                                   ,boost::forward_traversal_tag
                                   >
{
public:
    const_node_iterator() noexcept = default;

    explicit const_node_iterator(node_base* p)
        : m_node{p} { }

private:
    friend class boost::iterator_core_access;
    
    void increment() { m_node = m_node->next(); }

    bool equal(const const_node_iterator& other) const
    {
        return this->m_node == other.m_node;
    }

    const node_base& dereference() const { return *m_node; }

// --- data members
    const node_base* m_node;
};




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
       ,std::mem_fun_ref(&node_base::double_me)
    );

    std::copy(
        node_iterator(nodes.get()), node_iterator()
       ,std::ostream_iterator<node_base>(std::cout, "/")
    );
    std::cout << std::endl;
}
