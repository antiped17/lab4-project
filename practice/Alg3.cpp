#include <iostream>
#include <array>

using namespace std;

const int SIZE = 10;
using Array = array<int, SIZE>;

Array bubbleSort(Array arr) {
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int t = arr[j + 1];
            arr[j + 1] = arr[j];
            arr[j] = t;
            }
        }
    }
    return arr;
}

Array selectionSort(Array arr) {
    for (int i = 0; i < SIZE; i++) {
        int minInd = i;
        for (int j = i + 1; j < SIZE; j++) {
            if (arr[j] < arr[minInd]) {
                minInd = j;
            }
        }
        int t = arr[minInd];
        arr[minInd] = arr[i];
        arr[i] = t;
    }
    return arr;
}
Array insertionSort(Array arr) {
    for (int i = 0; i < SIZE; i++) {
        int n = arr[i];
        int j = i - 1;
        while (j >= 0 && n < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = n;
    }
    return arr;
}
void displayArr(Array arr) {
    cout << "[";
    for (int i = 0; i < SIZE; i++) cout << arr[i] << " ";
    cout << "]";
}
Array rockSort(Array arr) {
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                int t = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = t;
            }
        }
    }
    return arr;
}

int main()
{
    Array arr = { 12, 35, 45, 74, 21, 42, 69, 88, 11, 8 };
    cout << "Bubble: ";
    Array bubble = bubbleSort(arr);
    displayArr(bubble);
    cout << "\n\n";
    cout << "Selection: ";
    Array selection = selectionSort(arr);
    displayArr(selection);
    cout << "\n\n";
    cout << "Insertion: ";
    Array insertion = insertionSort(arr);
    displayArr(insertion);
    Array rock = rockSort(arr);
    cout << "\n\n";
    cout << "Rock: ";
    displayArr(rock);
    return 0;
}

