#include <array>
#include <iostream>
#include <utility>

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
  for (int i = 0; i < SIZE - 1; i++) {
    int minInt = i;
    for (int j = i + 1; j < SIZE; j++) {
      if (arr[j] < arr[minInt]) {
        minInt = j;
      }
    }
    if (minInt != i) {
      int t = arr[minInt];
      arr[minInt] = arr[i];
      arr[i] = t;
    }
  }
  return arr;
}

Array insertionSort(Array arr) {
  for (int i = 1; i < SIZE; i++) {
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

void mergeSortRange(Array& arr, Array& buf, int left, int right) {
  if (right - left <= 1) {
    return;
  }
  int center = (right - left) / 2 + left;
  mergeSortRange(arr, buf, left, center);
  mergeSortRange(arr, buf, center, right);
  int i = left;
  int j = center;
  for (int k = left; k < right; k++) {
    if (j == right || (i < center && arr[i] <= arr[j])) {
      buf[k] = arr[i++];
    }
    else {
      buf[k] = arr[j++];
    }
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
  int pivot = arr[(right - left) / 2 + left];
  int i = left - 1;
  int j = right + 1;
  while (true) {
    do { i++; } while (arr[i] < pivot);
    do { j--; } while (arr[j] > pivot);
    if (i >= j) {
      return j;
    }
    swap(arr[i], arr[j]);
  }
}

void hoareSortRange(Array& arr, int left, int right) {
  if (left >= right) {
    return;
  }
  int center = hoarePart(arr, left, right);
  hoareSortRange(arr, left, center);
  hoareSortRange(arr, center + 1, right);
}

Array hoareSort(Array arr) {
  hoareSortRange(arr, 0, SIZE - 1);
  return arr;
}

void shiftDown(Array& arr, int root, int n) {
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
    if (larg == root) {
      return;
    }
    swap(arr[root], arr[larg]);
    root = larg;
  }
}

Array heapSort(Array arr) {
  for (int i = SIZE / 2 - 1; i >= 0; i--) {
    shiftDown(arr, i, SIZE);
  }
  for (int i = SIZE - 1; i > 0; i--) {
    swap(arr[0], arr[i]);
    shiftDown(arr, 0, i);
  }
  return arr;
}

void printArray(Array arr) {
  cout << "[";
  for (int i = 0; i < SIZE; i++) {
    cout << arr[i];
    if (i < SIZE - 1) {
      cout << ", ";
    }

  }
  cout << "]" << endl;

}


int main() {
  Array arr = { 12, 4, 7, 3, 5, 9, 8, 18, 1, 2 };
  cout << "Bubble sort:" << "\n";
  Array bubble = bubbleSort(arr);
  printArray(bubble);
  cout << "\n\n";

  cout << "Select sort:" << "\n";
  Array select = selectionSort(arr);
  printArray(select);
  cout << "\n\n";

  cout << "Insertion sort:" << "\n";
  Array insertion = insertionSort(arr);
  printArray(insertion);
  cout << "\n\n";

  cout << "Merge sort:" << "\n";
  Array merge = mergeSort(arr);
  printArray(merge);
  cout << "\n\n";

  cout << "Hoare sort:" << "\n";
  Array hoare = hoareSort(arr);
  printArray(hoare);
  cout << "\n\n";

  cout << "Heap sort:" << "\n";
  Array heap = heapSort(arr);
  printArray(heap);
}
