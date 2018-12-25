#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <utility>


class FormattedText {
public:
    explicit FormattedText(std::string const& plain_text)
        : plain_text_{plain_text}
        , caps_{std::make_unique<bool[]>(plain_text.length())}
    {
        std::fill_n(&caps_[0], plain_text.length(), false);
    }

    void capitalize(int start, int end)
    {
        std::fill(&caps_[0]+start, &caps_[0]+end, true);
    }

    friend std::ostream& operator<<(std::ostream& os, FormattedText const& obj)
    {
        std::ostringstream oss{};
        for(int i = 0; i < obj.plain_text_.length(); ++i)
        {
            const char c = obj.plain_text_[i];
            oss << (obj.caps_[i] ? static_cast<char const>(std::toupper(c)) : c);
        }
        return os << oss.str();
    }

private:
    std::string plain_text_;
    std::unique_ptr<bool[]> caps_;
};


class BetterFormattedText {
public:
    struct TextRange {
        int start, end;
        bool capitalize, bold, italic;
        bool covers(int position) const { return position >= start && position <= end; }
    };

    TextRange& get_range(int start, int end)
    {
        formatting_.push_back(TextRange{start, end});
        return formatting_.back();
    }

    explicit BetterFormattedText(std::string const& plain_text)
        : plain_text_{plain_text}
    {
    }

    friend std::ostream& operator<<(std::ostream& os, BetterFormattedText const& obj)
    {
        std::ostringstream oss{};
        for (size_t i = 0; i < obj.plain_text_.length(); ++i)
        {
            auto c = obj.plain_text_[i];
            for (auto const& rng : obj.formatting_)
            {
                if (rng.covers(i) && rng.capitalize) {
                    c = toupper(c);
                }
            }
            os << c;
        }
        return os << oss.str();
    }

private:
    std::string plain_text_;
    std::vector<TextRange> formatting_{};
};

int main()
{
    FormattedText ft("This is a brave new world");
    ft.capitalize(10, 15);
    std::cerr << ft << "\n";

    BetterFormattedText bft("This is a brave new world!");
    bft.get_range(10, 15).capitalize = true;
    std::cerr << bft << "\n";
}
