#pragma once

#include <QWidget>
#include <QSlider>
#include <QFrame>
#include "widget.h"


class Application : public QFrame
{
public:
    Application(QWidget* parent = nullptr);

private:
    void init_ui();

    QSlider* slider_{nullptr};
    Widget* widget_{nullptr};
    int cur_width_{0};
};