#include "application.h"

#include <QVBoxLayout>
#include <QHBoxLayout>


Application::Application(QWidget* parent)
    : QFrame{parent}
{
    init_ui();
}

void Application::init_ui()
{
    constexpr int Max_Value{750};
    cur_width_ = 0;
    slider_ = new QSlider{Qt::Horizontal, this};
    slider_->setMaximum(Max_Value);
    slider_->setGeometry(50, 50, 170, 30);

    widget_ = new Widget{this};
    connect(slider_, &QSlider::valueChanged, widget_, &Widget::setValue);

    auto* vbox{new QVBoxLayout{this}};
    auto* hbox{new QHBoxLayout{}};

    vbox->addStretch(1);
    hbox->addWidget(widget_, 0);

    vbox->addLayout(hbox);
    setLayout(vbox);
}
