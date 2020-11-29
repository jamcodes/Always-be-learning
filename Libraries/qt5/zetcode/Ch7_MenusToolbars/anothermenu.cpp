#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>


class AnotherMenu : public QMainWindow
{
public:
    AnotherMenu(QWidget* parent = nullptr)
        : QMainWindow{parent}
    {
        auto app_path{QCoreApplication::applicationDirPath()};
        QPixmap pix_new{app_path + "/assets/018-add_file.png"};
        QPixmap pix_open{app_path + "/assets/044-open.png"};
        QPixmap pix_quit{app_path + "/assets/logout.png"};

        auto* act_new{new QAction{pix_new, "&New", this}};
        auto* act_open{new QAction{pix_open, "&Open", this}};
        auto* act_quit{new QAction{pix_quit, "&Quit", this}};
        act_quit->setShortcut(tr("CTRL+Q"));

        QMenu* menu_file{menuBar()->addMenu("&File")};
        menu_file->addAction(act_new);
        menu_file->addAction(act_open);
        menu_file->addSeparator();
        menu_file->addAction(act_quit);

        qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);

        connect(act_quit, &QAction::triggered, qApp, QApplication::quit);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    AnotherMenu window;
    window.resize(350, 200);
    window.setWindowTitle("Another menu");
    window. show();

    return app.exec();
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
