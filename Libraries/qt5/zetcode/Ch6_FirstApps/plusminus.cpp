#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>


// Example showing how widgets can communicate with each other

class PlusMinus : public QWidget {

    // The Q_OBJECT macro is required in classes that
    // declare their own signals and slots
    Q_OBJECT

public:
    PlusMinus(QWidget* parent = nullptr);

private slots:
    void onPlus();
    void onMinus();

private:
    QLabel* label_;
};

// CMake automatically runs the `moc` tool on headers
// but for .cpp files we need to explicitly include the generated moc file
#include "plusminus.moc"

PlusMinus::PlusMinus(QWidget* parent)
    : QWidget{parent},
        label_{new QLabel{"0", this}}
{
    auto* btn_plus{new QPushButton{"+", this}};
    auto* btn_minus{new QPushButton{"-", this}};

    auto* layout{new QGridLayout{this}};
    layout->addWidget(btn_plus, 0, 0);
    layout->addWidget(btn_minus, 0, 1);
    layout->addWidget(label_, 1, 1);

    setLayout(layout);

    connect(btn_plus, &QPushButton::clicked, this, &PlusMinus::onPlus);
    connect(btn_minus, &QPushButton::clicked, this, &PlusMinus::onMinus);
}

void PlusMinus::onPlus()
{
    const auto val{label_->text().toInt() + 1};
    label_->setText(QString::number(val));
}

void PlusMinus::onMinus()
{
    const auto val{label_->text().toInt() - 1};
    label_->setText(QString::number(val));
}



int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    
    PlusMinus window;

    window.resize(300, 150);
    window.setWindowTitle("Plus minus");
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
