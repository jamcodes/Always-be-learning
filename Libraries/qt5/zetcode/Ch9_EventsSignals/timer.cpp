#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QTime>


class Timer : public QWidget
{
public:
    Timer(QWidget* parent = nullptr)
        : QWidget{parent},
          label_{new QLabel{"", this}}
    {
        auto* hbox{new QHBoxLayout{this}};
        hbox->setSpacing(5);
        hbox->addWidget(label_, 0, Qt::AlignLeft | Qt::AlignTop);

        const QTime time{QTime::currentTime()};
        const auto str_time{time.toString()};
        label_->setText(str_time);

        startTimer(1000);
    }

protected:
    void timerEvent(QTimerEvent* e)
    {
        Q_UNUSED(e);

        const QTime time{QTime::currentTime()};
        const auto str_time{time.toString()};
        label_->setText(str_time);
    }

private:
    QLabel* label_;
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Timer window;
    window.resize(250, 150);
    window.setWindowTitle("Timer");
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
