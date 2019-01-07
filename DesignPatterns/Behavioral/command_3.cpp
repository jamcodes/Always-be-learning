#include <iostream>
#include <string>
#include <vector>


class BankAccount {
public:
    void deposit(int amount)
    {
        balance_ += amount;
        std::cerr << "deposited " << amount << ", balance = " << balance_ << "\n";
    }

    void withdraw(int amount)
    {
        if (balance_ - amount >= overdraft_limit_) {
            balance_ -= amount;
            std::cerr << "withdrew " << amount << ", balance = " << balance_ << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, BankAccount const& ba)
    {
        return os << "BankAccount balance = " << ba.balance_;
    }

private:
    int balance_{0};
    int overdraft_limit_{-500};
};

class Command {
public:
    Command() = default;
    Command(Command const&) = default;
    Command(Command&&) = default;
    Command& operator=(Command const&) = default;
    Command& operator=(Command&&) = default;
    virtual ~Command() = default;

    virtual void call() const = 0;
    virtual void undo() const = 0;
};

class BankAccountCommand : public Command {
public:
    enum Action { Deposit, Withdraw};

    BankAccountCommand(BankAccount& account, const Action action, const int amount)
        : account_{account}, action_{action}, amount_{amount}
    {
    }

    void call() const override
    {
        switch (action_) {
        case Deposit:
            account_.deposit(amount_);
            break;
        case Withdraw:
            account_.withdraw(amount_);
            break;
        default:
            break;
        }
    }

    void undo() const override
    {
        switch (action_) {
        case Withdraw:
            account_.deposit(amount_);
            break;
        case Deposit:
            account_.withdraw(amount_);
        default:
            break;
        }
    }

private:
    BankAccount& account_;
    Action action_;
    int amount_;
};

class CompositeBankAcountCommand : private std::vector<BankAccountCommand>, public Command {
    using Storage = std::vector<BankAccountCommand>;
public:
    using value_type = Storage::value_type;
    CompositeBankAcountCommand(std::initializer_list<value_type> il)
        : Storage(il)
    {
    }

    void call() const override
    {
        for (auto const& cmd : *this) {
            cmd.call();
        }
    }

    void undo() const override
    {
        for (auto const& cmd : *this) {
            cmd.undo();
        }
    }
};


int main()
{
    BankAccount account;
    CompositeBankAcountCommand commands {
        BankAccountCommand{account, BankAccountCommand::Deposit, 100},
        BankAccountCommand{account, BankAccountCommand::Withdraw, 200}
    };

    std::cerr << account << "\n";

    commands.call();
    std::cerr << account << "\n";
    commands.undo();
    std::cerr << account << "\n";
}
