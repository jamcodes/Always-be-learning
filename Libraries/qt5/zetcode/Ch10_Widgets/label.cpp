#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>


class Label : public QWidget
{
public:
    Label(QWidget* parent = nullptr)
        : QWidget{parent}
    {
  const QString lyrics = "Who doesn't long for someone to hold\n\
Who knows how to love you without being told\n\
Somebody tell me why I'm on my own\n\
If there's a soulmate for everyone\n\
\n\
Here we are again, circles never end\n\
How do I find the perfect fit\n\
There's enough for everyone\n\
But I'm still waiting in line\n\
\n\
Who doesn't long for someone to hold\n\
Who knows how to love you without being told\n\
Somebody tell me why I'm on my own\n\
If there's a soulmate for everyone";
        label_ = new QLabel{lyrics, this};
        label_->setFont(QFont{"Purisa", 10});

        auto* vbox{new QVBoxLayout{}};
        vbox->addWidget(label_);
        setLayout(vbox);
    }

private:
    QLabel* label_;
};


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};

    Label window;
    window.setWindowTitle("Label");
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
