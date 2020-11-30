#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QToolBar>


class Toolbar : public QMainWindow {

    Q_OBJECT;

public:
    Toolbar(QWidget* parent = nullptr);
};


#include "toolbar.moc"


Toolbar::Toolbar(QWidget* parent)
    : QMainWindow{parent}
{
    auto app_path{QCoreApplication::applicationDirPath()};
    QPixmap pix_new{app_path + "/assets/018-add_file.png"};
    QPixmap pix_open{app_path + "/assets/044-open.png"};
    QPixmap pix_quit{app_path + "/assets/logout.png"};

    auto* toolbar{addToolBar("main toolbar")};
    toolbar->addAction(QIcon{pix_new}, "New File");
    toolbar->addAction(QIcon{pix_open}, "Open File");
    toolbar->addSeparator();
    auto* act_quit{toolbar->addAction(QIcon{pix_quit}, "Quit Application")};

    connect(act_quit, &QAction::triggered, qApp, &QApplication::quit);
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Toolbar window;
    window.resize(350, 200);
    window.setWindowTitle("Toolbar");
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
