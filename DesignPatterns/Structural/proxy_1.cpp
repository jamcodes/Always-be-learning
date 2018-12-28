#include <iostream>
#include <memory>
#include <utility>
#include <mutex>


class Subject {
public:
    Subject() noexcept = default;
    Subject(Subject const&) = default;
    Subject(Subject&&) noexcept = default;
    Subject& operator=(Subject const&) = default;
    Subject& operator=(Subject&&) = default;
    virtual ~Subject() noexcept = default;
    virtual void request() = 0;
};

class RealSubject : public Subject {
public:
    RealSubject()
    {
        std::cerr << "RealSubject::RealSubject()\n";
    }

    void request() override
    {
        std::cerr << "RealSubject::request()\n";
    }
};

class Proxy : public Subject {
public:
    Proxy()
        : psubject_{std::make_unique<RealSubject>()}
    {
    }

    // Forward calls to the RealSubject:
    void request() override
    {
        std::cerr << "Proxy::request()\n";
        psubject_->request();
    }

private:
    std::unique_ptr<Subject> psubject_{};
};

class LazyProxy : public Subject {
public:
    LazyProxy() noexcept = default;

    void request() override
    {
        std::cerr << "LazyProxy::request()\n";
        std::call_once(psubject_init_flag_, &LazyProxy::init_subject, this);
        psubject_->request();
    }

protected:
    void init_subject()
    {
        std::cerr << "LazyProxy::init_subject()\n";
        psubject_ = std::make_unique<RealSubject>();
    }

private:
    std::unique_ptr<Subject> psubject_{};
    std::once_flag psubject_init_flag_{};
};

int main()
{
    Proxy p;
    p.request();

    std::cerr << "\nLazyProxy:\n";
    LazyProxy lp;
    lp.request();
    lp.request();
}
