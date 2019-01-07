#include <iostream>
#include <vector>

enum class Receiver {
    Light, Fan, /* Oven, Door, */ None, Count
};

class Command {
public:
    virtual void execute() = 0;
};

class Light {
public:
    void on() { std::cerr << "The light is on\n"; }
    void off() { std::cerr << "The light is off\n"; }
};

class Fan {
public:
    void on() { std::cerr << "The fan is on\n"; }
    void off() { std::cerr << "The fan is off\n"; }
};

class NullCommand : public Command {
public:
    void execute() { std::cerr << "Null command: does nothing\n"; }
};

class LightOnCommand : public Command {
public:
    explicit LightOnCommand(Light& light)
        : light_{light}
    {
    }

    void execute() override { light_.on(); }

private:
    Light& light_;
};

class LightOffCommand : public Command {
public:
    explicit LightOffCommand(Light& light)
        : light_{light}
    {
    }

    void execute() override { light_.off(); }

private:
    Light& light_;
};

class FanOnCommand : public Command {
public:
    explicit FanOnCommand(Fan& fan)
        : fan_{fan}
    {
    }

    void execute() override { fan_.on(); }

private:
    Fan& fan_;
};

class FanOffCommand : public Command {
public:
    explicit FanOffCommand(Fan& fan)
        : fan_{fan}
    {
    }

    void execute() override { fan_.off(); }

private:
    Fan& fan_;
};


class RemoteControl {
public:
    void push_command(Receiver idx, Command* on_cmd, Command* off_cmd)
    {
        on_commands_[static_cast<std::size_t>(idx)] = on_cmd;
        off_commands_[static_cast<std::size_t>(idx)] = off_cmd;
    }
    void on_button_pressed(Receiver idx)
    {
        on_commands_[static_cast<std::size_t>(idx)]->execute();
    }
    void off_button_pressed(Receiver idx)
    {
        off_commands_[static_cast<std::size_t>(idx)]->execute();
    }
private:
    NullCommand null_cmd_{};
    std::vector<Command*> on_commands_{static_cast<std::size_t>(Receiver::Count), &null_cmd_};
    std::vector<Command*> off_commands_{static_cast<std::size_t>(Receiver::Count), &null_cmd_};
};


int main()
{
    Light light;
    Fan fan;

    LightOnCommand light_on{light};
    LightOffCommand light_off{light};
    FanOnCommand fan_on{fan};
    FanOffCommand fan_off{fan};
    NullCommand null_on{};
    NullCommand null_off{};

    RemoteControl control;

    control.push_command(Receiver::Light, &light_on, &light_off);
    control.on_button_pressed(Receiver::Light);
    control.off_button_pressed(Receiver::Light);

    control.push_command(Receiver::Fan, &fan_on, &fan_off);
    control.on_button_pressed(Receiver::Fan);
    control.off_button_pressed(Receiver::Fan);
}
