#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QListWidget>
#include <QString>


class ListWidget : public QWidget
{

    Q_OBJECT;

public:
    ListWidget(QWidget* parent = nullptr);

private slots:
    void addItem();
    void renameItem();
    void removeItem();
    void clearItems();

private:
    QListWidget* lw_;
    QPushButton* btn_add_;
    QPushButton* btn_rename_;
    QPushButton* btn_remove_;
    QPushButton* btn_remove_all_;
};

#include "listwidget.moc"

ListWidget::ListWidget(QWidget* parent)
    : QWidget{parent},
      lw_{ new QListWidget{this}},
      btn_add_{new QPushButton{"Add", this}},
      btn_rename_{new QPushButton{"Rename", this}},
      btn_remove_{new QPushButton{"Remove", this}},
      btn_remove_all_{new QPushButton{"Remove All", this}}
{
    auto* vbox{new QVBoxLayout{}};
    vbox->setSpacing(10);
    auto* hbox{new QHBoxLayout{this}};

    lw_->addItem("The Omen");
    lw_->addItem("The Exorcist");
    lw_->addItem("Notes on a Scandal");
    lw_->addItem("Fargo");
    lw_->addItem("Capote");

    vbox->setSpacing(3);
    vbox->addStretch(1);
    vbox->addWidget(btn_add_);
    vbox->addWidget(btn_rename_);
    vbox->addWidget(btn_remove_);
    vbox->addWidget(btn_remove_all_);
    vbox->addStretch(1);

    hbox->addWidget(lw_);
    hbox->addSpacing(15);
    hbox->addLayout(vbox);

    connect(btn_add_, &QPushButton::clicked, this, &ListWidget::addItem);
    connect(btn_rename_, &QPushButton::clicked, this, &ListWidget::renameItem);
    connect(btn_remove_, &QPushButton::clicked, this, &ListWidget::removeItem);
    connect(btn_remove_all_, &QPushButton::clicked, this, &ListWidget::clearItems);

    setLayout(hbox);
}

void ListWidget::addItem()
{
    const QString c_text{QInputDialog::getText(this, "Item", "Enter new item")};
    const QString s_text{c_text.simplified()};

    if (!s_text.isEmpty())
    {
        lw_->addItem(s_text);
        const auto row{lw_->count() - 1};
        lw_->setCurrentRow(row);
    }
}
void ListWidget::renameItem()
{
    auto* it{lw_->currentItem()};
    const auto row{lw_->row(it)};
    QString c_text{it->text()};
    const QString r_text{QInputDialog::getText(this, "Item", "Enter new item", QLineEdit::Normal, c_text)};
    const QString s_text{r_text.simplified()};

    if (!s_text.isEmpty())
    {
        auto* item{lw_->takeItem(row)};
        delete item;
        lw_->insertItem(row, s_text);
        lw_->setCurrentRow(row);
    }
}
void ListWidget::removeItem()
{
    auto row{lw_->currentRow()};
    if (row != -1)
    {
        auto* item{lw_->takeItem(row)};
        delete item;
    }
}
void ListWidget::clearItems()
{
    if (lw_->count() != 0) {
        lw_->clear();
    }
}


int main(int argc, char* argv[])
{
try {
    QApplication app{argc, argv};
    ListWidget window;
    window.resize(300, 200);
    window.setWindowTitle("ListWidget");
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
