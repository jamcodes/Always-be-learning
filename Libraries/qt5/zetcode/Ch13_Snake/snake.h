#pragma once

#include <array>

#include <QWidget>
#include <QKeyEvent>
#include <QRandomGenerator>


class Snake : public QWidget
{
public:
    Snake(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    void load_images();
    void init_game();
    void locate_apple();
    void check_apple();
    void check_collision();
    void move();
    void do_draw();
    void game_over(QPainter&);

    static constexpr int B_Width_{300};
    static constexpr int B_Height_{300};
    static constexpr int Dot_Size_{10};
    static constexpr int All_Dots_{900};
    static constexpr int Rand_Pos_{29};
    static constexpr int Delay_{140};

    QImage dot_;
    QImage head_;
    QImage apple_;

    int timer_id_;
    int dots_{3};
    int apple_x_;
    int apple_y_;

    std::array<int, All_Dots_> x_;
    std::array<int, All_Dots_> y_;

    bool left_direction_{false};
    bool right_direction_{true};
    bool up_direction_{false};
    bool down_direction_{false};
    bool in_game_{true};
    QRandomGenerator rand_;
};
