#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

// QFormLayout is a simple layout manager used for input form widgets
// It's children are layed out in a two-column form
// the left column consists of labels
// the right consists of input widgets

class Form : public QWidget
{
public:
    Form(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* edt_name{new QLineEdit{this}};
        auto* edt_email{new QLineEdit{this}};
        auto* edt_age{new QLineEdit{this}};

        auto* layout{new QFormLayout{this}};
        layout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        layout->addRow("Name:", edt_name);
        layout->addRow("Email:", edt_email);
        layout->addRow("Age:", edt_age);

        setLayout(layout);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    Form window;
    window.setWindowTitle("Form Layout");
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
