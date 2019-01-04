#include <iostream>
#include <string>
#include <utility>


class Photo {
public:
    Photo(std::string s)
        : title_{std::move(s)}
    {
        std::cerr << "Processing " << title_ << "...\n";
    }

private:
    std::string title_{};
};


class PhotoProcessor {
public:
    PhotoProcessor() noexcept = default;
    PhotoProcessor(PhotoProcessor const&) = default;
    PhotoProcessor(PhotoProcessor&&) noexcept = default;
    PhotoProcessor& operator=(PhotoProcessor const&) = default;
    PhotoProcessor& operator=(PhotoProcessor&&) noexcept = default;
    virtual ~PhotoProcessor() noexcept = default;

    void process(Photo& p)
    {
        process_impl(p);
        if (next_processor_) {
            next_processor_->process(p);
        }
    }

    void next_processor(PhotoProcessor* p) { next_processor_ = p; }

protected:
    virtual void process_impl(Photo& p) = 0;

private:
    PhotoProcessor* next_processor_{nullptr};
};


class Scale : public PhotoProcessor {
public:
    enum SCALE { S50, S100, S200, S300, S500 };
    Scale(SCALE s)
        : scale_{s}
    {
    }

private:
    void process_impl(Photo& p) override { std::cerr << "Scaling photo\n"; }

    SCALE scale_{};
};

class RedEye : public PhotoProcessor {
private:
    void process_impl(Photo& p) override { std::cerr << "Removing red eye effect\n"; }
};

class Filter : public PhotoProcessor {
private:
    void process_impl(Photo& p) override { std::cerr << "Applying filters\n"; }
};

class ColorMatch : public PhotoProcessor {
private:
    void process_impl(Photo& p) override { std::cerr << "Matching colors\n"; }
};

void process_photo(Photo& p)
{
    ColorMatch color;
    RedEye eye;
    Filter filter;
    Scale scale(Scale::S200);
    scale.next_processor(&eye);
    eye.next_processor(&color);
    color.next_processor(&filter);
    scale.process(p);
}

int main()
{
    Photo p("Y2013 Photo");
    process_photo(p);
}
