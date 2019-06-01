#include <iostream>
// #include <iterator>
#include <vector>
#include <forward_list>
#include <list>


template<typename Derived>
    // std::iterator is deprecated in C++17
struct InserterBase // : public std::iterator<std::output_iterator_tag, void,void,void,void>
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using reference         = void;
    using pointer           = void;
    using difference_type   = std::ptrdiff_t;   // deprecated in C++17

    decltype(auto) operator*() noexcept { return derived(); }
    decltype(auto) operator*() const noexcept { return derived(); }
    decltype(auto) operator++() noexcept { return derived(); }
    auto operator++(int) noexcept { return derived(); }

protected:
    Derived& derived() noexcept
    { return static_cast<Derived&>(*this); }
    const Derived& derived() const noexcept
    { return static_cast<const Derived&>(*this); }

private:
    InserterBase() noexcept = default;
    friend Derived;
};

template<typename Container>
struct Backinserter : public InserterBase<Backinserter<Container>>
{
public:
    explicit Backinserter(Container& cont)
        : container{cont} { }

    Backinserter& operator=(const typename Container::value_type& val)
    {
        container.push_back(val);
        return *this;
    }

    Backinserter& operator=(typename Container::value_type&& val)
    {
        container.push_back(std::move(val));
        return *this;
    }
private:
    Container& container;
};

template<typename Container> constexpr
Backinserter<Container> make_Backinserter( Container& container )
{ return Backinserter<Container>(container); }


template<typename Container>
struct Frontinserter : public InserterBase<Frontinserter<Container>>
{
public:
    explicit Frontinserter(Container& cont) noexcept
        : container{cont} { }

    Frontinserter& operator=(const typename Container::value_type& val)
    {
        container.push_front(val);
        return *this;
    }

    Frontinserter& operator=(typename Container::value_type&& val)
    {
        container.push_front(std::move(val));
        return *this;
    }
private:
    Container& container;
};

template<typename Container> constexpr
Frontinserter<Container> make_Frontinserter(Container& container)
{ return Frontinserter<Container>(container); }

template<typename Container>
struct Inserter : public InserterBase<Inserter<Container>>
{
public:
    explicit Inserter( Container& cont, typename Container::iterator iter)
        : container{cont}, pos{iter} { }

    Inserter& operator=(const typename Container::value_type& val)
    {
        pos = container.insert(pos, val);
        ++pos;
        return *this;
    }

    Inserter& operator=(typename Container::value_type&& val)
    {
        pos = container.insert(pos, std::move(val));
        ++pos;
        return *this;
    }

private:
    Container& container;
    typename Container::iterator pos;
};

template<typename Container> constexpr
Inserter<Container> make_Inserter(Container& container, typename Container::iterator pos)
{ return Inserter<Container>(container, pos); }


template<typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& vec )
{
    for( const auto& elem : vec )
        os << elem << " ";
    return os;
}

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::forward_list<T>& vec )
{
    for( const auto& elem : vec )
        os << elem << " ";
    return os;
}

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::list<T>& vec )
{
    for( const auto& elem : vec )
        os << elem << " ";
    return os;
}

int main()
{
    std::vector<int> source{};
    for(int i=0; i<42; ++i) source.push_back(i);
    std::cout << "source: " << source << std::endl;
    std::vector<int> back_container{};
    back_container.reserve(42);
    std::copy(source.cbegin(), source.cend(), make_Backinserter(back_container));
    std::cout << "back_container: " << back_container << std::endl;
    
    std::forward_list<int> front_container{};
    std::copy(source.cbegin(), source.cend(), make_Frontinserter(front_container));
    std::cout << "front_container: " << front_container << std::endl;

    std::list<int> inserter_container{};
    std::copy(source.cbegin(), source.cend()
             , make_Inserter(inserter_container, inserter_container.begin()));
    std::cout << "inserter_container: " << inserter_container << std::endl;
}
