#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QPushButton>


class VerticalBox : public QWidget {
public:
    VerticalBox(QWidget* parent = nullptr)
        : QWidget{parent}
    {
        auto* layout{new QVBoxLayout{this}};
        layout->setSpacing(1);

        auto* btn_settings{new QPushButton{"Settings", this}};
        // make the button expendable in both directions
        btn_settings->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        auto* btn_accounts{new QPushButton{"Accounts", this}};
        auto* btn_loans{new QPushButton{"Accounts", this}};
        auto* btn_cash{new QPushButton{"Cash", this}};
        auto* btn_debts{new QPushButton{"Debts", this}};
        btn_accounts->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn_loans->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn_cash->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn_debts->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout->addWidget(btn_settings);
        layout->addWidget(btn_accounts);
        layout->addWidget(btn_loans);
        layout->addWidget(btn_cash);
        layout->addWidget(btn_debts);

        setLayout(layout);
    }
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    VerticalBox window;
    window.resize(240, 230);
    window.setWindowTitle("VerticalBox Layout");
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
