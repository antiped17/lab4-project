#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std; 

const int MAXVAL = 100;

void fillArr(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = rand() % MAXVAL + 1;
	}
}

int removeDub(int arr[], int size, int result[]) {
	int resultSize = 0;
	for (int i = 0; i < size; i++) {
		bool dublicates = false;
		for (int j = 0; j < resultSize; j++) {
			if (arr[i] == result[j]) {
				dublicates = true;
				break;
			}
		}
		if (!dublicates) {
			result[resultSize] = arr[i];
			resultSize++;
		}
	}
	return resultSize;
}
int removeDub1(int arr[], int size, int result[]) {
	bool used[MAXVAL + 1] = { false };
	int resultSize = 0;
	for (int i = 0; i < size; i++) {
		int n = arr[i];
		if (!used[n]) {
			used[n] = true;
			result[resultSize] = arr[i];
			resultSize++;
		}
	}
	return resultSize;
}

int main() {
	int sizes[] = { 100, 200, 500, 1000, 10000, 50000, 100000, 1000000 };
	const int sizeCount = sizeof(sizes) / sizeof(sizes[0]);
	int* arr = new int[1000000];
	int* result = new int[1000000];
	srand(time(0));
	cout << setw(10) << "n" << setw(18) << "removeDub, ms" << setw(18) << "removeDub1, ms" << endl;
	for (int i = 0; i < sizeCount; i++) {
		fillArr(arr, sizes[i]);
		clock_t start = clock();
		removeDub(arr, sizes[i], result);
		clock_t end = clock();
		double time1 = (double)(end - start) / CLOCKS_PER_SEC * 1000;
		start = clock();
		removeDub1(arr, sizes[i], result);
		end = clock();
		double time2 = (double)(end - start) / CLOCKS_PER_SEC * 1000;
		cout << setw(10) << sizes[i] << setw(18) << time1 << setw(18) << time2 << endl;
	}

	delete[] arr;
	delete[] result;

	return 0;
}

