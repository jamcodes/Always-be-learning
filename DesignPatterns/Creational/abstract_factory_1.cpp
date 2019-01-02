#include <iostream>
#include <memory>
#include <utility>

/**
 * The Abstract Factory design pattern provides an interface for creating families of related or
 * dependent objects without specifying their concrete classes.
 */


class Button {
public:
    virtual ~Button() noexcept = default;
    virtual void paint() const = 0;
};

class WinButton : public Button {
public:
    virtual ~WinButton() noexcept = default;
    void paint() const override { std::cout << "Windows Button \n"; }
};

class OSXButton : public Button {
public:
    virtual ~OSXButton() noexcept = default;
    void paint() const override { std::cout << "OSX Button \n"; }
};

class ScrollBar {
public:
    virtual ~ScrollBar() noexcept = default;
    virtual void paint() const = 0;
};

class WinScrollBar : public ScrollBar {
public:
    virtual ~WinScrollBar() noexcept = default;
    void paint() const override { std::cout << "Windows ScrollBar\n"; }
};

class OSXScrollBar : public ScrollBar {
public:
    virtual ~OSXScrollBar() noexcept = default;
    void paint() const override { std::cout << "OSX ScrollBar\n"; }
};


class GUIFactory {
public:
    virtual ~GUIFactory() noexcept = default;
    virtual std::unique_ptr<Button> createButton() const = 0;
    virtual std::unique_ptr<ScrollBar> createScrollBar() const = 0;
};

class WinFactory : public GUIFactory {
public:
    virtual ~WinFactory() noexcept = default;
    std::unique_ptr<Button> createButton() const override { return std::make_unique<WinButton>(); }
    std::unique_ptr<ScrollBar> createScrollBar() const override
    {
        return std::make_unique<WinScrollBar>();
    }
};

class OSXFactory : public GUIFactory {
public:
    virtual ~OSXFactory() noexcept = default;
    std::unique_ptr<Button> createButton() const override { return std::make_unique<OSXButton>(); }
    std::unique_ptr<ScrollBar> createScrollBar() const override
    {
        return std::make_unique<OSXScrollBar>();
    }
};


int main()
{
    std::unique_ptr<GUIFactory> gui_factory{};
    gui_factory = std::make_unique<OSXFactory>();
    gui_factory->createButton()->paint();
    gui_factory->createScrollBar()->paint();

    gui_factory = std::make_unique<WinFactory>();
    gui_factory->createButton()->paint();
    gui_factory->createScrollBar()->paint();
}
