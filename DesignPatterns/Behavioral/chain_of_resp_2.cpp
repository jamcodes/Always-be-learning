#include <iostream>
#include <string>
#include <utility>


struct Creature {
public:
    std::string name;
    int attack;
    int defence;

    Creature(std::string name_, const int attack_, const int defence_)
        : name{std::move(name_)}, attack{attack_}, defence{defence_}
    {
    }

    friend std::ostream& operator<<(std::ostream& os, Creature const& obj)
    {
        return os
            << "name: " << obj.name
            << ", attack: " << obj.attack
            << ", defence: " << obj.defence;
    }
};

class CreatureModifier {
public:
    explicit CreatureModifier(Creature& creature)
        : creature_{creature}
    {
    }

    virtual ~CreatureModifier() = default;

    void add(CreatureModifier* cm)
    {
        if (next_) {
            next_->add(cm);
        }
        else {
            next_ = cm;
        }
    }

// Two possible approaches
// 1. Allways call base handle()
// 2. Only call base handle if the current object doesn't handle the action itself

    virtual void handle()
    {
        if (next_) {
            next_->handle();
        }
    }

protected:
    Creature& creature_;
private:
    CreatureModifier* next_{nullptr};
};

class NoBonusesModifier : public CreatureModifier {
public:
    explicit NoBonusesModifier(Creature& cr)
        : CreatureModifier(cr)
    {
    }

    void handle() override { /* nop */ }
};

class DoubleAttachModifier : public CreatureModifier {
public:
    using CreatureModifier::CreatureModifier;

    void handle() override
    {
        creature_.attack *= 2;
        CreatureModifier::handle();
    }
};

class IncreaseDefenceModifier : public CreatureModifier {
public:
    using CreatureModifier::CreatureModifier;

    void handle() override
    {
        if (creature_.attack <= 2) {
            creature_.defence += 1;
        }
        CreatureModifier::handle();
    }
};


int main()
{
    Creature goblin{"Goblin", 1, 1};
    CreatureModifier root{goblin};
    DoubleAttachModifier r1{goblin};
    DoubleAttachModifier r1_2{goblin};
    IncreaseDefenceModifier r2{goblin};

    root.add(&r1);
    root.add(&r1_2);
    root.add(&r2);

    root.handle();

    std::cerr << goblin << "\n";
}
