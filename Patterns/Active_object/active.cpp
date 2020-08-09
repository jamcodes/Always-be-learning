#include "active.h"

namespace jam
{

Active::~Active()
{
    send([this](){ do_done(); });
    thread_.join();
}

void Active::send(Callback msg)
{
    queue_.push(std::move(msg));
}

void Active::run()
{
    while (!done_) {
        // wait until a task is available, then pop it and execute it
        Callback cb;
        queue_.wait_and_pop(cb);
        cb();
    }
}


Active2::~Active2()
{
    send([this](){ do_done(); });
    thread_.join();
}

void Active2::send(Callback msg)
{
    queue_.push(std::move(msg));
}

void Active2::run()
{
    while (!done_) {
        // wait until a task is available, then pop it and execute it
        Callback cb;
        queue_.wait_and_pop(cb);
        cb();
    }
}

// void Active2::do_create(Active2& a)
// {
//     a.thread_ = std::thread{[&a](){ a.run(); }};
// }



} // namespace jam
