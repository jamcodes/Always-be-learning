#include "ball.h"

#include <QCoreApplication>

#include <iostream>


Ball::Ball()
{
    auto const app_dir{QCoreApplication::applicationDirPath()};
    image_.load(app_dir + "/ball.png");
    rect_ = image_.rect();
    reset_state();
}

Ball::~Ball()
{
    std::cout << "Ball deleted" << std::endl;
}


void Ball::reset_state()
{
    rect_.moveTo(Initial_X_, Initial_Y_);
}

void Ball::auto_move()
{
    rect_.translate(x_dir_, y_dir_);
    if (rect_.left() == 0)
    {
        x_dir_ = 1;
    }
    if (rect_.right() == Right_Edge_)
    {
        x_dir_ = -1;
    }
    if (rect_.top() == 0)
    {
        y_dir_ = 1;
    }
}
