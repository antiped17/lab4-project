// Шаблонные функции сортировки для лабораторной работы
// «Анализ сложности алгоритмов».
//
//   insertion_sort — сортировка вставками, O(n^2) в среднем и худшем случае,
//                    O(n) на уже упорядоченных данных.
//   merge_sort     — сортировка слиянием (рекурсивная, сверху вниз),
//                    O(n log n) в любом случае, дополнительная память O(n).
//
// Обе функции принимают пару итераторов произвольного доступа и необязательный
// компаратор — так же, как std::sort, поэтому работают с std::vector, обычными
// массивами, std::deque и любыми типами элементов, для которых задан порядок.
#pragma once

#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace sorts {

// ---------------------------------------------------------------------------
// Сортировка вставками — O(n^2)
// ---------------------------------------------------------------------------
// Массив делится на отсортированную часть [first, i) и неотсортированную
// [i, last). На каждом шаге очередной элемент *i сдвигается влево, пока
// предыдущий элемент больше него.
template <typename RandomIt, typename Compare = std::less<>>
void insertion_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    if (last - first < 2) return;

    for (RandomIt i = first + 1; i != last; ++i) {
        auto key = std::move(*i);
        RandomIt j = i;
        while (j != first && comp(key, *(j - 1))) {
            *j = std::move(*(j - 1));
            --j;
        }
        *j = std::move(key);
    }
}

// ---------------------------------------------------------------------------
// Сортировка слиянием — O(n log n)
// ---------------------------------------------------------------------------
namespace detail {

// Слияние двух соседних отсортированных половин [first, mid) и [mid, last)
// через буфер buf. Слияние устойчиво: при равенстве берётся элемент слева.
template <typename RandomIt, typename T, typename Compare>
void merge(RandomIt first, RandomIt mid, RandomIt last,
           std::vector<T>& buf, Compare& comp)
{
    buf.clear();
    RandomIt left = first;
    RandomIt right = mid;

    while (left != mid && right != last) {
        if (comp(*right, *left))
            buf.push_back(std::move(*right++));
        else
            buf.push_back(std::move(*left++));
    }
    while (left != mid) buf.push_back(std::move(*left++));
    while (right != last) buf.push_back(std::move(*right++));

    std::move(buf.begin(), buf.end(), first);
}

template <typename RandomIt, typename T, typename Compare>
void merge_sort_impl(RandomIt first, RandomIt last,
                     std::vector<T>& buf, Compare& comp)
{
    const auto n = last - first;
    if (n < 2) return;

    RandomIt mid = first + n / 2;
    merge_sort_impl(first, mid, buf, comp);  // сортируем левую половину
    merge_sort_impl(mid, last, buf, comp);   // сортируем правую половину

    // Половины уже идут в правильном порядке — сливать не нужно.
    if (!comp(*mid, *(mid - 1))) return;

    merge(first, mid, last, buf, comp);
}

} // namespace detail

template <typename RandomIt, typename Compare = std::less<>>
void merge_sort(RandomIt first, RandomIt last, Compare comp = Compare{})
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    // Буфер выделяется один раз на всю сортировку, а не на каждое слияние.
    std::vector<T> buf;
    buf.reserve(static_cast<std::size_t>(last - first));
    detail::merge_sort_impl(first, last, buf, comp);
}

} // namespace sorts
