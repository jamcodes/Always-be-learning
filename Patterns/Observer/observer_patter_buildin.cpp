#include <iostream>
#include <vector>
#include <algorithm>


class Observer
{
public:
    virtual void notify() const = 0;
    virtual int id() const = 0;
};

class SomeObserver : public Observer
{
    int m_id;
public:
    SomeObserver( int id ) : m_id(id) { }
    auto notify() const -> void override
    {
        std::cout << "SomeObserver id[" << m_id << "] notified" << std::endl;
    }
    auto id() const -> int override { return m_id; }
};


class Subject
{
    std::vector<Observer*> m_observers;
public:
    auto register_observer( Observer* pobs ) -> void
    { m_observers.push_back(pobs); }
    
    auto erase_observer( Observer* pobs ) -> void
    { 
        auto it = std::find(m_observers.begin(), m_observers.end(), pobs);
        if( it != m_observers.end() ){
            m_observers.erase(it);
            std::cout << "Observer id[" << pobs->id() << "] erased" << std::endl;
        }
    }

    auto notify_observers( ) -> void
    {
        for( auto pobs : m_observers ){
            pobs->notify();
        }
    }
};


int main()
{
    Subject sub{};
    SomeObserver obs1{1};
    sub.register_observer( &obs1 );
    sub.notify_observers();
    {
        SomeObserver obs2{2};
        sub.register_observer(&obs2);
        sub.notify_observers();
        sub.erase_observer(&obs2);
    }
    SomeObserver sub3{3};
    sub.register_observer(&sub3);
    sub.notify_observers();
}
