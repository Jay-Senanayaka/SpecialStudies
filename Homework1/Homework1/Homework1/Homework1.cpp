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
#include <string>
using namespace std;

// Function to swap 
void swap(int *a, int *b)
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
int partition(vector<int> &A, int left, int right, int pivot) {
	
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
vector<int> generateVector( int vector_size) {
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
			random_val = rand() % (2 * vector_size) + (vector_size);
			A.push_back(random_val);
			text_file << random_val << " ,";
							
		}
		text_file << " }";
	}
	 
	cout << "Generation completed!" << endl;
	return A;
}

// Selection algorithm to find the Kth smallest by using the pivot to be random
int SelectionWithRandomPivot(vector<int> &A, int left, int right, int k) {
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
int SelectionWithMedianOfThreePivot(vector<int> &A, int left, int right, int k) {
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

int main()
{
	srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	
	vector<int> A, B;			
	int vector_size;
	cout << "Enter required vector size: ";
	cin >> vector_size;
	A = generateVector(vector_size);
	B = A;
	
	int k;
	cout << "Find Kth smallest where K is: ";
	cin >> k;

	int Kth_smallest = 0;
	Kth_smallest = SelectionWithMedianOfThreePivot(B, 0, B.size() - 1, k);

	cout << "Kth smallest by median of 3 is: " << Kth_smallest << endl;

	Kth_smallest = SelectionWithRandomPivot(A, 0, A.size() - 1, k);

	cout << "Kth smallest by random pivot is : " << Kth_smallest << endl;

	
}

