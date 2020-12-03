#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>

class SpinBox : public QWidget
{

    Q_OBJECT;

public:
    SpinBox(QWidget* parent = nullptr);

private:
    QSpinBox* spinbox_;
};

#include "spinbox.moc"

SpinBox::SpinBox(QWidget* parent)
    : QWidget{parent},
      spinbox_{new QSpinBox{this}}
{
    auto* hbox{new QHBoxLayout{this}};
    hbox->setSpacing(15);

    auto* lbl{new QLabel{"0", this}};

    hbox->addWidget(spinbox_);
    hbox->addWidget(lbl);

    connect(spinbox_, qOverload<int>(&QSpinBox::valueChanged), lbl, qOverload<int>(&QLabel::setNum));

    setLayout(hbox);
}

int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    SpinBox window;
    window.setWindowTitle("SpinBox");
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
