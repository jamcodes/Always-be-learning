#include <iostream>
#include <vector>
#include <algorithm>

class Subject;

class Observer
{
public:
    virtual void update(Subject*) = 0;
protected:
    ~Observer() = default;
};

class Subject
{
public:
    virtual ~Subject() = default;
    void notify()
    {
        for (auto pobs : observers_) {
            pobs->update(this);
        }
    }
    void register_observer(Observer* pobs) noexcept
    {
        observers_.push_back(pobs);
        std::cerr << "Observer " << reinterpret_cast<void*>(pobs) << " registered\n";
    }
    void deregister_observer(Observer* pobs) noexcept
    {
        const auto it = std::find(observers_.begin(), observers_.end(), pobs);
        if (it != observers_.end()) {
            observers_.erase(it);
            std::cerr << "Observer " << reinterpret_cast<void*>(pobs) << " deregistered\n";
        }
    }
private:
    std::vector<Observer*> observers_{};
};


class ConcreteObserver : public Observer
{
public:
    explicit constexpr ConcreteObserver(int id) noexcept
        : id_{id} { }

    void update(Subject* psub) override
    {
        std::cerr << __PRETTY_FUNCTION__ << ", id = " << id_
            << ", Subject: " << reinterpret_cast<void*>(psub) << "\n";
    }

private:
    int id_{};
};


int main()
{
    Subject sub{};
    ConcreteObserver obs1{1};
    sub.register_observer(&obs1);
    sub.notify();
    ConcreteObserver obs2{2};
    sub.register_observer(&obs2);
    sub.notify();
    sub.deregister_observer(&obs1);
    sub.notify();
}
