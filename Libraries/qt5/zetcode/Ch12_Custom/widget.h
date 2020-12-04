#pragma once

#include <QFrame>

class Widget : public QFrame
{

    Q_OBJECT;

public:
    Widget(QWidget* parent = nullptr);

public slots:
    void setValue(int) noexcept;

protected:
    void paintEvent(QPaintEvent* e);
    void drawWidget(QPainter& p);

private:
    static constexpr int Distance_{19};
    static constexpr int Line_Width_{5};
    static constexpr int Divisions_{10};
    static constexpr float Full_Capacity_{700};
    static constexpr float Max_Capacity_{750};

    int cur_width_{0};
};
