#include <iostream>
#include <algorithm>
#include <array>

using namespace std;

const int SIZE = 10;
using Array = array<int, SIZE>;

void mergeSortRange(Array &arr, Array& buf, int left, int right) {
    if (right - left <= 1) return;

    int center = (right - left) / 2 + left;

    mergeSortRange(arr, buf, left, center);
    mergeSortRange(arr, buf, center, right);

    int i = left;
    int j = center;

    for (int k = left; k < right; k++) {
        if (j == right || (i < center && arr[i] <= arr[j])) {
            buf[k]=arr[i++];
        }
        else buf[k] = arr[j++];
    }

    for (int k = left; k < right; k++) {
        arr[k] = buf[k];
    }
}

Array mergeSort(Array arr) {
    Array buf;
    mergeSortRange(arr, buf, 0, SIZE);
    return arr;
}

int hoarePart(Array& arr, int left, int right) {
    int pivot = arr[ (right - left) / 2 + left ];
    int i = left - 1;
    int j = right + 1;
    while(true){
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}

void hoareSortRange(Array& arr, int left, int right) {
    if (left >= right) return;
    int center = hoarePart(arr, left, right);
    hoareSortRange(arr, left, center);
    hoareSortRange(arr, center+1, right);
}

Array hoareSort(Array arr) {
    hoareSortRange(arr, 0, SIZE-1);
    return arr;
}

void shiftDown(Array &arr, int root, int n) {
    while (true) {
        int left = 2 * root + 1;
        int right = 2 * root + 2;
        int larg = root;

        if (left < n && arr[left] > arr[larg]) {
            larg = left;
        }
        if (right < n && arr[right] > arr[larg]) {
            larg = right;
        }
        if (root==larg) return;
        swap(arr[root], arr[larg]);
        root = larg;
    }
}

Array heapSort(Array arr) {
    for (int i = SIZE/2 - 1; i >= 0; i--) {
        shiftDown(arr, i, SIZE);
    }

    for (int i = SIZE - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        shiftDown(arr, 0, i);
    }
    return arr;
}

void printArray(Array arr) {
    cout << "[ ";
    for (int i = 0; i < SIZE; i++) {
        cout << arr[i];
        if (i < SIZE - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main()
{
    Array arr = { 2, 5, 7, 8, 1, 4, 3, 9, 6, 0 };
    printArray(arr);
    printArray(mergeSort(arr));
    printArray(hoareSort(arr));
    printArray(heapSort(arr));
    return 0;
}

