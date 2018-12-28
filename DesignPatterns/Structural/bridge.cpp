#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>


/**
 * The bridge pattern is used to decouple an abstraction from its implementation
 * so that the two can vary independently.
 * The interface is managed by one interface hierarchy and the implementation is
 * managed by a separate inheritance hierarchy.
 */

// Implementor
class DrawingImplementor {
public:
    DrawingImplementor() noexcept = default;
    DrawingImplementor(DrawingImplementor const&) = default;
    DrawingImplementor(DrawingImplementor&&) = default;
    DrawingImplementor& operator=(DrawingImplementor const&) = default;
    DrawingImplementor& operator=(DrawingImplementor&&) noexcept = default;
    virtual ~DrawingImplementor() noexcept = default;
    virtual void drawSquare(double) = 0;
};

// Concrete implementor A
class DrawingImplementorA : public DrawingImplementor {
public:
    void drawSquare(double side) override {
        std::cerr << "ImplementorA::square with side = " << side << "\n";
    }
};

// Concrete implementor B
class DrawingImplementorB : public DrawingImplementor {
public:
    void drawSquare(double side) override {
        std::cerr << "ImplementorB.square with side = " << side << "\n";
    }
};

// Abstraction
class Shape {
public:
    Shape() noexcept = default;
    Shape(Shape const&) = default;
    Shape(Shape&&) noexcept = default;
    Shape& operator=(Shape const&) = default;
    Shape& operator=(Shape&&) noexcept = default;
    virtual ~Shape() noexcept = default;
    virtual void draw() = 0;    // low-level
    virtual void resize(double pct) = 0;    // high-level
};

// Refined Abstraction
class Square : public Shape {
public:
    Square(double s, DrawingImplementor& drawer)
        : side_{s}, drawer_{drawer}
    {
    }

    // low-level, i.e. implementation specific
    void draw() override { drawer_.drawSquare(side_); }

    // high-level, i.e. Abstraction specific
    void resize(double pct) override { side_ *= pct; }

private:
    double side_;
    DrawingImplementor& drawer_;
};

int main()
{
    DrawingImplementorA a;
    DrawingImplementorB b;

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Square>(1, a));
    shapes.push_back(std::make_unique<Square>(2, b));
    for (auto&& shape : shapes) {
        shape->draw();
    }
    shapes.front()->resize(10);
    shapes.back()->resize(10);
    for (auto&& shape : shapes) {
        shape->draw();
    }
}
