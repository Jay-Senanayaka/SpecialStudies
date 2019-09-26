// Homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <unordered_set>
#include <random>
#include <chrono>
#include<fstream>
using namespace std;

struct RandomGenerator {
	
	int maxValue;
	RandomGenerator(int max) :
		maxValue(max) {
	}

	int operator()() {
		return rand() % maxValue;
	}

};

// Function to swap 
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Function to partition around a pivot and return the new pivot location
int partition(vector<int>& A, int left, int right, int pivot) {

	int s = 0;

	for (int i = left; i <= right; i++) {
		if (A[i] == pivot)
			s = i;
	}

	swap(&A[right], &A[s]);

	int pivot_val = A[right];
	int j = left;
	for (int i = left; i <= right - 1; i++) {
		if (A[i] <= pivot_val) {
			swap(&A[i], &A[j]);
			j++;
		}
	}

	swap(&A[j], &A[right]);

	return j;

}

// Function to generate a vector of disctinct values
void generateVector(vector<int>& A, int vector_size) {
	cout << "Generation started ..." << endl;
	srand(time(0));
	generate(A.begin(), A.end(), RandomGenerator(vector_size));

	cout << "Generation completed!" << endl;
}

//DEBUGGING-------------------------------------------------------------------
void printArray(vector<int> A, int start,int end) {
	for (int i = start; i <= end; i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}

int findMedian(vector<int> arr, int start, int end, int n)
{
	int i, key, j;
	for (int i = start +1; i <= end; i++) {
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}

		arr[j + 1] = key;
	}

	return arr[start + n / 2];
}

int Selection(vector<int>& A, int left, int right, int k) {
	
	// If k is smaller than number of elements in array 
	if (k > 0 && k <= right - left + 1)
	{
		int n = right - left + 1; // Number of elements in arr[l..r] 

		// Divide arr[] in groups of size 5, calculate median 
		// of every group and store it in median[] array. 

		int size = n / 5;

		if (size * 5 < n)
			size++;

		int i;
		vector<int> median(size); // There will be floor((n+4)/5) groups; 
		for (i = 0; i < n / 5; i++) {
			median[i] = findMedian(A, i * 5, i * 5 + 4, 5);
			
		}
		
		if (i * 5 < n) //For last group with less than 5 elements 
		{
			median[i] = findMedian(A,i*5,(i*5)+(n%5-1), n % 5);
			i++;
		}

		

		// Find median of all medians using recursive call. 
		// If median[] has only one element, then no need 
		// of recursive call 
		int medOfMed = (i == 1) ? median[i - 1] :
			Selection(median, 0, i - 1, i / 2);

		// Partition the array around a random element and 
		// get position of pivot element in sorted array 
	
		int pos = partition(A, left, right, medOfMed);
		
		// If position is same as k 
		if (pos - left == k - 1)
			return A[pos];
		if (pos - left > k - 1)  // If position is more, recur for left 
			return Selection(A, left, pos - 1, k);
		
		// Else recur for right subarray 
		return Selection(A, pos + 1, right, k - pos + left - 1);
	}

	// If k is more than number of elements in array 
	return INT_MAX;

}

int testAlgo(vector<int>& A, int k) {

	return 0;

}

//DEBUGGING--------------------------------------------------------
void printA(vector<int> A, int size) {
	
	int i, key, j;
	for (int i = 1; i < A.size(); i++) {
		key = A[i];
		j = i - 1;

		while (j >= 0 && A[j] > key) {
			A[j + 1] = A[j];
			j = j - 1;
		}

		A[j + 1] = key;
	}
	
	for (int i = 0; i < size; i++) {
		cout << A[i] << endl;
	}
}

int main()
{

	//vector<int> A;

	int vector_size;
	cout << "Enter required vector size: ";
	cin >> vector_size;

	vector<int> A(vector_size);
	generateVector(A, vector_size);

	//printA(A, vector_size);

	int k;
	cout << "Find Kth smallest where K is: ";
	cin >> k;

	int Kth_smallest = Selection(A, 0, A.size() - 1, k);

	//printA(A, vector_size);

	cout << "Kth smallest is: " << Kth_smallest << endl;

}

