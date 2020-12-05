#include "breakout.h"

#include <QPainter>
#include <QApplication>


Breakout::Breakout(QWidget* parent)
    : QWidget{parent},
      ball_{new Ball{}},
      paddle_{new Paddle{}}
{
    int k{0};
    for (int i{0}; i < 5; ++i)
    {
        for (int j{0}; j < 6; ++j)
        {
            bricks_[k] = new Brick(j * 40 + 30, i * 10 + 50);
            ++k;
        }
    }
}
Breakout::~Breakout()
{
    delete ball_;
    delete paddle_;
    for (int i{0}; i < Number_Of_Bricks_; ++i)
    {
        delete bricks_[i];
    }
}

void Breakout::paintEvent(QPaintEvent* /* e */)
{
    QPainter p{this};

    if (game_over_)
    {
        finishGame(&p, "Game lost");
    }
    else if (game_won_)
    {
        finishGame(&p, "Victoy");
    }
    else
    {
        drawObjects(&p);
    }
}
void Breakout::timerEvent(QTimerEvent* /* e */)
{
    moveObjects();
    check_collision();
    repaint();
}
void Breakout::keyPressedEvent(QKeyEvent* e)
{
    switch(e->key())
    {
    case Qt::Key_Left:
    {
        paddle_->set_dx(-1);
        break;
    }
    case Qt::Key_Right:
    {
        paddle_->set_dx(1);
        break;
    }
    case Qt::Key_P:
    {
        pause_game();
        break;
    }
    case Qt::Key_Space:
    {
        start_game();
        break;
    }
    case Qt::Key_Escape:
    {
        qApp->exit();
        break;
    }
    default: QWidget::keyPressEvent(e);
    }
}
void Breakout::keyReleasedEvent(QKeyEvent* e)
{
    switch(e->key())
    {
    case Qt::Key_Left: case Qt::Key_Right:
    {
        paddle_->set_dx(0);
        break;
    }
    }
}

void Breakout::drawObjects(QPainter* p)
{
    p->drawImage(ball_->get_rect(), ball_->get_image());
    p->drawImage(paddle_->get_rect(), paddle_->get_image());

    for (int i{0}; i != Number_Of_Bricks_; ++i)
    {
        auto* const brick{bricks_[i]};
        if (!brick->is_destroyed())
        {
            p->drawImage(brick->get_rect(), brick->get_image());
        }
    }
}

void Breakout::finishGame(QPainter* p, QString msg)
{
    const QFont font{"Courier", QFont::DemiBold};
    const QFontMetrics fm{font};
    int text_width{fm.horizontalAdvance(msg)};
    p->setFont(font);
    const auto h{height()};
    const auto w{width()};
    p->translate(QPoint{w/2, h/2});
    p->drawText(-text_width/2, 0, msg);
}

void Breakout::moveObjects()
{
    ball_->auto_move();
    paddle_->move();
}

void Breakout::start_game()
{
    if (!game_started_)
    {
        ball_->reset_state();
        paddle_->reset_state();

        for (int i{0}; i != Number_Of_Bricks_; ++i)
        {
            bricks_[i]->set_destroyed(false);
        }

        game_over_ = false;
        game_won_ = false;
        game_started_ = true;
        timer_id_ = startTimer(Delay_);
    }
}
void Breakout::pause_game()
{
    if (paused_)
    {
        timer_id_ = startTimer(Delay_);
        paused_ = false;
    }
    else
    {
        paused_ = true;
        killTimer(timer_id_);
    }
}
void Breakout::stop_game()
{
    killTimer(timer_id_);
    game_over_ = true;
    game_started_ = false;
}

void Breakout::victory()
{
    killTimer(timer_id_);
    game_won_ = true;
    game_started_ = false;
}

void Breakout::check_collision()
{
    if (ball_->get_rect().bottom() > Bottom_Edge_)
    {
        stop_game();
    }

    int destroyed_count{0};
    for (int i{0}; i != Number_Of_Bricks_; ++i)
    {
        if (bricks_[i]->is_destroyed())
        {
            ++destroyed_count;
        }
    }
    if (destroyed_count == Number_Of_Bricks_)
    {
        victory();
    }

    if (ball_->get_rect().intersects(paddle_->get_rect()))
    {
        const int paddle_l_pos{paddle_->get_rect().left()};
        const int ball_l_pos{ball_->get_rect().left()};

        const int first{paddle_l_pos + 8};
        const int second{paddle_l_pos + 16};
        const int third{paddle_l_pos + 24};
        const int fourth{paddle_l_pos + 32};

        if (ball_l_pos < first)
        {
            ball_->set_x_dir(-1);
            ball_->set_y_dir(-1);
        }
        if (ball_l_pos >= first && ball_l_pos < second)
        {
            ball_->set_x_dir(-1);
            ball_->set_y_dir(-1 * ball_->get_y_dir());
        }
        if (ball_l_pos >= second && ball_l_pos < third)
        {
            ball_->set_x_dir(0);
            ball_->set_y_dir(-1);
        }
        if (ball_l_pos >= third && ball_l_pos < fourth)
        {
            ball_->set_x_dir(1);
            ball_->set_y_dir(-1 * ball_->get_y_dir());
        }
        if (ball_l_pos > fourth)
        {
            ball_->set_x_dir(1);
            ball_->set_y_dir(-1);
        }
    }

    for (int i{0}; i != Number_Of_Bricks_; ++i)
    {
        auto* const brick{bricks_[i]};
        if (ball_->get_rect().intersects(brick->get_rect()))
        {
            const int ball_left{ball_->get_rect().left()};
            const int ball_height{ball_->get_rect().height()};
            const int ball_width{ball_->get_rect().width()};
            const int ball_top{ball_->get_rect().top()};

            const QPoint point_right{ball_left + ball_width + 1, ball_top};
            const QPoint point_left{ball_left - 1, ball_top};
            const QPoint point_top{ball_left, ball_top - 1};
            const QPoint point_bottom{ball_left, ball_top + ball_height + 1};

            if (!brick->is_destroyed())
            {
                if (brick->get_rect().contains(point_right))
                {
                    ball_->set_x_dir(-1);
                }
                else if (brick->get_rect().contains(point_left))
                {
                    ball_->set_x_dir(1);
                }
                if (brick->get_rect().contains(point_top))
                {
                    ball_->set_y_dir(1);
                }
                else if (brick->get_rect().contains(point_bottom))
                {
                    ball_->set_y_dir(-1);
                }
                brick->set_destroyed(true);
            }
        }
    }
}
