#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <type_traits>

#include <boost/utility/enable_if.hpp>
#include <boost/iterator/iterator_adaptor.hpp>



template<typename Iterator, typename UnaryFunction
        ,typename Reference
            = std::result_of_t<const UnaryFunction(typename std::iterator_traits<Iterator>::reference)>
        ,typename Value = std::remove_cv_t<std::remove_reference_t<Reference>>
        >
class transform_iterator
    : public boost::iterator_adaptor<
                transform_iterator<Iterator,UnaryFunction,Reference,Value>
            ,   Iterator
            ,   Value
            ,   boost::use_default
            ,   Reference
            >
{
    struct enabler { };
    using self = transform_iterator;
public:
    using value_type            = Value;
    using reference             = Reference;
    using pointer               = typename std::iterator_traits<Iterator>::pointer;
    using difference_type       = typename std::iterator_traits<Iterator>::difference_type;
    using iterator_category     = typename std::iterator_traits<Iterator>::iterator_category;

    transform_iterator() = default;
    transform_iterator( const Iterator& it, UnaryFunction function )
        : self::iterator_adaptor_{it}, m_function{function} { }
    
    template<typename I2, typename F2, typename R2,typename V2>
    transform_iterator(
        const transform_iterator<I2,F2,R2,V2>& other
       ,typename boost::enable_if<
                boost::is_convertible<I2,Iterator>
               ,enabler
        >::type = enabler()
       ,typename boost::enable_if<
                boost::is_convertible<F2,UnaryFunction>
               ,enabler
        >::type = enabler()
       ,typename boost::enable_if<
                boost::is_convertible<Reference,R2>
               ,enabler
        >::type = enabler()
       ,typename boost::enable_if<
                boost::is_convertible<Value,V2>
               ,enabler
        >::type = enabler()
    )
        : self::iterator_adaptor_{other.base()}, m_function{other.m_function}
        { }

    UnaryFunction functor() const { return m_function; }
private:
    friend boost::iterator_core_access;

    reference dereference() const
    {
        return m_function(*(this->base_reference()));
    }
// -- member data
    UnaryFunction m_function;
};
template<typename Iterator, typename Function
        >
auto make_transform_iterator( const Iterator& it, Function function )
{
    return transform_iterator<Iterator,Function>(it,function);
}


int main()
{
    auto xs = std::vector<int>{1,2,3,4,5,6,7,8,9};
    auto ys = std::vector<int>{};

    std::cout << "xs:\n";
    for( auto x : xs )
        std::cout << x << ", ";
    std::cout << std::endl;

    auto doubler = [](int i){ return i*i; };
    std::copy( make_transform_iterator(xs.cbegin(),doubler)
             , make_transform_iterator(xs.cend(),doubler)
             , std::back_inserter(ys)
             );

    std::cout << "After first copy:\n";
    for( auto i : ys )
        std::cout << i << ", ";
    std::cout << std::endl;

    auto plus100 = [](int i){ return i+100; };
    std::copy( make_transform_iterator(ys.cbegin(),plus100)
             , make_transform_iterator(ys.cend(),plus100)
             , std::ostream_iterator<int>(std::cout, " ")
             );
}
