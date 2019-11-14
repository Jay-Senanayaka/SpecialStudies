#include <iostream>
#include <fstream>
#include <sstream>
#include<string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <time.h>
#include <chrono>
using namespace std;

// Global variables
vector<vector<pair<int, int>>> GraphWithWeights;
vector<pair<int, int>> Coordinates;
int counter = 0;

// Function to generate the graph
void GenerateGraph(string lenFilePath) {
	ifstream dataFile;

	dataFile.open(lenFilePath, std::ios_base::in);
	int currentNode = -1;
	int source_vertex;
	int destination_vertex;
	int weight;
	string line;
	
	if (dataFile.is_open()) {
		
		getline(dataFile, line);


		while (getline(dataFile, line)) {
			istringstream  readline(line);
			readline >> source_vertex >> destination_vertex >> weight;

			// Check if the source vertex is the same as the current one
			// If it is not, create a new vector of pairs and push it 
			// Else push it to the same position
			if (source_vertex != currentNode) {
				currentNode = source_vertex;
				vector<pair<int, int>> temp;
				temp.push_back(make_pair(destination_vertex, weight));
				GraphWithWeights.push_back(temp);
			}
			else {
				GraphWithWeights[currentNode].push_back(make_pair(destination_vertex, weight));
			}

		}
	}
}

void GenerateCoordinateGraph(string coFilePath) {
	ifstream dataFile;
	string line;
	int vertex;
	double x_coord;
	double y_coord;

	dataFile.open(coFilePath, std::ios_base::in);

	if (dataFile.is_open()) {
		
		getline(dataFile, line);

		while (getline(dataFile, line)) {
			istringstream  readline(line);
			readline >> vertex >> x_coord >> y_coord;
			Coordinates.push_back(make_pair(x_coord, y_coord));
		}
	}

}

double EuclideanDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


void AStar(int start_vertex, int end_vertex) {
	vector<double> dist;
	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> open;
	//queue<int> closed;

	//Used this since I did not want to iterate through priority queue to see if the child exists
	unordered_set<int> in_open;
	//unordered_set<int> in_closed;

	// Initialization
	for (int i = 0; i < Coordinates.size(); i++) {
		if (i == start_vertex) {
			dist.push_back(0);
			open.push(make_pair(0,start_vertex));
			in_open.insert(start_vertex);
			counter++;
		}
		else {
			dist.push_back(INT_MAX);
		}
		
	}

	while (open.size() > 0) {
		int current_node = open.top().second;
		open.pop();
		//closed.push(current_node);
		
		//lazy work 
		in_open.erase(current_node);
		//in_closed.insert(current_node);

		if (current_node == end_vertex) {
			cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;

			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));
					open.push(make_pair(length + EuclideanDistance(Coordinates[current_node].first, Coordinates[current_node].second, Coordinates[end_vertex].first, Coordinates[end_vertex].second), GraphWithWeights[current_node][i].first));
					in_open.insert(GraphWithWeights[current_node][i].first);
					counter++;
				}
			}
			

		}

	}
	
}

void GenerateEuclidianHeuristics(double * heuristics, int end_vertex) {
	for (int i = 0; i < Coordinates.size(); i++) {
		heuristics[i] = EuclideanDistance(Coordinates[i].first, Coordinates[i].second, Coordinates[end_vertex].first, Coordinates[end_vertex].second);
	}
}

int main() {
	string lenFilePath = "C:/Users/JaY/Documents/UWW- Graduate/Fall 2019/Special Studies/Homework/Homework2/Data/NewYork.len";
	string coFilePath = "C:/Users/JaY/Documents/UWW- Graduate/Fall 2019/Special Studies/Homework/Homework2/Data/NewYork.co";

	GenerateGraph(lenFilePath);
	/*cout << GraphWithWeights[0][1].first << GraphWithWeights[0][1].second << endl;
	cout << GraphWithWeights[1][1].first << GraphWithWeights[1][1].second << endl;*/

	GenerateCoordinateGraph(coFilePath);

	/*double * heuristic = new double[Coordinates.size()];*/

	srand(time(NULL));
	double duration = 0;
	int average_edges_relaxed = 0;
	//GenerateEuclidianHeuristics(heuristic, end_vertex);
	for (int i = 0; i < 10; i++) {
		int end_vertex = rand() % Coordinates.size();
		int start_vertex = end_vertex;
		while (end_vertex == start_vertex) {
			start_vertex = rand() % Coordinates.size();
		}
		cout << "Starting Round " << i+1 << "..." << endl;
		auto started = chrono::high_resolution_clock::now();
		AStar(start_vertex, end_vertex);
		auto done = chrono::high_resolution_clock::now();
		duration += chrono::duration_cast<chrono::milliseconds>(done - started).count();
		average_edges_relaxed += counter;
		counter = 0;
	}
	cout << "Time (ms): " << duration/10 << endl;
	cout << "Edges relaxed: " << average_edges_relaxed / 10 << endl;
	
}