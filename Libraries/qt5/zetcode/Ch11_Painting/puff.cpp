#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QTextStream>


class Puff : public QWidget
{
public:
    Puff(QWidget* parent = nullptr)
        : QWidget{parent},
          timer_id_{startTimer(15)}
    {
    }

protected:
    void paintEvent(QPaintEvent* /* e */)
    {
        doPaint();
    }

    void timerEvent(QTimerEvent* /* e */)
    {
        // ++x_;
        op_(*this);
        if (opacity_ <= 0) {
            op_ = Puff::shrink;
        }
        else if (opacity_ >= 1.0) {
            op_ = Puff::grow;
        }
        repaint();
    }

private:
    static void grow(Puff& p) noexcept
    {
        ++p.x_;
        if (p.x_ > 10) {
            p.opacity_ -= 0.01;
        }
    }

    static void shrink(Puff& p) noexcept
    {
        --p.x_;
        p.opacity_ += 0.01;
    }

    void doPaint()
    {
        QPainter p{this};
        QTextStream out{stdout};

        const QString text{"Jamcodes"};
        p.setPen(QPen{QBrush{"#575555"}, 1});

        const QFont font{"Courier", x_, QFont::DemiBold};
        const QFontMetrics fm{font};
        int text_width{fm.horizontalAdvance(text)};
        p.setFont(font);

        // if (x_ > 10)
        // {
        //     opacity_ -= 0.01;
        //     p.setOpacity(opacity_);
        // }
        p.setOpacity(opacity_);

        // if (opacity_ <= 0)
        // {
        //     killTimer(timer_id_);
        //     out << "timer stopped" << Qt::endl;
        // }

        const int h{height()};
        const int w{width()};
        p.translate(QPoint{w/2, h/2});
        p.drawText(-text_width/2, 0, text);
    }

    using Callback = void (*)(Puff&) noexcept;
    Callback op_{Puff::grow};
    int x_{1};
    qreal opacity_{1.0};
    int timer_id_;
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    Puff window;
    window.resize(350, 280);
    window.setWindowTitle("Puff");
    window.show();
    return app.exec();
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
