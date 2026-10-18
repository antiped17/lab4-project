#include <iostream>

using namespace std;

const int max_size = 100;

struct Set {
	int data[max_size];
	int n = 0;
};

void pushBack(Set& s, int value) {
	if (s.n < max_size) {
		s.data[s.n] = value;
		s.n++;
	}
}
Set setUnion(Set& a, Set& b) {
	Set r;
	int i = 0;
	int j = 0;
	while (i < a.n && j < b.n) {
		if (a.data[i] < b.data[j]) {
			pushBack(r, a.data[i]);
			i++;
		}
		else if (a.data[i] > b.data[j]) {
			pushBack(r, b.data[j]);
			j++;
		}
		else {
			pushBack(r, a.data[i]);
			i++;
			j++;
		}
	}
	while (i<a.n) {
		pushBack(r, a.data[i]);
		i++;
	}
	while (j < b.n) {
		pushBack(r, b.data[j]);
		j++;
	}
	return r;
}
Set setIntersection(Set& a, Set& b) {
	Set r;
	int i = 0;
	int j = 0;
	while (i < a.n && j < b.n) {
		if (a.data[i] < b.data[j]) {
			i++;
		}
		else if (a.data[i] > b.data[j]) {
			j++;
		}
		else {
			pushBack(r, a.data[i]);
			i++;
			j++;
		}
	}
	return r;
}
Set setDifference(Set& a, Set& b) {
	Set r;
	int i = 0;
	int j = 0;
	while (i < a.n && j < b.n) {
		if (a.data[i] < b.data[j]) {
			pushBack(r, a.data[i]);
			i++;
		}
		else if (a.data[i] > b.data[j]) {
			j++;
		}
		else {
			i++;
			j++;
		}
	}
	while (i < a.n) {
		pushBack(r, a.data[i]);
		i++;
	}
	return r;
}
void printSet(Set& s) {
	for (int i = 0; i < s.n; i++) {
		cout << s.data[i] << " ";
	}
	cout << "\n";
}
int main() {
	Set A = { {1, 4, 5, 8, 13}, 5 };
	Set B = { {2, 3, 4, 6, 7, 8}, 6 };
	Set U = setUnion(A, B);
	Set I = setIntersection(A, B);
	Set D = setDifference(A, B);

	cout << "A = ";
	printSet(A);
	cout << "B = ";
	printSet(B);
	cout << "U = ";
	printSet(U);
	cout << "I = ";
	printSet(I);
	cout << "D = ";
	printSet(D);
}
