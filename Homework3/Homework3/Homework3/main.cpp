#include <iostream> 
#include <cstring> 
#include <algorithm> 
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#pragma region Global Variables
string line;
char *BWT;
char* fArray;

#pragma endregion

#pragma region Manber Myers Related

class Suffix {
public:
	int index;
	int rankFirst, rankSecond;
};

int compare(Suffix &a, Suffix &b) {
	return (a.rankFirst == b.rankFirst) ?
		(a.rankSecond < b.rankSecond ? 1 : 0) :
		(a.rankFirst < b.rankFirst ? 1 : 0);
}

/** A weaker MM implementation that runs in O(nlog^2 n) expected time
 * Replace sort with radix to improve to deterministic O(n log n)
 */
int *constructSA(char *txt, int n) {

	Suffix *suffixes = new Suffix[n];

	for (int i = 0; i < n; i++) {
		suffixes[i].index = i;
		suffixes[i].rankFirst = txt[i] - '$';
		suffixes[i].rankSecond = ((i + 1) < n) ? (txt[i + 1] - '$') : -1;
	}

	sort(suffixes, suffixes + n, compare); // TODO: Replace with Radix sort

	int *indexes = new int[n];
	for (int r = 4; r < 2 * n; r = r * 2) {

		int rank = 0;
		int prev_rank = suffixes[0].rankFirst;
		suffixes[0].rankFirst = rank;
		indexes[suffixes[0].index] = 0;

		for (int i = 1; i < n; i++) {

			if (suffixes[i].rankFirst == prev_rank
				&& suffixes[i].rankSecond == suffixes[i - 1].rankSecond) {
				prev_rank = suffixes[i].rankFirst;
				suffixes[i].rankFirst = rank;
			}
			else {
				prev_rank = suffixes[i].rankFirst;
				suffixes[i].rankFirst = ++rank;
			}
			indexes[suffixes[i].index] = i;
		}

		for (int i = 0; i < n; i++) {
			int nextindex = suffixes[i].index + r / 2;
			suffixes[i].rankSecond =
				(nextindex < n) ? suffixes[indexes[nextindex]].rankFirst : -1;
		}

		sort(suffixes, suffixes + n, compare);
	}

	int *suffixArr = new int[n];
	for (int i = 0; i < n; i++)
		suffixArr[i] = suffixes[i].index;

	return suffixArr;
}

#pragma endregion

#pragma region Input File to text

void getInputText(string inputFile) {
	ifstream dataFile;

	dataFile.open(inputFile, std::ios_base::in);
	

	if (dataFile.is_open()) {
		getline(dataFile, line);
		while (getline(dataFile, line)) {
			
		}
	}
}
#pragma endregion

#pragma region RLBWT Generation functions

void generateBWT(int *SA, int len) {
	BWT = new char[len + 1];
	for (int i = 0; i < len; i++) {
		if (SA[i] == 0) {
			BWT[i] = line[len - 1];
		}
		else {
			BWT[i] = line[SA[i] - 1];
		}
	}
}


void generateRLBWT(char* BWT, int len) {
	string str = "";
	char current = BWT[0];
	int counter = 1;
	for (int i = 1; i < len; i++) {
		
		if (BWT[i] == current) {
			counter++;
		}
		else {
			if (counter == 1) {
				str += current;
			}
			else {
				str += to_string(counter) + current;
			}
			current = BWT[i];
			counter = 1;
		}
	}
	str += to_string(counter) + current;

	ofstream myfile("C:/Users/IDP-admin/source/repos/SpecialStudies/Homework2/Homework3/Output/test.txt");
	if (myfile.is_open()) {
		myfile << str;
		myfile.close();
	}
	
}

#pragma endregion

#pragma region Tester functions
void printArray(int *A, int len) {
	for (int i = 0; i < len - 1; i++)
		cout << A[i] << ", ";
	cout << A[len - 1] << "]" << endl;
}

void printBWTArray(char *A, int len) {
	for (int i = 0; i < len - 1; i++)
		cout << A[i] << ", ";
	cout << A[len - 1] << "]" << endl;
}

void testCorrectness() {
	string str = "mississippi$";
	int len = str.length();
	char *string = new char[len];
	for (int i = 0; i < len; i++)
		string[i] = str.at(i);
	int *SA = constructSA(string, len);
	cout << "Suffix Array for " << str << ": [";
	printArray(SA, len);
}

void loadTest(int length) {
	char sigma[] = { '$', 'A', 'C', 'G', 'T' };
	char *str = new char[length + 1];
	int sigmaLen = sizeof(sigma) / sizeof(char);
	for (int i = 0; i < length; i++) {
		str[i] = sigma[1 + rand() % sigmaLen];
	}
	str[length] = '$';
	double startTime = clock();
	int *SA = constructSA(str, length);
	cout << "Time take to construct SA for " << length << "-length string: "
		<< (clock() - startTime) / CLOCKS_PER_SEC << "sec";
}

void decodeBWT(int *SA, int len) {

	fArray = new char[len];
	

	for (int i = 0; i < len; i++) {
		fArray[i] = line[SA[i]];
	}

	int fIndex = 0;
	int bCount = 0;
	int fCount = 0;
	char bwtVal = BWT[fIndex];
	string resultstr = "";

	while (bwtVal != '$') {
		
		resultstr = bwtVal + resultstr;

		for (int i = 0; i <= fIndex; i++) {
			if (BWT[i] == bwtVal)
				bCount++;
		}

		for (int i = 0; i < len; i++) {
			if (fArray[i] == bwtVal)
				fCount++;

			if (fCount == bCount) {
				fIndex = i;
				break;
			}
		}

		bwtVal = BWT[fIndex];

		bCount = 0;
		fCount = 0;
	}

	resultstr += bwtVal;
	
	cout << resultstr << endl;

}

void correctBWT() {
	string inputFile1 = "C:/Users/JaY/Documents/UWW- Graduate/Fall 2019/Special Studies/Homework/Homework3/Data/H.pylori.rlbwt";
	string inputFile2 = "C:/Users/JaY/Documents/UWW- Graduate/Fall 2019/Special Studies/Homework/Homework3/Output/H_pylori.txt";

	ifstream dataFile;

	dataFile.open(inputFile1, std::ios_base::in);
	ifstream dataFile2;

	dataFile.open(inputFile2, std::ios_base::in);

	string str1;
	string str2;

	if (dataFile.is_open()) {
		getline(dataFile, str1);
		while (getline(dataFile, str1)) {

		}
	}
	if (dataFile2.is_open()) {
		while (getline(dataFile2, str2)) {

		}
	}

	if (str1 == str2) {
		cout << "EQUAL" << endl;
	}
	else {
		cout << "FALSE" << endl;
	}
}
#pragma endregion

#pragma region Main method
int main() {
	srand(0);
	/*testCorrectness();
	loadTest(10000000);*/

	string inputFile = "C:/Users/IDP-admin/source/repos/SpecialStudies/Homework2/Homework3/Data/test.dna";
	getInputText(inputFile);

	char *string1 = new char[line.length() + 1];
	strcpy(string1, line.c_str());
	int len = line.length();
	int *SA = constructSA(string1, len);
	//cout << "Suffix Array for " << str << ": [";
	printArray(SA, len);

	generateBWT(SA, len);
	printBWTArray(BWT, len);
	generateRLBWT(BWT, len);
	decodeBWT(SA, len);

	/*correctBWT();*/
}
#pragma endregion