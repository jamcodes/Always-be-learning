#include "snake.h"

#include <QCoreApplication>
#include <QPainter>
#include <QTimer>
#include <QTime>


Snake::Snake(QWidget* parent)
    : QWidget{parent}
{
    setStyleSheet("background-color:black;");
    setFixedSize(B_Width_, B_Height_);
    load_images();
    init_game();
    const QTime time{QTime::currentTime()};
    rand_ = QRandomGenerator{static_cast<quint32>(time.msec())};
}

void Snake::paintEvent(QPaintEvent* /* e */)
{
    do_draw();
}

void Snake::timerEvent(QTimerEvent* /* e */)
{
    if (in_game_)
    {
        check_apple();
        check_collision();
        move();
    }

    repaint();
}

void Snake::keyPressEvent(QKeyEvent* e)
{
    const int key{e->key()};
    if ((key == Qt::Key_Left) && !right_direction_)
    {
        left_direction_ = true;
        up_direction_ = false;
        down_direction_ = false;
    }
    if ((key == Qt::Key_Right) && !left_direction_)
    {
        right_direction_ = true;
        up_direction_ = false;
        down_direction_ = false;
    }
    if ((key == Qt::Key_Up) && !down_direction_)
    {
        up_direction_ = true;
        right_direction_ = false;
        left_direction_ = false;
    }
    if ((key == Qt::Key_Down) && !up_direction_)
    {
        down_direction_ = true;
        right_direction_ = false;
        left_direction_ = false;
    }

    QWidget::keyPressEvent(e);
}


void Snake::load_images()
{
    const QString app_path{QCoreApplication::applicationDirPath()};
    dot_.load(app_path + "/dot.png");
    head_.load(app_path + "/head.png");
    apple_.load(app_path + "/apple.png");
}

void Snake::init_game()
{
    for (int z{0}; z < dots_; ++z)
    {
        x_[z] = 50 - z * 10;
        y_[z] = 50;
    }

    locate_apple();
    timer_id_ = startTimer(Delay_);
}

void Snake::locate_apple()
{
    apple_x_ = rand_.bounded(Rand_Pos_) * Dot_Size_;
    apple_y_ = rand_.bounded(Rand_Pos_) * Dot_Size_;
}

void Snake::check_apple()
{
    if ((x_[0] == apple_x_) && (y_[0] == apple_y_))
    {
        ++dots_;
        locate_apple();
    }
}

void Snake::check_collision()
{
    for (int z{dots_}; z != 0; --z)
    {
        if ((z > 4) && (x_[0] == x_[z]) && (y_[0] == y_[z]))
        {
            in_game_ = false;
        }
    }

    if (y_[0] >= B_Height_)
    {
        in_game_ = false;
    }

    if (y_[0] < 0)
    {
        in_game_ = false;
    }

    if (x_[0] >= B_Width_)
    {
        in_game_ = false;
    }

    if (x_[0] < 0)
    {
        in_game_ = false;
    }

    if (!in_game_)
    {
        killTimer(timer_id_);
    }
}

void Snake::move()
{
    for (int z{dots_}; z != 0; --z)
    {
        x_[z] = x_[z - 1];
        y_[z] = y_[z - 1];
    }

    if (left_direction_)
    {
        x_[0] -= Dot_Size_;
    }

    if (right_direction_)
    {
        x_[0] += Dot_Size_;
    }

    if (up_direction_)
    {
        y_[0] -= Dot_Size_;
    }

    if (down_direction_)
    {
        y_[0] += Dot_Size_;
    }
}

void Snake::do_draw()
{
    QPainter p{this};
    if (in_game_)
    {
        p.drawImage(apple_x_, apple_y_, apple_);

        for (int z{0}; z < dots_; ++z)
        {
            if (z == 0)
            {
                p.drawImage(x_[z], y_[z], head_);
            }
            else
            {
                p.drawImage(x_[z], y_[z], dot_);
            }
        }
    }
    else
    {
        game_over(p);
    }
}

void Snake::game_over(QPainter& p)
{
    const QString msg{"Game Over"};
    const QFont font{"Courier", 15, QFont::DemiBold};
    const QFontMetrics fm{font};
    const int text_width{fm.horizontalAdvance(msg)};

    p.setFont(font);
    const auto h{height()};
    const auto w{width()};
    p.translate(QPoint(w/2, h/2));
    p.drawText(-text_width/2, 0, msg);
}
