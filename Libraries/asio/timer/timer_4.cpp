#include <iostream>
#include <chrono>

#include <asio.hpp>


// Example of encapsulating partf of asio functionality into a class.
// Here the periodic printing is done in the Printer's class member function.
class Printer
{
public:
    Printer(asio::io_context& io)
        : timer_{io, std::chrono::seconds{1}}
    {
        timer_.async_wait([this](asio::error_code const& ec){
            if (!ec)
                print();
            else
                std::cout << "print error: " << ec.message() << std::endl;
        });
    }

    ~Printer()
    {
        std::cout << "Final count is: " << count_ << std::endl;
    }

    void print() noexcept
    {
        if (count_ < 5)
        {
            std::cout << "count: " << count_ << std::endl;
            ++count_;
            timer_.expires_at(timer_.expiry() + std::chrono::seconds{1});
            timer_.async_wait([this](asio::error_code const& ec)noexcept {
                if (!ec)
                    print();
                else
                    std::cout << "print error: " << ec.message() << std::endl;
            });
        }
    }

private:
    asio::steady_timer timer_;
    int count_{0};
};


int main()
{
    asio::io_context io;
    Printer printer{io};
    io.run();
}
