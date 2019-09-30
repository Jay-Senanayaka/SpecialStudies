// medofmedv2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <unordered_set>
#include <random>
#include <chrono>
#include<fstream>
#include <string>
using namespace std;

// Function to swap 
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Structure that keeps the index to value mapping
struct IndexToVal
{
	int index, val;
};

// A comparator to check 
bool Comparator(IndexToVal i, IndexToVal j) {
	return i.val < j.val;
}


// Function to partition around a pivot and return the new pivot location
int partition(vector<int>& A, int left, int right, int pivot) {

	swap(&A[right], &A[pivot]);

	int pivot_val = A[right];
	int j = left;
	for (int i = left; i <= right - 1; i++) {
		if (A[i] < pivot_val) {
			swap(&A[i], &A[j]);
			j++;
		}
	}

	swap(&A[j], &A[right]);

	return j;

}

// Function to generate a vector of disctinct values
vector<int> generateVector(int vector_size) {
	vector<int> A;
	cout << "Generation started ..." << endl;

	int random_val;
	string file_name = to_string(vector_size) + ".txt";
	ofstream text_file;
	text_file.open(file_name.c_str());
	if (text_file.is_open()) {
		text_file << "{";
		while (A.size() < vector_size)
		{
			random_val = rand() % (2 * vector_size) + (0);
			A.push_back(random_val);
			text_file << random_val << " ,";

		}
		text_file << " }";
	}

	cout << "Generation completed!" << endl;
	return A;
}

int findMedian(vector<int> &arr, int start, int end, int n)
{


	int i, key, j;
	for (int i = start + 1; i <= end; i++) {
		key = arr[i];
		j = i - 1;

		while (j >= start && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}

		arr[j + 1] = key;
	}

	return arr[start + n / 2];
}


// Selection algorithm to find the Kth smallest by using the pivot to be random
int SelectionWithRandomPivot(vector<int>& A, int left, int right, int k) {
	int random_pivot = 0;
	do {
		random_pivot = left + rand() % (right + left);
	} while (random_pivot >= A.size() | random_pivot > right | random_pivot < left);


	int position = partition(A, left, right, random_pivot);

	if (position + 1 == k) {
		return A[position];
	}
	else if (position + 1 < k) {
		if (position + 1 <= right) {
			return SelectionWithRandomPivot(A, position + 1, right, k);
		}
		return INT_MAX;
	}
	else {
		if (position - 1 >= left) {
			return SelectionWithRandomPivot(A, left, position - 1, k);
		}

		return INT_MIN;
	}

}


// Selection algorithm to find the Kth smallest by using the pivot to be the median of three
int SelectionWithMedianOfThreePivot(vector<int>& A, int left, int right, int k) {
	int pivot = 0;

	int middle = (left + right) / 2;

	IndexToVal arr[] = { {left, A[left]}, {right, A[right]}, {middle, A[middle]} };

	sort(arr, arr + 3, Comparator);

	pivot = arr[1].index;

	int position = partition(A, left, right, pivot);

	if (position + 1 == k) {
		return A[position];
	}
	else if (position + 1 < k) {
		if (position + 1 <= right) {
			return SelectionWithMedianOfThreePivot(A, position + 1, right, k);
		}
		return INT_MAX;
	}
	else {
		if (position - 1 >= left) {
			return SelectionWithMedianOfThreePivot(A, left, position - 1, k);
		}

		return INT_MIN;
	}

}

