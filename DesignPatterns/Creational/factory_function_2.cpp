#include <iostream>
#include <memory>

class Button {
public:
    virtual ~Button() noexcept = default;
    virtual void print() const = 0;
};

class OSXButton : public Button {
public:
    virtual ~OSXButton() noexcept = default;
    void print() const override {
        std::cout << "OSXButton\n";
    }
};

class WindowsButton : public Button {
public:
    virtual ~WindowsButton() noexcept = default;
    void print() const override {
        std::cout << "WindowsButton\n";
    }
};

class Gui {
public:
    virtual ~Gui() noexcept = default;

    template<typename T>
    std::unique_ptr<Gui> static create()
    {
        auto p = std::make_unique<T>();
        p->post_initialize();
        return p;
    }

    virtual void press_button() = 0;
protected:
    Gui() noexcept = default;
    void post_initialize() {
        press_button();
    };
};

class WindowsGui : public Gui {
public:
    virtual ~WindowsGui() noexcept = default;
    void press_button() override {
        button_.print();
    }
private:
    WindowsButton button_{};
};

class OSXGui : public Gui {
public:
    virtual ~OSXGui() noexcept = default;
    void press_button() override {
        button_.print();
    }
private:
    OSXButton button_{};
};


int main()
{
    auto gui = Gui::create<WindowsGui>();
    gui = Gui::create<OSXGui>();
}
