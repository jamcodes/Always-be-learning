#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

/**
 * The Composite design pattern composes objects into tree-like structures.
 * Classes which aggregate multiple instances of other, possibly also composite,
 * classes implement the same interface, which makes it possible to treat
 * individual objects and compositions of objects uniformly.
 */

class Graphic {
public:
    Graphic() noexcept = default;
    Graphic(Graphic const&) = default;
    Graphic(Graphic&&) = default;
    Graphic& operator=(Graphic const&) = default;
    Graphic& operator=(Graphic&&) = default;
    virtual ~Graphic() noexcept = default;

    virtual void draw() const = 0;
    virtual void remove(Graphic*) { };
    virtual void add(Graphic*) { };
    virtual void get_child(int) { };
};

class Line : public Graphic {
public:
    void draw() const override {
        std::cerr << "Line::draw()\n";
    }
};

class Rectangle : public Graphic {
public:
    void draw() const override {
        std::cerr << "Rectangle::draw()\n";
    }
};

class Text : public Graphic {
public:
    void draw() const override {
        std::cerr << "Text::draw()\n";
    }
};

// Composite
class Picture : public Graphic {
public:
    void draw() const override {
        std::cerr << "Picture:\n";
        std::for_each(g_list_.cbegin(), g_list_.cend(), [](Graphic* g){g->draw();});
    }

    void add(Graphic* g) override { g_list_.push_back(g); }

private:
    std::vector<Graphic*> g_list_{};
};


int main()
{
    Line line;
    line.draw();
    Rectangle rect;
    rect.draw();
    Text text;
    text.draw();

    std::cerr << "\n";
    Picture pic;
    pic.add(&line);
    pic.add(&rect);
    pic.add(&text);
    pic.draw();

    std::cerr << "\n";
    Picture pic2;
    pic2.add(&rect);
    pic2.add(&pic);
    pic2.add(&rect);
    pic2.draw();
}
