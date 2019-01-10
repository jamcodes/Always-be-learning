#include <iostream>
#include <string>
#include <type_traits>


/**
 * NullObject encapsulates absence of an object by providing a substitutable alternative that
 * offers suitable default do-nothing behavior. It's used when an object requires a collaborator,
 * but some collaborators should do nothing. It also provides a way of abstracting hanlding of
 * nullpointers from the client code.
 */

class Log {
public:
    virtual void log_info(std::string const& message) const = 0;
};

class ConsoleLog : public Log {
public:
    void log_info(std::string const& message) const override
    {
        std::cerr << message << "\n";
    }
};

template<typename LogType>
class PaymentProcessing {
    static_assert(std::is_base_of_v<Log, LogType>, "LogType must derive from Log");
public:
    void process()
    {
        log_.log_info("Processing payments...");
    }

private:
    LogType log_;
};

class NullLog : public Log {
public:
    void log_info(std::string const&) const override { }
};


int main()
{
    PaymentProcessing<NullLog> pp;
    pp.process();
    PaymentProcessing<ConsoleLog> ppc;
    ppc.process();
}
