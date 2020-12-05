#include "brick.h"

#include <QCoreApplication>

#include <iostream>


Brick::Brick(int x, int y)
{
    auto const app_dir{QCoreApplication::applicationDirPath()};
    image_.load(app_dir + "/brick.png");
    rect_ = image_.rect();
    rect_.translate(x, y);
}

Brick::~Brick()
{
    std::cout << "Brick deleted" << std::endl;
}
