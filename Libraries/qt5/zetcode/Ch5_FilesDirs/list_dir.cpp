#include <algorithm>
#include <iostream>

#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QDir>

int main(int argc, char* argv[])
{
try {
    QTextStream out{stdout};

    if (argc != 2) {
        out << "Usage: list_dir <directory>" << Qt::endl;
        return 1;
    }

    QString dirname{argv[1]};
    QDir dir{dirname};

    if (!dir.exists()) {
        qWarning("The directory does not exist");
        return 1;
    }

    dir.setFilter(QDir::Files | QDir::AllDirs);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList finfo_list{dir.entryInfoList()};

    auto max_size{
        std::accumulate(finfo_list.cbegin(), finfo_list.cend(), finfo_list.front().fileName().size(),
                        [](auto acc, auto val) noexcept { return std::max(acc, val.fileName().size()); })
    };

    const int len{max_size + 2};
    out << QString{"Filename"}.leftJustified(len).append("Bytes") << Qt::endl;
    for (int i{0}; i < finfo_list.size(); ++i) {
        QFileInfo finfo{finfo_list.at(i)};
        QString str{finfo.fileName().leftJustified(len)};
        str = str.append(QString{"%1"}).arg(finfo.size());
        out << str << Qt::endl;
    }
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
}
}
