#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>


class Slider : public QWidget
{

    Q_OBJECT;

public:
    Slider(QWidget* parent = nullptr);

private:
    QSlider* slider_;
    QLabel* label_;
};

#include "slider.moc"

Slider::Slider(QWidget* parent)
    : QWidget{parent},
      slider_{new QSlider{Qt::Horizontal, this}},
      label_{new QLabel{"0", this}}
{
    auto* hbox{new QHBoxLayout{this}};
    hbox->addWidget(slider_);
    hbox->addWidget(label_);
    connect(slider_, &QSlider::valueChanged, label_, qOverload<int>(&QLabel::setNum));
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Slider window;
    window.setWindowTitle("Slider");
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
