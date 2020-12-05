#include "paddle.h"

#include <QCoreApplication>

#include <iostream>


Paddle::Paddle()
{
    auto const app_dir{QCoreApplication::applicationDirPath()};
    image_.load(app_dir + "/paddle.png");
    rect_ = image_.rect();
    reset_state();
}

Paddle::~Paddle()
{
    std::cout << "Paddle deleted" << std::endl;
}


void Paddle::reset_state()
{
    rect_.moveTo(Initial_X_, Initial_Y_);
}

void Paddle::move()
{
    const int x{rect_.x() + dx_};
    const int y{rect_.top()};

    rect_.moveTo(x, y);
}
