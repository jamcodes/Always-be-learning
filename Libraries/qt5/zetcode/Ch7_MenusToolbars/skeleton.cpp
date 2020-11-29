#include "skeleton.h"

#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>


Skeleton::Skeleton(QWidget* parent)
    : QMainWindow{parent}
{
    auto app_path{QCoreApplication::applicationDirPath()};
    QPixmap pix_new{app_path + "/assets/018-add_file.png"};
    QPixmap pix_open{app_path + "/assets/044-open.png"};
    QPixmap pix_quit{app_path + "/assets/logout.png"};

    auto* act_quit{new QAction{"&Quit", this}};

    auto* mnu_file{menuBar()->addMenu("&File")};
    mnu_file->addAction(act_quit);
    connect(act_quit, &QAction::triggered, qApp, &QApplication::quit);

    auto* toolbar{addToolBar("main toolbar")};
    toolbar->addAction(QIcon{pix_new}, "New File");
    toolbar->addAction(QIcon{pix_open}, "Open File");
    toolbar->addSeparator();
    auto* act_tb_quit{toolbar->addAction(QIcon{pix_quit}, "Quit Application")};
    connect(act_tb_quit, &QAction::triggered, qApp, &QApplication::quit);

    auto* edit{new QTextEdit{this}};
    setCentralWidget(edit);
    statusBar()->showMessage("Ready");
}
