#include "widget.h"

// #include "application.h"

#include <QtGui>

#include <array>

namespace
{
constexpr int Panel_Height{30};
} // namespace

Widget::Widget(QWidget* parent)
    : QFrame{parent}
{
    setMinimumHeight(Panel_Height);
}

void Widget::setValue(int value) noexcept
{
    cur_width_ = value;
    repaint();
}

void Widget::paintEvent(QPaintEvent* e)
{
    QPainter p{this};
    drawWidget(p);
    QFrame::paintEvent(e);
}

void Widget::drawWidget(QPainter& p)
{
    const std::array<QString, 9> num{{"75", "150", "225", "300", "375", "450", "525", "600", "675"}};
    const QColor color_red{255, 175, 175};
    const QColor color_yellow{255, 255, 184};

    const auto width{size().width()};
    const int step{static_cast<int>(qRound(static_cast<double>(width) / Divisions_))};
    const int till{static_cast<int>((width / Max_Capacity_) * cur_width_)};
    const int full{static_cast<int>((width / Max_Capacity_) * Full_Capacity_)};

    if (cur_width_ >= Full_Capacity_)
    {
        p.setPen(color_yellow);
        p.setBrush(color_yellow);
        p.drawRect(0, 0, full, 30);
        p.setPen(color_red);
        p.setBrush(color_red);
        p.drawRect(full, 0, till - full, Panel_Height);

    }
    else if (till > 0)
    {
        p.setPen(color_yellow);
        p.setBrush(color_yellow);
        p.drawRect(0, 0, till, Panel_Height);
    }

    const QColor color_gray{90, 80, 60};
    p.setPen(color_gray);

    for (int i{1}; i < num.size(); ++i)
    {
        p.drawLine(i * step, 0, i*step, Line_Width_);
        QFont new_font{font()};
        new_font.setPointSize(8);
        setFont(new_font);

        const QFontMetrics fm{font()};
        const auto w{fm.horizontalAdvance(num[i-1])};
        p.drawText(i*step-w/2, Distance_, num[i-1]);
    }
}
