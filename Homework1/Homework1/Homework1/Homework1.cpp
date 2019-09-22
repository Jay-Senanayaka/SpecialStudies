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

// Function to swap 
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

struct IndexToVal
{
	int index, val;
};

bool Comparator(IndexToVal i, IndexToVal j) {
	return i.val < j.val;
}

// Function to partition around a pivot and return the new pivot location
int partition(vector<int> &A, int left, int right, int pivot) {
	
	swap(&A[right], &A[pivot]);

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
vector<int> generateVector( int vector_size) {
	vector<int> A;
	cout << "Generation started ..." << endl;
	//unordered_set<int> in_vector;
	int random_val;
	//ofstream text_file("100000.txt");
	//if (text_file.is_open()) {
		//text_file << "{";
		while (A.size() <= vector_size)
		{
			srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			random_val = rand() % (2 * vector_size) + (vector_size);
			//unordered_set<int>::const_iterator itr = in_vector.find(random_val);
			//if (itr == in_vector.end()) {
				A.push_back(random_val);
				//in_vector.insert(random_val);
				//text_file << random_val << " ,";
			//}
		}
		//text_file << " }";
	//}
	 
	//in_vector.empty();
	cout << "Generation completed!" << endl;
	return A;
}

int SelectionWithRandomPivot(vector<int> &A, int left, int right, int k) {
	int random_pivot = 0;
	do {
		random_pivot = left + rand() % right + left;
	} while (random_pivot >= A.size() || random_pivot > right || random_pivot < left);
		

	int position = partition(A, left, right, random_pivot);

	if (position + 1 == k) {
		return A[position];
	}
	else if (position + 1 < k) {
		SelectionWithRandomPivot(A, position, right, k);
	}
	else {
		SelectionWithRandomPivot(A, left, position, k);
	}
			
}

int SelectionWithMedianOfThreePivot(vector<int> &A, int left, int right, int k) {
	int pivot = 0;
		
	IndexToVal arr[] = { {left, A[left]}, {right, A[right]}, {left + right / 2, A[left + right / 2]} };

	sort(arr, arr + 3, Comparator);

	pivot = arr[1].index;
	int position = partition(A, left, right, pivot);

	if (position + 1 == k) {
		return A[position];
	}
	else if (position + 1 < k) {
		SelectionWithMedianOfThreePivot(A, position, right, k);
	}
	else {
		SelectionWithMedianOfThreePivot(A, left, position, k);
	}

}

int main()
{
	
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

