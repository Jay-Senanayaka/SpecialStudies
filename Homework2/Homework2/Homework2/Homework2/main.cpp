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
bool* in_closed;
//unordered_set<int> in_closed;
double* dist;

//landmark variables
int** toDist;
int** fromDist;
int* landmarks;

//test variables
int* sources = new int[10];
int* destinations = new int[100];

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

void Dijkstra(int start_vertex, int end_vertex) {
	
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;
	//queue<int> closed;
	in_closed = new bool[GraphWithWeights.size()];
	for (int i = 0; i < GraphWithWeights.size(); i++)
		in_closed[i] = false;

	//Used this since I did not want to iterate through priority queue to see if the child exists
	//unordered_set<int> in_open;

	// Initialization
	for (int i = 0; i < GraphWithWeights.size(); i++) {
		if (i == start_vertex) {
			dist[i] = 0;
			open.push(make_pair(0, start_vertex));
			//in_open.insert(start_vertex);
			//counter++;

		}
		else {
			dist[i] = INT_MAX;
		}

	}

	while (open.size() > 0) {
		int current_node = open.top().second;
		open.pop();
		//closed.push(current_node);

		//if (in_closed.find(current_node) != in_closed.end())
		if(in_closed[current_node])
			continue;

		//lazy work 
		//in_open.erase(current_node);
		
		//in_closed.insert(current_node);
		in_closed[current_node] = true;

		if (current_node == end_vertex) {
			//cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			counter++;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length, GraphWithWeights[current_node][i].first));
				//in_open.insert(GraphWithWeights[current_node][i].first);
				//counter++;
			//}
			}


		}

	}

	//return INT_MIN;

}

void Dijkstra(int start_vertex) {
	dist = new double[Coordinates.size()];
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;
	//queue<int> closed;
	in_closed = new bool[GraphWithWeights.size()];
	for (int i = 0; i < GraphWithWeights.size(); i++)
		in_closed[i] = false;

	//Used this since I did not want to iterate through priority queue to see if the child exists
	//unordered_set<int> in_open;

	// Initialization
	for (int i = 0; i < GraphWithWeights.size(); i++) {
		if (i == start_vertex) {
			dist[i] = 0;
			open.push(make_pair(0, start_vertex));
			//in_open.insert(start_vertex);
			//counter++;

		}
		else {
			dist[i] = INT_MAX;
		}

	}

	while (open.size() > 0) {
		int current_node = open.top().second;
		open.pop();
		//closed.push(current_node);

		//if (in_closed.find(current_node) != in_closed.end())
		if (in_closed[current_node])
			continue;

		//lazy work 
		//in_open.erase(current_node);

		//in_closed.insert(current_node);
		in_closed[current_node] = true;

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			//counter++;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length, GraphWithWeights[current_node][i].first));
				//in_open.insert(GraphWithWeights[current_node][i].first);
				//counter++;
			//}
			}


		}
	}
}

double EuclideanDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void AStar(int start_vertex, int end_vertex) {
	dist = new double[Coordinates.size()];
	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> open;
	//queue<int> closed;

	in_closed = new bool[GraphWithWeights.size()];
	for (int i = 0; i < GraphWithWeights.size(); i++)
		in_closed[i] = false;

	//Used this since I did not want to iterate through priority queue to see if the child exists
	//unordered_set<int> in_open;

	// Initialization
	for (int i = 0; i < Coordinates.size(); i++) {
		if (i == start_vertex) {
			dist[i] = 0;
			open.push(make_pair(0,start_vertex));
			//in_open.insert(start_vertex);
			//counter++;

		}
		else {
			dist[i] = INT_MAX;
		}
		
	}

	while (open.size() > 0) {
		int current_node = open.top().second;
		open.pop();
		//closed.push(current_node);
		
		//if (in_closed.find(current_node) != in_closed.end())
		if (in_closed[current_node])
			continue;

		//lazy work 
		//in_open.erase(current_node);

		//in_closed.insert(current_node);
		in_closed[current_node] = true;

		if (current_node == end_vertex) {
			cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			counter++;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));
					
				open.push(make_pair(length + EuclideanDistance(Coordinates[current_node].first, Coordinates[current_node].second, Coordinates[end_vertex].first, Coordinates[end_vertex].second), GraphWithWeights[current_node][i].first));
					//in_open.insert(GraphWithWeights[current_node][i].first);
					//counter++;
				//}
			}
			

		}

	}
	
}

double rLHeuristic(int startNode, int endNode, int numLandmarks) {
	
	double l = 0;
	double max = INT_MIN;
	for (int i = 0; i < numLandmarks; i++) {
		l = abs(fromDist[i][startNode] - fromDist[i][endNode]);
		if (l > max)
			max = l;
	}

	return max;
}

