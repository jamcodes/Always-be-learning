#include <algorithm>
#include <iostream>
#include <utility>
#include <QTextStream>


class Book {
public:
    Book(QString author, QString title)
        : author_{std::move(author)},
          title_{std::move(title)}
    {
    }

    QString const& getAuthor() const& noexcept { return author_; }
    QString getAuthor() && noexcept { return std::move(author_); }

    QString const& getTitle() const& noexcept { return title_; }
    QString getTitle() && noexcept { return std::move(title_); }

private:
    QString author_;
    QString title_;
};


int main()
{
try {
    QTextStream out{stdout};

    QList<Book> books = {
        Book("Jack London", "The Call of the Wild"),
        Book("Honoré de Balzac", "Father Goriot"),
        Book("Leo Tolstoy", "War and Peace"),
        Book("Gustave Flaubert", "Sentimental education"),
        Book("Guy de Maupassant", "Une vie"),
        Book("William Shakespeare", "Hamlet")
    };

    auto by_title = [](Book const& lhs, Book const& rhs)noexcept {return lhs.getTitle() == rhs.getTitle(); };
    std::sort(books.begin(), books.end(), by_title);

    for (auto const& b : books) {
        out << b.getAuthor() << ": " << b.getTitle() << Qt::endl;
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
