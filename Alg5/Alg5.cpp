#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

const int TESTS = 10;          // сколько разных массивов на каждый размер n
const int MAX_N2 = 50000;      // insertionSort (O(n^2)) запускаем только до этого n
const int PARTS = 10;          // на сколько упорядоченных фрагментов делим массив

// ---------------------------------------------------------------------------
// 1. Сортировка вставками, O(n^2)
// ---------------------------------------------------------------------------
template <typename T>
void insertionSort(T arr[], int size) {
    for (int i = 1; i < size; i++) {
        T n = arr[i];
        int j = i - 1;
        while (j >= 0 && n < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = n;
    }
}

// ---------------------------------------------------------------------------
// 2. Сортировка слиянием, O(n log n)
// ---------------------------------------------------------------------------
template <typename T>
void mergeSortRange(T arr[], T buf[], int left, int right) {
    if (right - left <= 1) return;

    int center = (right - left) / 2 + left;

    mergeSortRange(arr, buf, left, center);
    mergeSortRange(arr, buf, center, right);

    int i = left;
    int j = center;

    for (int k = left; k < right; k++) {
        if (j == right || (i < center && arr[i] <= arr[j])) {
            buf[k] = arr[i++];
        }
        else buf[k] = arr[j++];
    }

    for (int k = left; k < right; k++) {
        arr[k] = buf[k];
    }
}

template <typename T>
void mergeSort(T arr[], int size) {
    T* buf = new T[size];
    mergeSortRange(arr, buf, 0, size);
    delete[] buf;
}

// Стандартная сортировка, обёрнута в такую же функцию, чтобы мерить одинаково.
template <typename T>
void stdSort(T arr[], int size) {
    sort(arr, arr + size);
}

// ---------------------------------------------------------------------------
// Заполнение массивов
// ---------------------------------------------------------------------------

// п. 4: случайные числа
void fillRandom(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand();
    }
}

// п. 5: массив из PARTS упорядоченных фрагментов.
// Сначала заполняем случайно, потом сортируем каждый кусок отдельно.
void fillFragments(int arr[], int size) {
    fillRandom(arr, size);
    int len = size / PARTS;
    for (int p = 0; p < PARTS; p++) {
        int left = p * len;
        int right = (p == PARTS - 1) ? size : left + len;
        sort(arr + left, arr + right);
    }
}

// ---------------------------------------------------------------------------
// Вспомогательные функции
// ---------------------------------------------------------------------------
template <typename T>
bool isSorted(T arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) return false;
    }
    return true;
}

template <typename T>
void printArray(T arr[], int size) {
    cout << "[ ";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << " ]" << endl;
}

// Время одной сортировки в миллисекундах.
// src не портится: сортируем его копию arr, поэтому все алгоритмы
// получают одинаковые данные. Копирование в замер не входит.
double measure(void (*sortFunc)(int[], int), int src[], int arr[], int size) {
    for (int i = 0; i < size; i++) arr[i] = src[i];

    auto start = chrono::steady_clock::now();
    sortFunc(arr, size);
    auto end = chrono::steady_clock::now();

    if (!isSorted(arr, size)) {
        cout << "ERROR: array is not sorted!" << endl;
        exit(1);
    }
    return chrono::duration<double, milli>(end - start).count();
}

// Среднее время трёх алгоритмов на TESTS массивах размера size.
// Результат: печать строки таблицы и запись строки в CSV.
void runTests(const char* name, void (*fill)(int[], int), int size,
              int src[], int arr[], ofstream& csv) {
    double tIns = 0, tMerge = 0, tStd = 0;
    bool doIns = size <= MAX_N2;

    for (int t = 0; t < TESTS; t++) {
        fill(src, size);
        if (doIns) tIns += measure(insertionSort<int>, src, arr, size);
        tMerge += measure(mergeSort<int>, src, arr, size);
        tStd += measure(stdSort<int>, src, arr, size);
    }
    tIns /= TESTS;
    tMerge /= TESTS;
    tStd /= TESTS;

    cout << setw(10) << size;
    if (doIns) cout << setw(16) << tIns;
    else cout << setw(16) << "-";
    cout << setw(16) << tMerge << setw(16) << tStd << endl;

    csv << name << ";" << size << ";";
    if (doIns) csv << tIns;
    csv << ";" << tMerge << ";" << tStd << endl;
}

int main() {
    srand((unsigned)time(0));

    // Проверка, что шаблоны работают с разными типами
    int a[] = { 2, 5, 7, 8, 1, 4, 3, 9, 6, 0 };
    double b[] = { 3.5, -1.2, 8.0, 0.5, 2.25 };
    insertionSort(a, 10);
    mergeSort(b, 5);
    cout << "insertionSort<int>: ";
    printArray(a, 10);
    cout << "mergeSort<double>:  ";
    printArray(b, 5);
    cout << endl;

    int sizes[] = { 1000, 2000, 5000, 10000, 20000, 30000, 40000, 50000,
                    100000, 200000, 500000, 1000000 };
    const int sizeCount = sizeof(sizes) / sizeof(sizes[0]);
    const int maxSize = 1000000;

    int* src = new int[maxSize];
    int* arr = new int[maxSize];

    ofstream csv("results.csv");
    csv << "data;n;insertionSort;mergeSort;std::sort" << endl;
    cout << fixed << setprecision(3);
    csv << fixed << setprecision(3);

    cout << "Average time, ms (" << TESTS << " arrays for each n)" << endl;

    cout << endl << "Random arrays:" << endl;
    cout << setw(10) << "n" << setw(16) << "insertionSort" << setw(16) << "mergeSort" << setw(16) << "std::sort" << endl;
    for (int i = 0; i < sizeCount; i++) {
        runTests("random", fillRandom, sizes[i], src, arr, csv);
    }

    cout << endl << "Arrays with " << PARTS << " sorted fragments:" << endl;
    cout << setw(10) << "n" << setw(16) << "insertionSort" << setw(16) << "mergeSort" << setw(16) << "std::sort" << endl;
    for (int i = 0; i < sizeCount; i++) {
        runTests("fragments", fillFragments, sizes[i], src, arr, csv);
    }

    cout << endl << "Saved to results.csv" << endl;

    delete[] src;
    delete[] arr;
    return 0;
}