void populateLandmarks(int numLandmarks) {
	//toDist = new int*[numLandmarks];
	fromDist = new int*[numLandmarks];
	landmarks = new int[numLandmarks];

	for (int i = 0; i < numLandmarks; i++) {
		//toDist[i] = new int[GraphWithWeights.size()];
		fromDist[i] = new int[GraphWithWeights.size()];
	}

	srand(time(NULL));
	int l = 0;
	for (int i = 0; i < numLandmarks; i++) {
		l = rand() % GraphWithWeights.size();
		landmarks[i] = l;
	}

	double d1 = 0;
	double d2 = 0;
	for (int i = 0; i < numLandmarks; i++) {
		
			//d1 = Dijkstra(j, landmarks[i]);
			Dijkstra(landmarks[i]);
			//cout << "round " << j << endl;
			//toDist[i][j] = d1;
			for (int j = 0; j < GraphWithWeights.size(); j++)
				fromDist[i][j] = dist[j];
	}
		
}

void AStarRandomLandmarks(int start_vertex, int end_vertex, int numLandmarks) {
	cout << "start" << endl;
	dist = new double[Coordinates.size()];
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;

	in_closed = new bool[GraphWithWeights.size()];
	for (int i = 0; i < GraphWithWeights.size(); i++)
		in_closed[i] = false;

	for (int i = 0; i < Coordinates.size(); i++) {
		if (i == start_vertex) {
			dist[i] = 0;
			open.push(make_pair(0, start_vertex));
			//in_open.insert(start_vertex);
			//counter++;

		}
		else {
			dist[i] = INT_MAX;
		}

	}

	while (open.size() > 0) {
		int current_node = open.top().second;
		open.pop();
		//closed.push(current_node);

		//if (in_closed.find(current_node) != in_closed.end())
		if (in_closed[current_node])
			continue;

		//lazy work 
		//in_open.erase(current_node);

		//in_closed.insert(current_node);
		in_closed[current_node] = true;

		if (current_node == end_vertex) {
			cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			counter++;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length + rLHeuristic(current_node, GraphWithWeights[current_node][i].first,numLandmarks), GraphWithWeights[current_node][i].first));
				//in_open.insert(GraphWithWeights[current_node][i].first);
				//counter++;
			//}
			}


		}

	}

}

void GenerateEuclidianHeuristics(double * heuristics, int end_vertex) {
	for (int i = 0; i < Coordinates.size(); i++) {
		heuristics[i] = EuclideanDistance(Coordinates[i].first, Coordinates[i].second, Coordinates[end_vertex].first, Coordinates[end_vertex].second);
	}
}

void test() {

	for (int i = 0; i < 6; i++) {
		vector<pair<int, int>> temp;
		GraphWithWeights.push_back(temp);
	}
	GraphWithWeights[0].push_back(make_pair(1, 3));
	GraphWithWeights[0].push_back(make_pair(2, 4));
	GraphWithWeights[1].push_back(make_pair(3, 7));
	GraphWithWeights[1].push_back(make_pair(4, 5));
	GraphWithWeights[2].push_back(make_pair(4, 8));
	GraphWithWeights[3].push_back(make_pair(5, 1));
	GraphWithWeights[4].push_back(make_pair(5, 2));
	GraphWithWeights[5].push_back(make_pair(5, 0));
}


int main() {
	string lenFilePath = "C:/Users/IDP-admin/source/repos/SpecialStudies/Homework2/Homework2/Homework2/Homework2/Homework2/NewYork.len";
	string coFilePath = "C:/Users/IDP-admin/source/repos/SpecialStudies/Homework2/Homework2/Homework2/Homework2/Homework2/NewYork.co";

	GenerateGraph(lenFilePath);
	GenerateCoordinateGraph(coFilePath);
	

	populateLandmarks(4);
	
	//dist = new double[GraphWithWeights.size()];
	//test();
	/*double * heuristic = new double[Coordinates.size()];*/

	srand(time(NULL));
	double duration = 0;
	int average_edges_relaxed = 0;
	//GenerateEuclidianHeuristics(heuristic, end_vertex);

	for (int i = 0; i < 10; i++) {
		sources[i] = rand() % Coordinates.size();
	}
	for (int i = 0; i < 100; i++)
		destinations[i] = rand() % Coordinates.size();

	

	int rounds = 1;
	double d = 0;
	for (int i = 0; i < rounds; i++) {
		int end_vertex = rand() % Coordinates.size();
		//int end_vertex = rand() % GraphWithWeights.size();
		int start_vertex = end_vertex;
		while (end_vertex == start_vertex) {
			start_vertex = rand() % Coordinates.size();
			//start_vertex = rand() % GraphWithWeights.size();
		}
		//cout << "Starting Round " << i+1 << "..." << endl;
		auto started = chrono::high_resolution_clock::now();
		//AStar(start_vertex, end_vertex);
		//Dijkstra(sources, destinations);
		AStarRandomLandmarks(start_vertex, end_vertex, 4);
		auto done = chrono::high_resolution_clock::now();
		duration += chrono::duration_cast<chrono::milliseconds>(done - started).count();
		average_edges_relaxed += counter;
		counter = 0;
		//cout << "dist = " << d << endl;
		//cout << "time = " << chrono::duration_cast<chrono::milliseconds>(done - started).count() << endl;
	}
	
	
	cout << "Time (ms): " << duration/1 << endl;
	cout << "Edges relaxed: " << average_edges_relaxed / 1 << endl;
	
}