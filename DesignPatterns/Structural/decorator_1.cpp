#include <iostream>
#include <string>
#include <memory>


/**
 * Decorator pattern is used to add reponsibilities (modify behavior) at runtime. It implements
 * the same interface as decorated class, but adds to it - more functionality is executed before
 * and/or after the decorated code is executed.
 * The core object can be wrapped recursively, thus adding multiple desired functionalities.
 */


class Window {
public:
    Window() = default;
    Window(Window const&) = default;
    Window(Window&&) = default;
    Window& operator=(Window const&) = default;
    Window& operator=(Window&&) = default;
    virtual ~Window() noexcept = default;
    virtual void draw() = 0;
    virtual std::string get_description() = 0;
};

// Implementation
class SimpleWindow : public Window {
public:
    void draw() override { std::cerr << "SimpleWindow::draw()\n"; }
    std::string get_description() override { return "SimpleWindow\n"; }
};

class WindowDecorator : public Window {     // implement the same interface
public:
    explicit WindowDecorator(Window* w) noexcept
        : decorated_window_{w}
    {
    }

protected:
    std::unique_ptr<Window> decorated_window_;
};

class VerticalScrollBarDecorator : public WindowDecorator {
public:
    using WindowDecorator::WindowDecorator;

    void draw() override
    {
        draw_vertical_scrollbar();
        decorated_window_->draw();
    }

    std::string get_description() override
    {
        return decorated_window_->get_description() + " with vertical scrollbars\n";
    }

private:
    void draw_vertical_scrollbar() { std::cerr << "draw_vertical_scrollbar\n"; }
};

class HorizontalScrollBarDecorator : public WindowDecorator {
public:
    using WindowDecorator::WindowDecorator;

    void draw() override
    {
        draw_horizontal_scrollbar();
        decorated_window_->draw();
    }

    std::string get_description() override
    {
        return decorated_window_->get_description() + " with horizontal scrollbars\n";
    }

private:
    void draw_horizontal_scrollbar() { std::cerr << "draw_horizontal_scrollbar\n"; }
};


int main()
{
    auto simple = std::make_unique<SimpleWindow>();
    std::cerr << simple->get_description() << "\n";
    simple->draw();

    auto horiz = std::make_unique<HorizontalScrollBarDecorator>(new SimpleWindow());
    std::cerr << horiz->get_description() << "\n";
    horiz->draw();

    auto vert = std::make_unique<VerticalScrollBarDecorator>(new SimpleWindow());
    std::cerr << vert->get_description() << "\n";
    vert->draw();

    auto decorated_window = std::make_unique<HorizontalScrollBarDecorator>(
                                new VerticalScrollBarDecorator{new SimpleWindow{}});
    std::cerr << decorated_window->get_description() << "\n";
    decorated_window->draw();
}
