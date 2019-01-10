#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <utility>
#include <stdexcept>

/**
 * The `Strategy` design pattern defines an algorithm interface and lets the implementation vary
 * independently by encapsulating it in the derived class. It only adds value to the design
 * if the strategy is chosen or varies at runtime. If it can be chosen statically it's probably
 * better to replace it with a policy based design using templates.
 */

enum class OutputFormat {
    Markdown, Html
};

struct ListStrategy {
    ListStrategy() noexcept = default;
    ListStrategy(ListStrategy const&) = default;
    ListStrategy(ListStrategy&&) = default;
    ListStrategy& operator=(ListStrategy const&) = default;
    ListStrategy& operator=(ListStrategy&&) = default;
    virtual ~ListStrategy() = default;

    virtual void add_list_item(std::ostringstream& oss, std::string const& item) = 0;
    virtual void start(std::ostringstream& oss) = 0;
    virtual void end(std::ostringstream& oss) = 0;
};

struct MarkdownListStrategy : public ListStrategy {
    void start(std::ostringstream&) override { }
    void end(std::ostringstream&) override { }

    void add_list_item(std::ostringstream& oss, std::string const& item) override
    {
        oss << " * " << item << "\n";
    }
};

struct HtmlListStrategy : public ListStrategy {
    void start(std::ostringstream& oss) override
    {
        oss << "<ul>\n";
    }

    void end(std::ostringstream& oss) override
    {
        oss << "</ul>\n";
    }

    void add_list_item(std::ostringstream& oss, std::string const& item) override
    {
        oss << "<li>" << item << "</li>\n";
    }
};

// statically choose a strategy - would probably be better to use a policy
template<typename LS>
class TextProcessor_static {
public:
    void clear()
    {
        oss_.str("");
        oss_.clear();
    }

    void append_list(std::vector<std::string> const& items)
    {
        list_strategy_.start(oss_);
        for (auto const& item : items) {
            list_strategy_.add_list_item(oss_, item);
        }
        list_strategy_.end(oss_);
    }

    std::string str() const { return oss_.str(); }

private:
    std::ostringstream oss_{};
    LS list_strategy_{};
};

// choose the strategy dynamically
class TextProcessor_dynamic {
public:
    void clear()
    {
        oss_.str("");
        oss_.clear();
    }

    void append_list(std::vector<std::string> const& items)
    {
        list_strategy_->start(oss_);
        for (auto const& item : items) {
            list_strategy_->add_list_item(oss_, item);
        }
        list_strategy_->end(oss_);
    }

    void set_output_format(OutputFormat const format)
    {
        switch (format) {
        case OutputFormat::Markdown:
            list_strategy_ = std::make_unique<MarkdownListStrategy>();
            break;
        case OutputFormat::Html:
            list_strategy_ = std::make_unique<HtmlListStrategy>();
            break;
        default:
            throw std::logic_error{"Unsupported strategy"};
        }
    }

    std::string str() const { return oss_.str(); }

private:
    std::ostringstream oss_{};
    std::unique_ptr<ListStrategy> list_strategy_{};
};


int main()
{
    // markdown
    TextProcessor_static<MarkdownListStrategy> tpm;
    tpm.append_list({"foo", "bar", "baz"});
    std::cerr << tpm.str() << "\n";

    // html
    TextProcessor_static<HtmlListStrategy> tph;
    tph.append_list({"foo", "bar", "baz"});
    std::cerr << tph.str() << "\n";

    std::cerr << "\ndynamic TextProcessor:\n";
    TextProcessor_dynamic tp;
    tp.set_output_format(OutputFormat::Markdown);
    tp.append_list({"foo", "bar", "baz"});
    tp.set_output_format(OutputFormat::Html);
    tp.append_list({"foo","bar","baz"});
    std::cerr << tp.str() << "\n";
}
