#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>


class Checkable : public QMainWindow
{

    Q_OBJECT

public:
    Checkable(QWidget* parent = nullptr);

private slots:
    void toggleStatusbar();

private:
    QAction* view_status_;
};


#include "checkable.moc"

Checkable::Checkable(QWidget* parent)
    : QMainWindow{parent},
      view_status_{new QAction{"&View Statusbar", this}}
{
    view_status_->setCheckable(true);
    view_status_->setChecked(true);

    auto* menu_file{menuBar()->addMenu("&File")};
    menu_file->addAction(view_status_);

    statusBar();

    connect(view_status_, &QAction::triggered, this, &Checkable::toggleStatusbar);
}

void Checkable::toggleStatusbar()
{
    if (view_status_->isChecked()) {
        statusBar()->show();
        statusBar()->showMessage("Hello!");
    }
    else {
        statusBar()->hide();
    }
}

int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    Checkable window;
    window.resize(250, 150);
    window.setWindowTitle("Checkable menu");
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
