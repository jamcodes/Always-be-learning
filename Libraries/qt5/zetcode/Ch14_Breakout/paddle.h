#pragma once

#include <QImage>
#include <QRect>


class Paddle
{
public:
    Paddle();
    ~Paddle();

    void reset_state();
    void move();
    void set_dx(int x) { dx_ = x; }
    QRect get_rect() { return rect_; }
    QImage& get_image() { return image_; }

private:
    static constexpr int Initial_X_{200};
    static constexpr int Initial_Y_{360};

    int dx_{0};
    QImage image_;
    QRect rect_;
};
