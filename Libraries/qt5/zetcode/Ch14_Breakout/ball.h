#pragma once

#include <QImage>
#include <QRect>


class Ball
{
public:
    Ball();
    ~Ball();

    void reset_state();
    void auto_move();
    void set_x_dir(int dir) { x_dir_ = dir; }
    void set_y_dir(int dir) { y_dir_ = dir; }
    int get_x_dir() { return x_dir_; }
    int get_y_dir() { return y_dir_; }
    QRect get_rect() { return rect_; }
    QImage& get_image() { return image_; }

private:
    static constexpr int Initial_X_{230};
    static constexpr int Initial_Y_{355};
    static constexpr int Right_Edge_{300};

    int x_dir_{1};
    int y_dir_{-1};
    QImage image_;
    QRect rect_;
};
