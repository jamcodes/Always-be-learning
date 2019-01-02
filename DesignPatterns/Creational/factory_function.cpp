#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

/**
 * The Factory method / member function defines an inteface for creating objects, but lets
 * subclasses decide which class to instantiate. It is also used to encapsulate virtual function
 * calls at construction. A constructor can not safely perform dynamic dispatch, as the object
 * is not yet fully constructed. To emulate dynamic dyspatch at construction an object is partially
 * instantiated in the constructor and then dynamic dispatch is done post-construction. The whole
 * operation is encapsulated in the factory member function - making it the only valid way to
 * construct an object.
 */

class Button {
public:
    virtual ~Button() noexcept = default;
    virtual void paint()  = 0;
};

class OSXButton : public Button {
public:
    virtual ~OSXButton() noexcept = default;
    void paint() override {
        std::cout << "OSX Button\n";
    }
};

class WindowsButton : public Button {
public:
    virtual ~WindowsButton() noexcept = default;
    void paint() override {
        std::cout << "Windows Button\n";
    }
};

enum class ButtonKind { OSX, Windows };

class GUIFactory {
public:
    virtual ~GUIFactory() noexcept = default;
    virtual std::unique_ptr<Button> create_Button(ButtonKind kind) const = 0;
};

class Factory : public GUIFactory {
public:
    virtual ~Factory() noexcept = default;
    std::unique_ptr<Button> create_Button(ButtonKind kind) const override {
        switch(kind)
        {
        case ButtonKind::OSX :
            return std::make_unique<OSXButton>();
            break;
        case ButtonKind::Windows :
            return std::make_unique<WindowsButton>();
            break;
        default:
            throw std::logic_error{"Unhandled button type"};
        }
    }
};



int main()
{
    std::unique_ptr<GUIFactory> guiFactory{new Factory{}};
    std::unique_ptr<Button> btn{};
    btn = guiFactory->create_Button(ButtonKind::OSX);
    btn->paint();
    btn = guiFactory->create_Button(ButtonKind::Windows);
    btn->paint();
}
