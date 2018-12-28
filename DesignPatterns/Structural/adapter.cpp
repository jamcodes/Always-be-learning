#include <iostream>
#include <memory>
#include <utility>


/**
 * The Adapter design pattern converts the interface of a class into another interface that client
 * code expects. It wraps an existing class in a class with the desired interface.
 * Can be done via inheritance by inheriting publicly from the desired interface class and privately
 * from the wrapped class. The calls to the new interface member functions are then delegated to
 * the wrapped class.
 * It provides a different interface to its subject.
 */

// Desired interface (Target)
class Rectangle {
public:
    virtual ~Rectangle() noexcept = default;
    Rectangle() noexcept = default;
    Rectangle(Rectangle const&) noexcept = default;
    Rectangle(Rectangle&&) noexcept = default;
    Rectangle& operator=(Rectangle const&) noexcept = default;
    Rectangle& operator=(Rectangle&&) noexcept = default;
    virtual void draw() = 0;
};

// Legacy component (Adaptee)
class LegacyRectangle {
public:
    LegacyRectangle(int x1, int y1, int x2, int y2)
        : x1_{x1}, y1_{y1}, x2_{x2}, y2_{y2}
    {
        std::cerr << "LegacyRectangle(" << x1 << "," << y1 << "," << x2 << "," << y2 << ")\n";
    }

    void old_draw()
    {
        std::cerr << "LegacyRectangle::old_draw()\n";
    }

private:
    int x1_;
    int y1_;
    int x2_;
    int y2_;
};

// Adapter wrapper
class RectangleAdapter : public Rectangle, private LegacyRectangle {
public:
    RectangleAdapter(int x, int y, int w, int h) noexcept
        : LegacyRectangle(x, y, x + w, y + h)
    {
        std::cerr << "RectangleAdapter(" << x << "," << y << "," << w << "," << h << ")\n";
    }

    void draw() override
    {
        std::cerr << "RectangleAdapter::draw()\n";
        old_draw();
    }
};


int main()
{
    int x = 20, y = 50, w = 300, h = 200;
    std::unique_ptr<Rectangle> rect = std::make_unique<RectangleAdapter>(x,y,w,h);
    rect->draw();
}
