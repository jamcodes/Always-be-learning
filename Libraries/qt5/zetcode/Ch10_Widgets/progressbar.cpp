#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QGridLayout>

class ProgressBar : public QWidget
{

    Q_OBJECT;

public:
    ProgressBar(QWidget* parent = nullptr);

private:
    void updateBar();
    void startMyTimer();
    void stopMyTimer();

    static constexpr int Delay{200};
    static constexpr int Max_Value{100};

    int progress_{0};
    QTimer* timer_;
    QProgressBar* pbar_;
    QPushButton* btn_start_;
    QPushButton* btn_stop_;
};

#include "progressbar.moc"

ProgressBar::ProgressBar(QWidget* parent)
    : QWidget{parent},
      timer_{new QTimer{this}},
      pbar_{new QProgressBar{}},
      btn_start_{new QPushButton{"Start", this}},
      btn_stop_{new QPushButton{"Stop", this}}
{
    connect(timer_, &QTimer::timeout, this, &ProgressBar::updateBar);

    auto* grid{new QGridLayout{this}};
    grid->setColumnStretch(2, 1);

    grid->addWidget(pbar_, 0, 0, 1, 3);

    connect(btn_start_, &QPushButton::clicked, this, &ProgressBar::startMyTimer);
    grid->addWidget(btn_start_, 1, 0, 1, 1);

    connect(btn_stop_, &QPushButton::clicked, this, &ProgressBar::stopMyTimer);
    grid->addWidget(btn_stop_, 1, 1, 1, 1);

    setLayout(grid);
}

void ProgressBar::updateBar()
{
    ++progress_;
    if (progress_ <= Max_Value)
    {
        pbar_->setValue(progress_);
    }
    else
    {
        timer_->stop();
        btn_start_->setEnabled(true);
        btn_stop_->setEnabled(false);
    }
}

void ProgressBar::startMyTimer()
{
    if (progress_ >= Max_Value)
    {
        progress_ = 0;
        pbar_->setValue(0);
    }

    if (!timer_->isActive())
    {
        btn_start_->setEnabled(false);
        btn_stop_->setEnabled(true);
        timer_->start(Delay);
    }
}

void ProgressBar::stopMyTimer()
{
    if (timer_->isActive())
    {
        btn_start_->setEnabled(true);
        btn_stop_->setEnabled(false);
        timer_->stop();
    }
}

int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    ProgressBar window;
    window.setWindowTitle("Progress Bar");
    window.resize(300, 200);
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
