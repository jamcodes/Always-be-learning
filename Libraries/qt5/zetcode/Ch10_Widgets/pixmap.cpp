#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>

class Pixmap : public QWidget
{
public:
    Pixmap(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* hbox{new QHBoxLayout{this}};
        auto app_path{QCoreApplication::applicationDirPath()};
        const QPixmap pixmap{app_path + "/assets/kiwi.png"};
        auto* label_{new QLabel{this}};
        label_->setPixmap(pixmap);

        hbox->addWidget(label_, 0, Qt::AlignTop);
        setLayout(hbox);
    }
};

int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Pixmap window;
    window.setWindowTitle("Pixmap");
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
