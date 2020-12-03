#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>


class ComboBox : public QWidget
{

    Q_OBJECT;

public:
    ComboBox(QWidget* parent = nullptr);

private:
    QComboBox* combo_;
    QLabel* label_;
};

#include "combobox.moc"

ComboBox::ComboBox(QWidget* parent)
    : QWidget{parent},
      combo_{new QComboBox{}},
      label_{new QLabel{"", this}}
{
    const QStringList distros{"Arch", "Xubuntu", "RedHat", "Debian", "Mandriva"};
    auto* hbox{new QHBoxLayout{this}};
    combo_->addItems(distros);
    hbox->addWidget(combo_);
    hbox->addSpacing(15);
    label_->setText(distros.front());
    hbox->addWidget(label_);

    connect(combo_, qOverload<QString const&>(&QComboBox::textActivated), label_, &QLabel::setText);
    // Equivalent call
    // connect(combo_, static_cast<void (QComboBox::*)(QString const&)>(&QComboBox::textActivated),
    //         label_, &QLabel::setText);

    setLayout(hbox);
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    ComboBox window;
    window.setWindowTitle("ComboBox");
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