int SelectionWithMedianOfMedian(vector<int>& A, int left, int right, int k) {

	int n = right - left + 1; // Number of elements in arr[l..r]
	if (left == right) return A[left];
	/*else if (n < 5) return findMedian(A, left, right, n);*/

	// If k is smaller than number of elements in array 
	if (k > 0 && k <= right - left + 1)
	{
		//int n = right - left + 1; // Number of elements in arr[l..r]
		// Divide arr[] in groups of size 5, calculate median 
		// of every group and store it in median[] array. 

		int size = n / 5;

		if (size * 5 < n)
			size++;

		int i = 0;
		vector<int> median(size); // There will be floor((n+4)/5) groups; 
		if (n >= 5) {
			for (i = 0; i < n / 5; i++) {
				median[i] = findMedian(A, i * 5 + left, i * 5 + 4 + left, 5);

			}
		}

		if (i * 5 < n) //For last group with less than 5 elements 
		{
			median[i] = findMedian(A, i * 5 + left, (i * 5 + left) + (n % 5 - 1), n % 5);
			i++;
		}


		// Find median of all medians using recursive call. 
		// If median[] has only one element, then no need 
		// of recursive call 

		int medOfMed = (size <= 2) ? median[0] :
			SelectionWithMedianOfMedian(median, 0, size - 1, size / 2);

		// Partition the array around a random element and 
		// get position of pivot element in sorted array 


		int s = 0;

		for (int i = left; i <= right; i++) {
			if (A[i] == medOfMed) {
				s = i;
				break;
			}

		}

		int pos = partition(A, left, right, s);


		// If position is same as k 
		if (pos - left == k - 1)
			return A[pos];
		else if (pos - left > k - 1) {  // If position is more, recur for left 
			//cout << "pos - " << pos - left << " and k " << k - 1 << "LEFT"<<endl;
			return SelectionWithMedianOfMedian(A, left, pos - 1, k);

		}

		// Else recur for right subarray
		else {

			return SelectionWithMedianOfMedian(A, pos + 1, right, k - pos + left - 1);
		}



	}

	// If k is more than number of elements in array 
	return INT_MAX;

}

int main()
{
	srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

	vector<int> A, B;
	int vector_size;
	cout << "Enter required vector size: ";
	cin >> vector_size;
	A = generateVector(vector_size);
	B = A;
	//C = A;

	int k1 = 10;
	int k2 = 1000;
	/*cout << "Find Kth smallest where K is: ";
	cin >> k;*/

	int Kth_smallest = 0;

	for (int i = 0; i < 3; i++) {

		clock_t begin = clock();
		Kth_smallest = SelectionWithMedianOfThreePivot(A, 0, A.size() - 1, k1);
		clock_t end = clock();
		cout << "Kth smallest by median of 3 is: " << Kth_smallest << endl;
		double duration = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Duration to find Kth smallest by median of 3: " << duration << "s" << endl;

		A = B;

		begin = clock();
		Kth_smallest = SelectionWithRandomPivot(A, 0, A.size() - 1, k1);
		end = clock();
		cout << "Kth smallest by random pivot is : " << Kth_smallest << endl;
		duration = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Duration to find Kth smallest by random pivot: " << duration << "s" << endl;

		A = B;

		begin = clock();
		Kth_smallest = SelectionWithMedianOfMedian(A, 0, A.size() - 1, k1);
		end = clock();
		cout << "Kth smallest by median of median is : " << Kth_smallest << endl;
		duration = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Duration to find Kth smallest by median of median: " << duration << "s" << endl;
		cout << endl << endl;

		A = B;
	}

	for (int i = 0; i < 3; i++) {

		clock_t begin = clock();
		Kth_smallest = SelectionWithMedianOfThreePivot(A, 0, A.size() - 1, k2);
		clock_t end = clock();
		cout << "Kth smallest by median of 3 is: " << Kth_smallest << endl;
		double duration = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Duration to find Kth smallest by median of 3: " << duration << "s" << endl;

		A = B;

		begin = clock();
		Kth_smallest = SelectionWithRandomPivot(A, 0, A.size() - 1, k2);
		end = clock();
		cout << "Kth smallest by random pivot is : " << Kth_smallest << endl;
		duration = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Duration to find Kth smallest by random pivot: " << duration << "s" << endl;

		A = B;

		begin = clock();
		Kth_smallest = SelectionWithMedianOfMedian(A, 0, A.size() - 1, k2);
		end = clock();
		cout << "Kth smallest by median of median is : " << Kth_smallest << endl;
		duration = double(end - begin) / CLOCKS_PER_SEC;
		cout << "Duration to find Kth smallest by median of median: " << duration << "s" << endl;
		cout << endl << endl;

		A = B;
	}
}
