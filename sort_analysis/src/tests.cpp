// Проверка корректности шаблонных сортировок на разных типах и данных.
#include "sorts.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

int failures = 0;

void check(bool ok, const std::string& name)
{
    std::cout << (ok ? "[ OK ] " : "[FAIL] ") << name << '\n';
    if (!ok) ++failures;
}

// Сравнивает результат нашей сортировки с std::stable_sort на тех же данных.
template <typename Container, typename Sorter, typename Compare = std::less<>>
bool same_as_std(Container data, Sorter sorter, Compare comp = Compare{})
{
    Container expected = data;
    std::stable_sort(expected.begin(), expected.end(), comp);
    sorter(data.begin(), data.end(), comp);
    return data == expected;
}

struct Person {
    std::string name;
    int age;
    bool operator==(const Person& o) const { return name == o.name && age == o.age; }
};

template <typename Sorter>
void run_suite(const std::string& algo, Sorter sorter)
{
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(-1000, 1000);

    check(same_as_std(std::vector<int>{}, sorter), algo + ": пустой массив");
    check(same_as_std(std::vector<int>{42}, sorter), algo + ": один элемент");
    check(same_as_std(std::vector<int>{2, 1}, sorter), algo + ": два элемента");
    check(same_as_std(std::vector<int>{5, 5, 5, 5}, sorter), algo + ": все равны");
    check(same_as_std(std::vector<int>{1, 2, 3, 4, 5, 6}, sorter), algo + ": уже отсортирован");
    check(same_as_std(std::vector<int>{6, 5, 4, 3, 2, 1}, sorter), algo + ": обратный порядок");

    for (int n : {3, 10, 100, 1000, 5000}) {
        std::vector<int> v(n);
        for (auto& x : v) x = dist(rng);
        check(same_as_std(v, sorter), algo + ": случайный int, n = " + std::to_string(n));
    }

    std::vector<double> d(500);
    for (auto& x : d) x = std::uniform_real_distribution<double>(-1, 1)(rng);
    check(same_as_std(d, sorter), algo + ": double");

    check(same_as_std(std::vector<std::string>{"pear", "apple", "fig", "kiwi", "banana"}, sorter),
          algo + ": std::string");

    std::deque<int> dq{9, 3, 7, 1, 8, 2};
    check(same_as_std(dq, sorter), algo + ": std::deque");

    // Обычный C-массив.
    int arr[] = {4, -2, 9, 0, 3, 3, -7};
    int ref[] = {4, -2, 9, 0, 3, 3, -7};
    sorter(std::begin(arr), std::end(arr), std::less<>{});
    std::sort(std::begin(ref), std::end(ref));
    check(std::equal(std::begin(arr), std::end(arr), std::begin(ref)), algo + ": C-массив");

    check(same_as_std(std::vector<int>{1, 5, 2, 8, 3}, sorter, std::greater<>{}),
          algo + ": по убыванию (std::greater)");

    // Устойчивость: люди с одинаковым возрастом сохраняют исходный порядок.
    std::vector<Person> people{{"Аня", 20}, {"Борис", 19}, {"Вика", 20},
                               {"Гоша", 19}, {"Даша", 21}, {"Егор", 20}};
    auto by_age = [](const Person& a, const Person& b) { return a.age < b.age; };
    check(same_as_std(people, sorter, by_age), algo + ": устойчивость (свой компаратор)");
}

} // namespace

int main()
{
    run_suite("insertion_sort", [](auto f, auto l, auto c) { sorts::insertion_sort(f, l, c); });
    run_suite("merge_sort", [](auto f, auto l, auto c) { sorts::merge_sort(f, l, c); });

    std::cout << (failures == 0 ? "\nВсе тесты пройдены.\n" : "\nЕсть ошибки!\n");
    return failures == 0 ? 0 : 1;
}
