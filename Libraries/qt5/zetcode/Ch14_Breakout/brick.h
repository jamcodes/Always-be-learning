#pragma once

#include <QImage>
#include <QRect>

class Brick
{
public:
    Brick(int, int);
    ~Brick();

    bool is_destroyed() { return destroyed_; }
    void set_destroyed(bool dstr) noexcept { destroyed_ = dstr; }
    QRect get_rect() { return rect_; }
    void set_rect(QRect rect) { rect_ = rect; }
    QImage& get_image() { return image_; }

private:
    QImage image_;
    QRect rect_;
    bool destroyed_{false};
};
