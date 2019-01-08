#include <iostream>

/**
 * The `Command` design pattern decouples sender from the request receiver. It encapsulates a
 * request as an object, allowing for client parameterization with different requests.
 * It can be used to implement undoable operations.
 */

class Command {
public:
    virtual void execute() = 0;
};

class Light {
public:
    void on() { std::cerr << "The light is on\n"; }
    void off() { std::cerr << "The light is off\n"; }
};

class LightOnCommand : public Command {
public:
    explicit LightOnCommand(Light& light) noexcept
        : light_{light}
    {
    }

    void execute() override { light_.on(); }

private:
    Light& light_;
};

class LightOffCommand : public Command {
public:
    explicit LightOffCommand(Light& light) noexcept
        : light_{light}
    {
    }

    void execute() override { light_.off(); }

private:
    Light& light_;
};

// Invoker - stores a concrete Command object
class RemoteControl {
public:
    void set_command(Command* cmd) noexcept { cmd_ = cmd; }

    void button_pressed() { cmd_->execute(); }

private:
    Command* cmd_;
};


int main()
{
    Light light;
    LightOnCommand light_on{light};
    LightOffCommand light_off{light};

    RemoteControl control;
    control.set_command(&light_on);
    control.button_pressed();
    control.set_command(&light_off);
    control.button_pressed();
}
