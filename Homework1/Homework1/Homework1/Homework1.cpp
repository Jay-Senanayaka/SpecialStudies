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
void generateVector(vector<int> &A, int vector_size) {
	cout << "Generation started ..." << endl;
	//unordered_set<int> in_vector;
	int random_val;
	ofstream text_file("100000.txt");
	if (text_file.is_open()) {
		text_file << "{";
		while (A.size() <= vector_size)
		{
			srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			random_val = rand() % (2 * vector_size) + (vector_size);
			//unordered_set<int>::const_iterator itr = in_vector.find(random_val);
			//if (itr == in_vector.end()) {
				A.push_back(random_val);
				//in_vector.insert(random_val);
				text_file << random_val << " ,";
			//}
		}
		text_file << " }";
	}
	 
	//in_vector.empty();
	cout << "Generation completed!" << endl;
}

int Selection(vector<int> &A, int left, int right, int k) {
	int random_pivot = 0;
	do {
		random_pivot = left + rand() % right + left;
	} while (random_pivot >= A.size() || random_pivot > right || random_pivot < left);
		

	int position = partition(A, left, right, random_pivot);

	if (position + 1 == k) {
		return A[position];
	}
	else if (position + 1 < k) {
		Selection(A, position, right, k);
	}
	else {
		Selection(A, left, position, k);
	}
			
}

int main()
{
	
	vector<int> A;
				
	int vector_size;
	cout << "Enter required vector size: ";
	cin >> vector_size;
	generateVector(A, vector_size);

	int k;
	cout << "Find Kth smallest where K is: ";
	cin >> k;

	int Kth_smallest = Selection(A, 0, A.size() - 1, k);

	cout << "Kth smallest is: " << Kth_smallest << endl;
	
}

