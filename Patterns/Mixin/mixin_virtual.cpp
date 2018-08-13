/* This is an example of code-reuse in C++ using mixin-classes.
 * The concept relies on a pattern of inheriting from a templated base class,
 * which provides the desired functionality (a method with a given name).
 */
#include <iostream>
#include <chrono>
using namespace std::chrono_literals;

struct ITask    // some interface
{
    virtual ~ITask() noexcept = default;
    virtual const std::string& get_name() const noexcept = 0;
    virtual void execute() = 0;
};

class MyTask
{
public:
    virtual ~MyTask() noexcept = default;   // necessary?

    void execute()
    {
        std::cout << "... This is where MyTask is executed ... " << std::endl;
    }

    const std::string& get_name() const noexcept
    { return name; }
private:
    std::string name{"MyTask"};
};

template<typename T>
class LoggingTask : public T
{
public:
    virtual ~LoggingTask() noexcept = default;   // necessary?
    void execute()
    {
        std::cout << "LOG: The task is starting - " << T::get_name() << std::endl;
        T::execute();
        std::cout << "LOG: The task has completed - " << T::get_name() << std::endl;
    }
};


template<typename T>
class TimingTask : public T
{
public:
    virtual ~TimingTask() noexcept = default;   // necessary?
    void execute()
    {
        auto start = std::chrono::steady_clock::now();
        T::execute();
        auto end = std::chrono::steady_clock::now();
        std::cout << "Task duration: " << (end - start).count() << std::endl;
    }
};

template<typename T>
class TaskAdapter : public ITask, public T
{
public:
    void execute() override
    {
        T::execute();
    }

    const std::string& get_name() const noexcept override
    {
        return T::get_name();
    }
};

using Task = TaskAdapter<
                LoggingTask<
                    TimingTask<
                        MyTask> > >;

int main()
{
    Task t;
    auto it = &t;
    it->execute();
}
