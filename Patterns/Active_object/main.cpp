#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

#include "backgrounder.h"

namespace {

void printPercentageLeft(unsigned num, unsigned& progress, unsigned max)
{
    const float percent{100 * (static_cast<float>(num) / max)};
    const unsigned int rounded{(static_cast<int>(percent) / 10) * 10};
    if (rounded != progress) {
        std::cout << 100 - progress << " ";
        progress = rounded;
        return;
    }
}

template <typename T>
void printProgress(std::vector<T> const& out, std::vector<T> const& in, const unsigned max)
{
    std::cout << "\nLeft to Process [%]: ";
    unsigned progress{100};
    do {
        const unsigned remaining{in.size() - out.size()};
        printPercentageLeft(remaining, progress, max);
    } while ((out.size() < max) && progress <= 100);
}

void runStrWorkers(int num_items)
{
    std::vector<std::string> save_to_q;
    std::vector<std::string> compare_q;

    const auto start{std::chrono::steady_clock::now()};
    {
        Backgrounder<std::string> worker(save_to_q);
        std::random_device rd;
        std::default_random_engine re{rd()};
        std::uniform_int_distribution<> ud;

        for (auto i{0}; i < num_items; ++i) {
            auto const random{ud(re)};
            std::ostringstream oss;
            oss << random;
            compare_q.push_back(oss.str());
            worker.save_data(std::move(oss.str()));
        }
        auto const push_time{std::chrono::steady_clock::now() - start};
        std::cout << "Finished pushing #" << num_items << " jobs to bg worker"
                  << " in " << std::chrono::duration_cast<std::chrono::seconds>(push_time).count()
                  << " [s]" << std::endl;

        printProgress(save_to_q, compare_q, num_items);
    }  // end of backgrounder scope
    auto const work_time{std::chrono::steady_clock::now() - start};
    std::cout << "\nBackgrounder finished with processing jobs in "
              << std::chrono::duration_cast<std::chrono::seconds>(work_time).count() << " [s]"
              << std::endl;

    assert(std::equal(compare_q.cbegin(), compare_q.cend(), save_to_q.cbegin()));
    assert(save_to_q.size() == num_items);
}

void runIntWorkers(int num_items)
{
    std::vector<int> save_to_q;
    std::vector<int> compare_q;

    const auto start{std::chrono::steady_clock::now()};
    {
        Backgrounder<int> worker(save_to_q);
        std::random_device rd;
        std::default_random_engine re{rd()};
        std::uniform_int_distribution<> ud;

        for (auto i{0}; i < num_items; ++i) {
            auto const random{ud(re)};
            compare_q.push_back(random);
            worker.save_data(random);
        }

        // extra case for empty item
        compare_q.push_back(0);
        worker.save_data(0);

        auto const push_time{std::chrono::steady_clock::now() - start};
        std::cout << "Finished pushing #" << num_items << " jobs to bg worker"
                  << " in " << std::chrono::duration_cast<std::chrono::seconds>(push_time).count()
                  << " [s]" << std::endl;

        printProgress(save_to_q, compare_q, num_items);
    }  // end of backgrounder scope
    auto const work_time{std::chrono::steady_clock::now() - start};
    std::cout << "\nBackgrounder finished with processing jobs in "
              << std::chrono::duration_cast<std::chrono::seconds>(work_time).count() << " [s]"
              << std::endl;

    assert(std::equal(compare_q.cbegin(), compare_q.cend(), save_to_q.cbegin()));
    assert(save_to_q.size() == num_items + 1);  // account for the "extra case for empty item"
}

void runStrWorkers_Backgrounder2(int num_items)
{
    std::vector<std::string> save_to_q;
    std::vector<std::string> compare_q;

    const auto start{std::chrono::steady_clock::now()};
    {
        auto worker{jam::Active2::create<Backgrounder2<std::string>>(save_to_q)};
        std::random_device rd;
        std::default_random_engine re{rd()};
        std::uniform_int_distribution<> ud;

        for (auto i{0}; i < num_items; ++i) {
            auto const random{ud(re)};
            std::ostringstream oss;
            oss << random;
            compare_q.push_back(oss.str());
            worker.save_data(std::move(oss.str()));
        }
        auto const push_time{std::chrono::steady_clock::now() - start};
        std::cout << "Finished pushing #" << num_items << " jobs to bg worker"
                  << " in " << std::chrono::duration_cast<std::chrono::seconds>(push_time).count()
                  << " [s]" << std::endl;

        printProgress(save_to_q, compare_q, num_items);
    }  // end of backgrounder scope
    auto const work_time{std::chrono::steady_clock::now() - start};
    std::cout << "\nBackgrounder finished with processing jobs in "
              << std::chrono::duration_cast<std::chrono::seconds>(work_time).count() << " [s]"
              << std::endl;

    assert(std::equal(compare_q.cbegin(), compare_q.cend(), save_to_q.cbegin()));
    assert(save_to_q.size() == num_items);
}

void runIntWorkers_Backgrounder2(int num_items)
{
    std::vector<int> save_to_q;
    std::vector<int> compare_q;

    const auto start{std::chrono::steady_clock::now()};
    {
        // Backgrounder<int> worker(save_to_q);
        auto worker{jam::Active2::create<Backgrounder2<int>>(save_to_q)};
        std::random_device rd;
        std::default_random_engine re{rd()};
        std::uniform_int_distribution<> ud;

        for (auto i{0}; i < num_items; ++i) {
            auto const random{ud(re)};
            compare_q.push_back(random);
            worker.save_data(random);
        }

        // extra case for empty item
        compare_q.push_back(0);
        worker.save_data(0);

        auto const push_time{std::chrono::steady_clock::now() - start};
        std::cout << "Finished pushing #" << num_items << " jobs to bg worker"
                  << " in " << std::chrono::duration_cast<std::chrono::seconds>(push_time).count()
                  << " [s]" << std::endl;

        printProgress(save_to_q, compare_q, num_items);
    }  // end of backgrounder scope
    auto const work_time{std::chrono::steady_clock::now() - start};
    std::cout << "\nBackgrounder finished with processing jobs in "
              << std::chrono::duration_cast<std::chrono::seconds>(work_time).count() << " [s]"
              << std::endl;

    assert(std::equal(compare_q.cbegin(), compare_q.cend(), save_to_q.cbegin()));
    assert(save_to_q.size() == num_items + 1);  // account for the "axtra case for empty item"
}


void runStrWorkers_Backgrounder3(int num_items)
{
    std::vector<std::string> save_to_q;
    std::vector<std::string> compare_q;

    const auto start{std::chrono::steady_clock::now()};
    {
        auto worker{Backgrounder3<std::string>::create(save_to_q)};
        std::random_device rd;
        std::default_random_engine re{rd()};
        std::uniform_int_distribution<> ud;

        for (auto i{0}; i < num_items; ++i) {
            auto const random{ud(re)};
            std::ostringstream oss;
            oss << random;
            compare_q.push_back(oss.str());
            worker.save_data(std::move(oss.str()));
        }
        auto const push_time{std::chrono::steady_clock::now() - start};
        std::cout << "Finished pushing #" << num_items << " jobs to bg worker"
                  << " in " << std::chrono::duration_cast<std::chrono::seconds>(push_time).count()
                  << " [s]" << std::endl;

        printProgress(save_to_q, compare_q, num_items);
    }  // end of backgrounder scope
    auto const work_time{std::chrono::steady_clock::now() - start};
    std::cout << "\nBackgrounder finished with processing jobs in "
              << std::chrono::duration_cast<std::chrono::seconds>(work_time).count() << " [s]"
              << std::endl;

    assert(std::equal(compare_q.cbegin(), compare_q.cend(), save_to_q.cbegin()));
    assert(save_to_q.size() == num_items);
}

void runIntWorkers_Backgrounder3(int num_items)
{
    std::vector<int> save_to_q;
    std::vector<int> compare_q;

    const auto start{std::chrono::steady_clock::now()};
    {
        auto worker{Backgrounder3<int>::create(save_to_q)};
        std::random_device rd;
        std::default_random_engine re{rd()};
        std::uniform_int_distribution<> ud;

        for (auto i{0}; i < num_items; ++i) {
            auto const random{ud(re)};
            compare_q.push_back(random);
            worker.save_data(random);
        }

        // extra case for empty item
        compare_q.push_back(0);
        worker.save_data(0);

        auto const push_time{std::chrono::steady_clock::now() - start};
        std::cout << "Finished pushing #" << num_items << " jobs to bg worker"
                  << " in " << std::chrono::duration_cast<std::chrono::seconds>(push_time).count()
                  << " [s]" << std::endl;

        printProgress(save_to_q, compare_q, num_items);
    }  // end of backgrounder scope
    auto const work_time{std::chrono::steady_clock::now() - start};
    std::cout << "\nBackgrounder finished with processing jobs in "
              << std::chrono::duration_cast<std::chrono::seconds>(work_time).count() << " [s]"
              << std::endl;

    assert(std::equal(compare_q.cbegin(), compare_q.cend(), save_to_q.cbegin()));
    assert(save_to_q.size() == num_items + 1);  // account for the "axtra case for empty item"
}
}  // namespace

int main()
{
    constexpr int num_items{10000};

    std::cout << "First iteration 'Active' object (composition):\n";
    std::cout << num_items << " transactions of std::string/int" << std::endl;
    runStrWorkers(num_items);

    std::cout << "\n\n" << num_items << " transactions of std::string/int" << std::endl;
    runIntWorkers(num_items);

    std::cout << "\n\nSecond iteration 'Active' object (inheritance):\n";
    std::cout << num_items << " transactions of std::string/int" << std::endl;
    runStrWorkers_Backgrounder2(num_items);

    std::cout << "\n\n" << num_items << " transactions of std::string/int" << std::endl;
    runIntWorkers_Backgrounder2(num_items);

    std::cout << "\n\nSecond iteration 'Active' object (CRTP):\n";
    std::cout << num_items << " transactions of std::string/int" << std::endl;
    runStrWorkers_Backgrounder3(num_items);

    std::cout << "\n\n" << num_items << " transactions of std::string/int" << std::endl;
    runIntWorkers_Backgrounder3(num_items);
}
