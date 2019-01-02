#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>
#include <memory>

/**
 * The Builder design pattern handles construction of many possible representations
 * based on given input. It constructs complex objects step by step.
 * It often builds a Composite.
 */

class HtmlBuilder;

class HtmlElement {
public:
    friend class HtmlBuilder;
    HtmlElement() = default;
    template<typename StrT1, typename StrT2,
             typename = std::enable_if_t<(std::is_convertible_v<StrT1,std::string>) &&
                              std::is_convertible_v<StrT2,std::string>>>
    HtmlElement(StrT1&& name, StrT2&& text)
        : name_{std::forward<StrT1>(name)}, text_{std::forward<StrT2>(text)}
        {
        }

    std::string str(int indent = 0) const
    {
        std::ostringstream oss;
        std::string i(indent_size_ * indent, ' ');
        oss << i << "<" << name_ << ">\n";
        if (text_.size() > 0) {
            oss << std::string(indent_size_ * (indent+1), ' ') << text_ << "\n";
        }
        for (const auto& e : elements_) {
            oss << e.str(indent + 1);
        }
        oss << i << "</" << name_ << ">\n";
        return oss.str();
    }

    static std::unique_ptr<HtmlBuilder> build(std::string root_name)
    {
        return std::make_unique<HtmlBuilder>(std::move(root_name));
    }

private:
    std::string name_{};
    std::string text_{};
    std::vector<HtmlElement> elements_{};
    const int indent_size_{2};
};

class HtmlBuilder {
public:
    HtmlBuilder(std::string root_name)
    {
        root_.name_ = std::move(root_name);
    }

    HtmlBuilder& add_child(std::string child_name, std::string child_text)
    {
        root_.elements_.emplace_back(std::move(child_name), std::move(child_text));
        return *this;
    }

    HtmlBuilder* add_child_p(std::string child_name, std::string child_text)
    {
        root_.elements_.emplace_back(std::move(child_name), std::move(child_text));
        return this;
    }

    std::string str() const { return root_.str(); }

    operator HtmlElement() const { return root_; }

private:
    HtmlElement root_{};
};

int main()
{
// non-builder based examples
    // <p>Hello</p>
    auto text = "Hello";
    std::string output;
    output += "<p>";
    output += text;
    output += "</p>";
    std::cout << output << "\n";

    // <ul><li>Hello</li><li>world</li></ul>
    std::string words[] = {"Hello", "world"};
    std::ostringstream oss;
    oss << "<ul>";
    for (const auto& w : words) {
        oss << " <li>" << w << "</li>";
    }
    oss << "</ul>";
    std::cout << oss.str() << "\n";

// builder-based example - easier and cleaner
    HtmlBuilder builder{"ul"};
    builder.add_child("li", "Hello").add_child("li","world");
    std::cout << builder.str() << "\n";

    auto builder2 = HtmlElement::build("ul");
    builder2->add_child_p("li", "Hello")
            ->add_child_p("li", "world");
    std::cout << builder2->str() << "\n";
}
