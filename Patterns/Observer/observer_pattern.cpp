#include <iostream>
#include <vector>
#include <memory>


class observer
{
public:
    virtual ~observer() noexcept = default;
    virtual void notify() = 0;
};

class SomeObserver : public observer
{
    int id{0};
public:
    SomeObserver( int id_ )
        : id(id_) { }
    void notify() override
    {
        std::cout << "SomeObserver [" << id << "] notified" << std::endl;
    }
};


class Subject
{
public:
    void register_observer( const std::shared_ptr<observer> sp_obs )
    {
        observers.push_back(std::move(sp_obs));
    }

    void notify_observers()
    {
        for( auto it = observers.begin(); it != observers.end(); ++it ){
            if( auto spt = it->lock() ){
                spt->notify();
            }
            else{
                std::cout << "Removing an expired observer" << std::endl;
            }
        }
    }

private:
    std::vector<std::weak_ptr<observer>> observers{};
};


int main()
{
    Subject sub;
    auto obs1 = std::make_shared<SomeObserver>(1);
    sub.register_observer(obs1);
    sub.notify_observers();
    {
        auto obs2 = std::make_shared<SomeObserver>(2);
        sub.register_observer(obs2);
        sub.notify_observers();
    }
    auto obs3 = std::make_shared<SomeObserver>(3);
    sub.register_observer(obs3);
    sub.notify_observers();
}
