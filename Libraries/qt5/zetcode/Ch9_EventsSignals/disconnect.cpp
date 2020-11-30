#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTextStream>

class Disconnect : public QWidget
{

    Q_OBJECT;

public:
    Disconnect(QWidget* parent = nullptr);

private slots:
    void onClick();
    void onCheck(int);

private:
    QPushButton* click_btn_;
};

#include "disconnect.moc"

Disconnect::Disconnect(QWidget* parent)
    : QWidget{parent},
      click_btn_{new QPushButton{"Click", this}}
{
    auto* hbox{new QHBoxLayout{this}};
    hbox->setSpacing(5);
    hbox->addWidget(click_btn_, 0, Qt::AlignLeft | Qt::AlignTop);

    auto* cb{new QCheckBox{"Connect", this}};
    cb->setCheckState(Qt::Checked);
    hbox->addWidget(cb, 0, Qt::AlignLeft | Qt::AlignTop);

    connect(click_btn_, &QPushButton::clicked, this, &Disconnect::onClick);
    connect(cb, &QCheckBox::stateChanged, this, &Disconnect::onCheck);

    setLayout(hbox);
}

void Disconnect::onClick()
{
    QTextStream out{stdout};
    out << "Button clicked" << Qt::endl;
}

void Disconnect::onCheck(int state)
{
    if (state == Qt::Checked)
    {
        connect(click_btn_, &QPushButton::clicked, this, &Disconnect::onClick);
    }
    else
    {
        disconnect(click_btn_, &QPushButton::clicked, this, &Disconnect::onClick);
    }
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Disconnect window;
    window.resize(250, 150);
    window.setWindowTitle("Disconnect");
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
