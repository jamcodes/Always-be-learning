#include <iostream>
#include <string>
#include <vector>


// two classes of objects
// Renderers - determine how an object is drawn
// Shapes - determine what to draw


class Renderer {
public:
    virtual ~Renderer() noexcept = default;
    virtual void render_circle(float x, float y, float radius) = 0;
};

class VectorRenderer : public Renderer {
public:
    void render_circle(float x, float y, float r) override
    {
        std::cerr << "Drawing a vector circle at {" << x << "," << y
            << "}, of radius " << r << "\n";
    }
};

class RasterRenderer : public Renderer {
public:
    void render_circle(float x, float y, float r) override
    {
        std::cerr << "Rasterizing circle at {" << x << "," << y
            << "}, of radius " << r << "\n";
    }
};

class Shape {
public:
    virtual void draw() = 0;    // implementation specific
    virtual void resize(float factor) = 0;  // abstraction specific
};

class Circle : public Shape {
public:
    Circle(Renderer& renderer, float x, float y, float radius)
        : renderer_{renderer}, x_{x}, y_{y}, radius_{radius}
    {
    }

    void draw() override { renderer_.render_circle(x_, y_, radius_); }
    void resize(float factor) override { radius_ *= factor; }
private:
    Renderer& renderer_;
    float x_, y_, radius_;
};


int main()
{
    RasterRenderer rr;
    VectorRenderer vr;
    Circle rc{rr, 5,5,5};
    Circle vc{vr, 1,2,3};
    rc.draw();
    vc.draw();
    rc.resize(10);
    vc.resize(10);
    rc.draw();
    vc.draw();
}
