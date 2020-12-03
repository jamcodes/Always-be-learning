#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QCheckBox>
#include <QHBoxLayout>


class CheckBox : public QWidget
{

    Q_OBJECT;

public:
    CheckBox(QWidget* parent = nullptr);

private slots:
    void showTitle(int);
};

#include "checkbox.moc"

CheckBox::CheckBox(QWidget* parent)
    : QWidget{parent}
{
    auto* hbox{new QHBoxLayout{this}};
    auto* cb{new QCheckBox{"Show Title", this}};
    cb->setCheckState(Qt::Checked);
    hbox->addWidget(cb, 0, Qt::AlignLeft | Qt::AlignTop);

    connect(cb, &QCheckBox::stateChanged, this, &CheckBox::showTitle);
}

void CheckBox::showTitle(int state)
{
    if (state == Qt::Checked)
    {
        setWindowTitle("QCheckBox");
    }
    else
    {
        setWindowTitle(" ");
    }
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    CheckBox window;
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
