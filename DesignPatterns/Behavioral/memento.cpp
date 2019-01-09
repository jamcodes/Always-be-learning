#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>


class Memento {
public:
    explicit Memento(int balance)
        : balance_{balance} { }
    friend class BankAccount;
    friend class BankAccount2;

private:
    int balance_;
};

class BankAccount {
public:
    explicit BankAccount(int const balance)
        : balance_{balance} { }

    Memento deposit(int const amount)
    {
        balance_ += amount;
        return Memento{balance_};
    }

    void restore(Memento const& m)
    {
        balance_ = m.balance_;
    }

    friend std::ostream& operator<<(std::ostream& os, BankAccount const& obj)
    {
        return os << "balance: " << obj.balance_;
    }

private:
    int balance_;
};

// undo/redo
class BankAccount2 {
public:
    explicit BankAccount2(int const balance)
        : balance_{balance}, changes_{std::make_shared<Memento>(balance)}
    {
    }

    std::shared_ptr<Memento> deposit(int amount)
    {
        balance_ += amount;
        auto const m = std::make_shared<Memento>(balance_);
        changes_.push_back(m);
        ++current_;
        return m;
    }

    void restore(std::shared_ptr<Memento> m)
    {
        if (m) {
            balance_ = m->balance_;
            changes_.push_back(std::move(m));
            current_ = changes_.size() - 1;
        }
    }

    std::shared_ptr<Memento> undo()
    {
        if (current_ > 0) {
            --current_;
            auto const m = changes_[current_];
            balance_ = m->balance_;
            return m;
        }
        return {};
    }

    std::shared_ptr<Memento> redo()
    {
        if (current_ +1 < changes_.size()) {
            ++current_;
            auto const m = changes_[current_];
            balance_ = m->balance_;
            return m;
        }
        return {};
    }

    friend std::ostream& operator<<(std::ostream& os, BankAccount2 const& obj)
    {
        return os << "balance: " << obj.balance_;
    }

private:
    int balance_{0};
    int current_{0};
    std::vector<std::shared_ptr<Memento>> changes_;
};


void memento()
{
    BankAccount ba{100};
    auto m1 = ba.deposit(50);
    auto m2 = ba.deposit(25);
    std::cerr << ba << "\n";

    // restore state m1
    ba.restore(m1);
    std::cerr << "after restoring state after first deposit: " << ba << "\n";

    // restore state m2
    ba.restore(m2);
    std::cerr << "after restoring state after second deposit: " << ba << "\n";
}

void undo_redo()
{
    BankAccount2 ba{100};
    ba.deposit(50);
    std::cerr << "after 1st deposit: " << ba << "\n";
    ba.deposit(25);
    std::cerr << "after 2nd deposit: " << ba << "\n";

    ba.undo();
    std::cerr << "undo 1: " << ba << "\n";
    ba.undo();
    std::cerr << "undo 2: " << ba << "\n";
    ba.redo();
    std::cerr << "redo 1: " << ba << "\n";
}

int main()
{
std::cerr << "bank account 1:\n";
    memento();
std::cerr << "\nbank account 2:\n";
    undo_redo();
}
