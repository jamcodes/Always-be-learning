#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>


class BankAccount {
public:
    void deposit(int amount)
    {
        balance_ += amount;
        std::cerr << "deposited " << amount << ", balance = " << balance_ << "\n";
    }

    bool withdraw(int amount)
    {
        if (balance_ - amount >= overdraft_limit_) {
            balance_ -= amount;
            std::cerr << "withdrew " << amount << ", balance = " << balance_ << "\n";
            return true;
        }
        return false;
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
    bool succeeded;
    virtual void call() = 0;
    virtual void undo() = 0;
};

class BankAccountCommand : public Command {
public:
    enum Action { Deposit, Withdraw };

    BankAccountCommand(BankAccount& account, Action const action, int const amount)
        : account_{account}, action_{action}, amount_{amount}
    {
        Command::succeeded = false;
    }

    void call() override
    {
        switch (action_) {
        case Deposit:
            account_.deposit(amount_);
            Command::succeeded = true;
            break;
        case Withdraw:
            Command::succeeded = account_.withdraw(amount_);
        default:
            break;
        }
    }

    void undo() override
    {
        if (!Command::succeeded) return;

        switch (action_) {
        case Withdraw:
            if (Command::succeeded) {
                account_.deposit(amount_);
            }
            break;
        case Deposit:
            account_.withdraw(amount_);
            break;
        default:
            break;
        }
    }

private:
    BankAccount& account_;
    Action action_;
    int amount_;
};

class CompositeBankAccountCommand : private std::vector<BankAccountCommand>, public Command {
    using Storage = std::vector<BankAccountCommand>;
public:
    using value_type = typename Storage::value_type;
    explicit CompositeBankAccountCommand( std::initializer_list<value_type> il)
        : Storage(il)
    {
    }

    void call() override
    {
        for (auto& cmd : *this) {
            cmd.call();
        }
    }

    void undo() override
    {
        for (auto it = Storage::rbegin(); it != Storage::rend(); ++it) {
            it->undo();
        }
    }

    using Storage::begin;
    using Storage::end;
    using Storage::cbegin;
    using Storage::cend;
};

class DependentCompositeCommand : public CompositeBankAccountCommand {
public:
    using CompositeBankAccountCommand::CompositeBankAccountCommand;

    void call() override
    {
        bool ok{true};
        for (auto& cmd : *this) {
            if (ok) {
                cmd.call();
                ok = cmd.succeeded;
            }
            else {
                cmd.succeeded = false;
            }
        }
    }
};

class MoneyTransferCommand : public DependentCompositeCommand {
public:
    MoneyTransferCommand(BankAccount& from, BankAccount& to, int amount)
        : DependentCompositeCommand{
            BankAccountCommand{from, BankAccountCommand::Withdraw, amount},
            BankAccountCommand{to, BankAccountCommand::Deposit, amount}
            }
    {
    }
};


int main()
{
    BankAccount account_a{};
    BankAccount account_b{};
    
    account_a.deposit(300);
    account_b.deposit(100);
    std::cerr << "account_a: " << account_a << "\n";
    std::cerr << "account_b: " << account_b << "\n";

    MoneyTransferCommand money_transfer{account_a, account_b, 333};
    money_transfer.call();
    std::cerr << "account_a: " << account_a << "\n";
    std::cerr << "account_b: " << account_b << "\n";

    money_transfer.undo();
    std::cerr << "account_a: " << account_a << "\n";
    std::cerr << "account_b: " << account_b << "\n";
}
