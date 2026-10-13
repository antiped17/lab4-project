// Измерение среднего времени работы сортировок:
//   insertion_sort (O(n^2)), merge_sort (O(n log n)) и std::sort.
//
// Типы тестовых массивов:
//   random        — элементы заполняются случайно (п. 4 задания);
//   fragments     — массив состоит из нескольких упорядоченных фрагментов
//                   случайной длины (п. 5 задания);
//   nearly_sorted — отсортированный массив, в котором переставлено 5 %
//                   случайных пар элементов (дополнительный опыт к п. 5).
//
// Для каждого n генерируется `trials` разных массивов; каждый алгоритм
// сортирует копию одного и того же массива, поэтому сравнение честное.
// Время копирования и проверки результата в замер не входит.
//
// Результаты печатаются на экран и сохраняются в CSV:
//   data,algorithm,n,trials,avg_ms,min_ms,max_ms
//
// Запуск:  benchmark [каталог_для_результатов] [--quick]
#include "sorts.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

using Array = std::vector<int>;
using Clock = std::chrono::steady_clock;

// ------------------------------- генераторы ---------------------------------

Array make_random(std::size_t n, std::mt19937& rng)
{
    std::uniform_int_distribution<int> dist(0, 1'000'000'000);
    Array a(n);
    for (auto& x : a) x = dist(rng);
    return a;
}

// Случайный массив, разбитый на `parts` фрагментов случайной длины;
// каждый фрагмент отсортирован по возрастанию.
Array make_fragments(std::size_t n, std::mt19937& rng, std::size_t parts = 10)
{
    Array a = make_random(n, rng);
    if (n < 2) return a;

    std::vector<std::size_t> cuts{0, n};
    std::uniform_int_distribution<std::size_t> pos(1, n - 1);
    for (std::size_t i = 1; i < parts; ++i) cuts.push_back(pos(rng));
    std::sort(cuts.begin(), cuts.end());

    for (std::size_t i = 0; i + 1 < cuts.size(); ++i)
        std::sort(a.begin() + cuts[i], a.begin() + cuts[i + 1]);
    return a;
}

// Отсортированный массив, в котором переставлено `percent` % пар элементов.
Array make_nearly_sorted(std::size_t n, std::mt19937& rng, double percent = 5.0)
{
    Array a = make_random(n, rng);
    std::sort(a.begin(), a.end());
    if (n < 2) return a;

    std::uniform_int_distribution<std::size_t> pos(0, n - 1);
    const auto swaps = static_cast<std::size_t>(n * percent / 100.0);
    for (std::size_t i = 0; i < swaps; ++i) std::swap(a[pos(rng)], a[pos(rng)]);
    return a;
}

struct DataKind {
    std::string name;
    std::function<Array(std::size_t, std::mt19937&)> make;
};

struct Algorithm {
    std::string name;
    std::function<void(Array&)> sort;
    std::size_t max_n;  // O(n^2) не запускаем на очень больших массивах
};

// ------------------------------- замер --------------------------------------

double time_ms(const Algorithm& alg, const Array& src)
{
    Array a = src;  // копия, чтобы каждый алгоритм получал одинаковые данные
    const auto start = Clock::now();
    alg.sort(a);
    const auto stop = Clock::now();

    if (!std::is_sorted(a.begin(), a.end())) {
        std::cerr << "ОШИБКА: " << alg.name << " вернул неотсортированный массив\n";
        std::exit(1);
    }
    return std::chrono::duration<double, std::milli>(stop - start).count();
}

} // namespace

int main(int argc, char* argv[])
{
    std::string out_dir = "results";
    bool quick = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--quick") quick = true;
        else out_dir = arg;
    }

    std::vector<std::size_t> sizes;
    std::size_t quadratic_limit;
    int trials;
    if (quick) {
        sizes = {1000, 2000, 5000, 10000, 20000, 50000, 100000};
        quadratic_limit = 20000;
        trials = 3;
    } else {
        sizes = {1000, 2000, 5000, 10000, 15000, 20000, 30000, 40000, 50000,
                 100000, 200000, 300000, 500000, 750000, 1000000};
        quadratic_limit = 50000;
        trials = 10;
    }

    const std::vector<DataKind> kinds{
        {"random", make_random},
        {"fragments", [](std::size_t n, std::mt19937& r) { return make_fragments(n, r); }},
        {"nearly_sorted", [](std::size_t n, std::mt19937& r) { return make_nearly_sorted(n, r); }},
    };

    const std::vector<Algorithm> algorithms{
        {"insertion_sort", [](Array& a) { sorts::insertion_sort(a.begin(), a.end()); }, quadratic_limit},
        {"merge_sort", [](Array& a) { sorts::merge_sort(a.begin(), a.end()); }, SIZE_MAX},
        {"std::sort", [](Array& a) { std::sort(a.begin(), a.end()); }, SIZE_MAX},
    };

    const std::string csv_path = out_dir + "/results.csv";
    std::ofstream csv(csv_path);
    if (!csv) {
        std::cerr << "Не удалось открыть " << csv_path
                  << " (существует ли каталог " << out_dir << "?)\n";
        return 1;
    }
    csv << "data,algorithm,n,trials,avg_ms,min_ms,max_ms\n";
    csv << std::fixed << std::setprecision(4);
    std::cout << std::fixed << std::setprecision(3);

    for (const auto& kind : kinds) {
        std::cout << "\n=== Данные: " << kind.name << " (" << trials
                  << " массивов на каждое n), время в мс ===\n";
        std::cout << std::setw(10) << "n";
        for (const auto& alg : algorithms) std::cout << std::setw(18) << alg.name;
        std::cout << '\n';

        for (std::size_t n : sizes) {
            std::vector<double> sum(algorithms.size(), 0.0);
            std::vector<double> mn(algorithms.size(), 1e300);
            std::vector<double> mx(algorithms.size(), 0.0);

            // Фиксированное зерно -> результаты воспроизводимы.
            std::mt19937 rng(static_cast<std::uint32_t>(n * 7919 + kind.name.size()));
            for (int t = 0; t < trials; ++t) {
                const Array src = kind.make(n, rng);
                for (std::size_t k = 0; k < algorithms.size(); ++k) {
                    if (n > algorithms[k].max_n) continue;
                    const double ms = time_ms(algorithms[k], src);
                    sum[k] += ms;
                    mn[k] = std::min(mn[k], ms);
                    mx[k] = std::max(mx[k], ms);
                }
            }

            std::cout << std::setw(10) << n;
            for (std::size_t k = 0; k < algorithms.size(); ++k) {
                if (n > algorithms[k].max_n) {
                    std::cout << std::setw(18) << "-";
                    continue;
                }
                const double avg = sum[k] / trials;
                std::cout << std::setw(18) << avg;
                csv << kind.name << ',' << algorithms[k].name << ',' << n << ',' << trials
                    << ',' << avg << ',' << mn[k] << ',' << mx[k] << '\n';
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\nРезультаты сохранены в " << csv_path << '\n';
    return 0;
}
