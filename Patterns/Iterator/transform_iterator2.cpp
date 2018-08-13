#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <cctype>

#include <boost/utility/enable_if.hpp>
#include <boost/iterator/iterator_adaptor.hpp>


template<typename Iterator
        ,typename Reference = const typename std::iterator_traits<Iterator>::value_type
        ,typename Value = typename std::iterator_traits<Iterator>::value_type
        >
class toupper_iterator
    : public boost::iterator_adaptor<
                toupper_iterator<Iterator,Reference,Value>
            ,   Iterator
            ,   Value
            ,   boost::use_default
            ,   Reference
            >
{
    struct enabler { };
    using self = toupper_iterator;
public:
    using value_type = Value;
    using reference = Reference;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

    toupper_iterator() noexcept = default;
    explicit toupper_iterator( const Iterator& it )
        : self::iterator_adaptor_{it} { }

    template<typename I2, typename R2, typename V2>
    toupper_iterator(
            const toupper_iterator<I2,R2,V2>& other
        ,   typename boost::enable_if<
                boost::is_convertible<I2,Iterator>, enabler>::type = enabler()
        ,   typename boost::enable_if<
                boost::is_convertible<R2,Reference>, enabler>::type = enabler()
        ,   typename boost::enable_if<
                boost::is_convertible<V2,Value>, enabler>::type = enabler()
        )
        : self::iteator_adaptor_{other.base()} { }

private:
    friend boost::iterator_core_access;

    reference dereference() const
    {
        auto toup = [](unsigned char ch){return std::toupper(ch);};
        return toup( *(this->base_reference()) );
    }
};

template<typename Iterator>
auto make_toupper_iterator( const Iterator& it )
{
    return toupper_iterator<Iterator>(it);
}


int main()
{
    auto str = std::string("Here's some text");
    auto v = std::vector<char>{'a','b','c','d'};

    std::cout << "str = " << str << std::endl;
    std::cout << "v = ";
    for( auto ch : v ){
        std::cout << ch;
    }
    std::cout << std::endl;

    std::cout << "\nstr:\n";
    std::copy( make_toupper_iterator(str.cbegin())
             , make_toupper_iterator(str.cend())
             , std::ostream_iterator<char>(std::cout, " "));
    std::cout << "\nv:\n";
    std::copy( make_toupper_iterator(v.cbegin())
             , make_toupper_iterator(v.cend())
             , std::ostream_iterator<char>(std::cout, " "));
}