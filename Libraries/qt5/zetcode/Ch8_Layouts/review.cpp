#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>


class Review : public QWidget {
public:
    Review(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* grid{new QGridLayout{this}};
        grid->setVerticalSpacing(15);
        grid->setHorizontalSpacing(10);

        auto* lbl_title{new QLabel{"Title:", this}};
        lbl_title->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        grid->addWidget(lbl_title, 0, 0, 1, 1);
        auto* edt_title{new QLineEdit{this}};
        grid->addWidget(edt_title, 0, 1, 1, 1);

        auto* lbl_author{new QLabel{"Author:", this}};
        lbl_author->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        grid->addWidget(lbl_author, 1, 0, 1, 1);
        auto* edt_author{new QLineEdit{this}};
        grid->addWidget(edt_author, 1, 1, 1, 1);

        auto* lbl_review{new QLabel{"Review:", this}};
        lbl_review->setAlignment(Qt::AlignRight | Qt::AlignTop);
        grid->addWidget(lbl_review, 2, 0, 1, 1);
        auto* edt_review{new QTextEdit{this}};
        grid->addWidget(edt_review, 2, 1, 3, 1);

        setLayout(grid);
    }

};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Review window;
    window.setWindowTitle("Review");
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
