#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

/**
 * The `Mediator` defines an object that encapsulates how a set of objects interact. It promotes
 * loose coupling by keeping objects from referring to each other directly.
 * Mediator abstracts functionality of existing classes, all objects interact through the Mediator.
 */

class Person;

class ChatRoom {
public:
    void join(Person* p);
    void broadcast(std::string const& origin, std::string const& message);
    void message(std::string const& origin, std::string const& who, std::string const& message);

private:
    std::vector<Person*> people_{};
};

class Person {
public:
    friend ChatRoom;
    explicit Person(std::string name)
        : name_{std::move(name)}
    {
    }

    void receive(std::string const& origin, std::string const& message);
    void say(std::string const& message) const;
    void pm(std::string const& who, std::string const& message) const;

    friend bool operator==(Person const& lhs, Person const& rhs) noexcept
    {
        return lhs.name_ == rhs.name_;
    }

    friend bool operator!=(Person const& lhs, Person const& rhs) noexcept { return !(lhs == rhs); }

private:
    std::string name_;
    ChatRoom* room_{nullptr};
    std::vector<std::string> chat_log_{};
};


void ChatRoom::broadcast(std::string const& origin, std::string const& message)
{
    for (auto p : people_) {
        if (p->name_ != origin) {
            p->receive(origin, message);
        }
    }
}

void ChatRoom::join(Person* p)
{
    std::string const join_msg = p->name_ + " joins the chat";
    broadcast("room", join_msg);
    p->room_ = this;
    people_.push_back(p);
}

void ChatRoom::message(std::string const& origin, std::string const& who, std::string const& message)
{
    auto const target = std::find_if(std::begin(people_), std::end(people_),
                               [&who=who](Person const* const p){return p->name_ == who;}
                               );
    if (target != std::end(people_)) {
        (*target)->receive(origin, message);
    }
}


void Person::receive(std::string const& origin, std::string const& message)
{
    std::string const s{origin + ": \"" + message + "\""};
    std::cerr << "[" << name_ << "'s chat session] " << s << "\n";
    chat_log_.push_back(std::move(s));
}

void Person::say(std::string const& message) const { room_->broadcast(name_, message); }

void Person::pm(std::string const& who, std::string const& message) const
{
    room_->message(name_, who, message);
}


int main()
{
    ChatRoom chat;

    Person john{"John"};
    Person jane{"Jane"};
    chat.join(&john);
    chat.join(&jane);
    john.say("hi room");
    jane.say("oh, hey John");

    Person simon{"Simon"};
    chat.join(&simon);
    simon.say("hi everyone!");

    jane.pm("Simon", "glad you could join us, Simon");
}
