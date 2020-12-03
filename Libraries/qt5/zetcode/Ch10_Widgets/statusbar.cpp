#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QStatusBar>
#include <QHBoxLayout>


class StatusBar : public QMainWindow
{

    Q_OBJECT;

public:
    StatusBar(QWidget* parent = nullptr);

private slots:
    void onOkPressed();
    void onApplyPressed();

private:
    QPushButton* btn_ok_;
    QPushButton* btn_apply_;
};

#include "statusbar.moc"

StatusBar::StatusBar(QWidget* parent)
    : QMainWindow{parent}
{
    auto* frame{new QFrame{this}};
    setCentralWidget(frame);

    btn_ok_ = new QPushButton{"OK", frame};
    btn_apply_ = new QPushButton{"Apply", frame};

    auto* hbox{new QHBoxLayout{frame}};
    hbox->addWidget(btn_ok_, 0, Qt::AlignLeft | Qt::AlignTop);
    hbox->addWidget(btn_apply_, 1, Qt::AlignLeft | Qt::AlignTop);

    statusBar();

    connect(btn_ok_, &QPushButton::clicked, this, &StatusBar::onOkPressed);
    connect(btn_apply_, &QPushButton::clicked, this, &StatusBar::onApplyPressed);

    setLayout(hbox);
}

void StatusBar::onOkPressed()
{
    statusBar()->showMessage("OK button pressed", 2000);
}
void StatusBar::onApplyPressed()
{
    statusBar()->showMessage("Apply button pressed", 2000);
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    StatusBar window;
    window.setWindowTitle("StatusBar");
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
