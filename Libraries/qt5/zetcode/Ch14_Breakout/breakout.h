#pragma once

#include <array>

#include <QWidget>
#include <QKeyEvent>

#include "ball.h"
#include "brick.h"
#include "paddle.h"


class Breakout : public QWidget
{
public:
    Breakout(QWidget* parent = nullptr);
    ~Breakout();

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void keyPressedEvent(QKeyEvent*);
    void keyReleasedEvent(QKeyEvent*);
    void drawObjects(QPainter*);
    void finishGame(QPainter*, QString);
    void moveObjects();

    void start_game();
    void pause_game();
    void stop_game();
    void victory();
    void check_collision();

private:
    static constexpr int Number_Of_Bricks_{30};
    static constexpr int Delay_{10};
    static constexpr int Bottom_Edge_{400};

    int x_{0};
    int timer_id_{};
    Ball* ball_;
    Paddle* paddle_;
    std::array<Brick*, Number_Of_Bricks_> bricks_;
    bool game_over_{false};
    bool game_won_{false};
    bool game_started_{false};
    bool paused_{false};
};
