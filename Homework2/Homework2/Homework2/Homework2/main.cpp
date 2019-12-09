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
bool* in_closed;
double* dist;

//landmark variables
int** GBDist;
int** rLDist;
int* gridLandmarks;
int* landmarks;

//test variables
double* duration = new double[13];
int* avgEdges = new int[13];
int* sources = new int[10];
int* destinations = new int[100];

//bidirectional variables
bool* in_closedr;
double* distr;
double mu = 0;

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

void Dijkstra(int start_vertex, int end_vertex, int countIndex) {
	for (int i = 0; i < GraphWithWeights.size(); i++)
		dist[i] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;
	//queue<int> closed;

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
			avgEdges[countIndex] += 1;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length, GraphWithWeights[current_node][i].first));
				//in_open.insert(GraphWithWeights[current_node][i].first);
				
			//}
			}


		}

	}

	//return INT_MIN;

}


void BiDijkstra(int start_vertex, int end_vertex, int countIndex)
{
	for (int i = 0; i < GraphWithWeights.size(); i++)
		dist[i] = 0;
	for (int i = 0; i < GraphWithWeights.size(); i++)
		distr[i] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> openr;
	

	for (int i = 0; i < GraphWithWeights.size(); i++)
		in_closed[i] = false;
	for (int i = 0; i < GraphWithWeights.size(); i++)
		in_closedr[i] = false;


	// Initialization
	for (int i = 0; i < GraphWithWeights.size(); i++) {
		if (i == start_vertex) {
			dist[i] = 0;
			open.push(make_pair(0, start_vertex));

		}
		else {
			dist[i] = INT_MAX;
		}

		if (i == end_vertex) {
			distr[i] = 0;
			openr.push(make_pair(0, end_vertex));

		}
		else {
			distr[i] = INT_MAX;
		}

	}

	mu = INT_MAX;

	while (open.size() > 0 && openr.size() > 0) {
		int current_node = open.top().second;
		int current_noder = openr.top().second;

		if ((dist[current_node] + distr[current_noder]) >= mu)
			return;

		open.pop();
		
		if (in_closed[current_node])
			continue;

		in_closed[current_node] = true;

		if (current_node == end_vertex) {
			//cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			if (in_closed[GraphWithWeights[current_node][i].first])
				continue;

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			avgEdges[countIndex] += 1;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length, GraphWithWeights[current_node][i].first));
				
				if ((length + distr[GraphWithWeights[current_node][i].first]) < mu)
					mu = length + distr[GraphWithWeights[current_node][i].first];

			//}
			}
		}

		//reverse

		if (in_closedr[current_noder])
			continue;

		in_closedr[current_noder] = true;

		if (current_noder == start_vertex) {
			//cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_noder].size(); i++) {

			if (in_closedr[GraphWithWeights[current_noder][i].first])
				continue;

			double length = distr[current_noder] + GraphWithWeights[current_noder][i].second;
			avgEdges[countIndex] += 1;
			if (length < distr[GraphWithWeights[current_noder][i].first]) {
				distr[GraphWithWeights[current_noder][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				openr.push(make_pair(length, GraphWithWeights[current_noder][i].first));

				if ((length + dist[GraphWithWeights[current_noder][i].first]) < mu)
					mu = length + dist[GraphWithWeights[current_noder][i].first];

				//}
			}
		}

	}

}

void Dijkstra(int start_vertex) {
	
	for (int i = 0; i < GraphWithWeights.size(); i++)
		dist[i] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;
	//queue<int> closed;
	
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

void AStar(int start_vertex, int end_vertex, int countIndex) {
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

	//cout << "flag" << endl;

	while (open.size() > 0) {
		int current_node = open.top().second;
		open.pop();
		//closed.push(current_node);
		//cout << "open size = " << current_node << endl;
		
		//if (in_closed.find(current_node) != in_closed.end())
		if (in_closed[current_node])
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
			avgEdges[countIndex] += 1;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));
					
				open.push(make_pair(length + EuclideanDistance(Coordinates[GraphWithWeights[current_node][i].first].first, Coordinates[GraphWithWeights[current_node][i].first].second, Coordinates[end_vertex].first, Coordinates[end_vertex].second), GraphWithWeights[current_node][i].first));
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
		l = abs(rLDist[i][startNode] - rLDist[i][endNode]);
		if (l > max)
			max = l;
	}

	return max;
}

double GBHeuristic(int startNode, int endNode, int numLandmarks) {
	double l = 0;
	double max = INT_MIN;
	
	for (int i = 0; i < numLandmarks; i++) {
		l = abs(GBDist[i][startNode] - GBDist[i][endNode]);
		if (l > max)
			max = l;
	}

	return max;
}

void selectRandomLandmarks(int numLandmarks) {
	landmarks = new int[numLandmarks];	

	for (int i = 0; i < numLandmarks; i++) {
		rLDist[i] = new int[GraphWithWeights.size()];
	}

	srand(time(NULL));
	int l = 0;
	for (int i = 0; i < numLandmarks; i++) {
		l = rand() % GraphWithWeights.size();
		landmarks[i] = l;
	}
}

//pair<int, int> findClosestVertex(int middleOfBox_x, int middleOfBox_y, int x_interval, int y_interval) {
int findClosestVertex(int middleOfBox_x, int middleOfBox_y, int x_interval, int y_interval) {
		int boxBoundary_y_max = middleOfBox_y + (y_interval / 2);
		int boxBoundary_y_min = middleOfBox_y - (y_interval / 2);
		int boxBoundary_x_max = middleOfBox_x + (x_interval / 2);
		int boxBoundary_x_min = middleOfBox_x - (x_interval / 2);

		int eucDistanceToVertex = INT_MAX;
		int tempEuc = 0, x = 0, y = 0;

		int viableX = 0, viableY = 0;
		int viableI = 0;

		for (int i = 0; i < Coordinates.size(); i++) {
			x = Coordinates[i].first;
			y = Coordinates[i].second;

			if (x < boxBoundary_x_max && x > boxBoundary_x_min&& y < boxBoundary_y_max && y > boxBoundary_y_min) {
				tempEuc = EuclideanDistance(middleOfBox_x, middleOfBox_y, x, y);
				if (eucDistanceToVertex > tempEuc) {
					eucDistanceToVertex = tempEuc;
					viableI = i;
					//viableX = x;
					//viableY = y;
				}
			}
		}

		return viableI;
}

void selectGridLandMarks(int numberOfLandmarks) {

	gridLandmarks = new int[numberOfLandmarks];

	for (int i = 0; i < numberOfLandmarks; i++) {
		GBDist[i] = new int[GraphWithWeights.size()];
	}

	int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;

	for (int i = 0; i < Coordinates.size(); i++) {
		if (Coordinates[i].first <= min_x) {
			min_x = Coordinates[i].first;
		}

		if (Coordinates[i].first >= max_x) {
			max_x = Coordinates[i].first;
		}

		if (Coordinates[i].second <= min_y) {
			min_y = Coordinates[i].second;
		}

		if (Coordinates[i].second >= max_y) {
			max_y = Coordinates[i].second;
		}
	}

	//cout << "MAX X: " << max_x << " Min X: " << min_x << " MAX Y : " << max_y << " Min Y: " << min_y << endl;

	int divideGridBy = sqrt(numberOfLandmarks);

	int x_interval = (max_x - min_x) / divideGridBy;
	int y_interval = (max_y - min_y) / divideGridBy;

	int x_grid = min_x;
	int y_grid = min_y;

	int middleOfBox_x = min_x + (x_interval / 2);
	int middleOfBox_y = min_y + (y_interval / 2);
	int initialMidX = middleOfBox_x;
	for (int i = 0; i < numberOfLandmarks; i++) {
		gridLandmarks[i] = findClosestVertex(middleOfBox_x, middleOfBox_y, x_interval, y_interval);
		if (middleOfBox_x + x_interval < max_x) {
			middleOfBox_x += x_interval;
		}
		else {
			middleOfBox_y += y_interval;
			middleOfBox_x = initialMidX;
		}

	}

	/*cout << "SELECTED LANDMARKS ======" << endl;
	for (int i = 0; i < numberOfLandmarks; i++) {
		cout << "(X,Y): " << gridLandmarks[i] << "," << gridLandmarks[i] << endl;
	}*/
}

void populateLandmarks(int numLandmarks) {

	rLDist = new int* [numLandmarks];
	GBDist = new int* [numLandmarks];

	selectRandomLandmarks(numLandmarks);
	selectGridLandMarks(numLandmarks);

	for (int i = 0; i < numLandmarks; i++) {


		Dijkstra(landmarks[i]);

		for (int j = 0; j < GraphWithWeights.size(); j++)
			rLDist[i][j] = dist[j];

		Dijkstra(gridLandmarks[i]);

		for (int j = 0; j < GraphWithWeights.size(); j++)
			GBDist[i][j] = dist[j];
	}

}

void AStarRandomLandmarks(int start_vertex, int end_vertex, int numLandmarks, int countIndex) {

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
			//cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			avgEdges[countIndex] += 1;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length + rLHeuristic(GraphWithWeights[current_node][i].first, end_vertex,numLandmarks), GraphWithWeights[current_node][i].first));
				//in_open.insert(GraphWithWeights[current_node][i].first);
				//counter++;
			//}
			}


		}

	}

}

void AStarGridLandmarks(int start_vertex, int end_vertex, int numLandmarks, int countIndex) {

	for (int i = 0; i < GraphWithWeights.size(); i++)
		dist[i] = 0;
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> open;

	
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
			//cout << "Started at node : " << start_vertex << " and reached target node: " << current_node << ". Input end node :" << end_vertex << endl;
			return;
		}

		for (int i = 0; i < GraphWithWeights[current_node].size(); i++) {

			double length = dist[current_node] + GraphWithWeights[current_node][i].second;
			avgEdges[countIndex] += 1;
			if (length < dist[GraphWithWeights[current_node][i].first]) {
				dist[GraphWithWeights[current_node][i].first] = length;
				//if (in_open.find(GraphWithWeights[current_node][i].first) == in_open.end()) {
					//open.push(make_pair(length + heuristic[GraphWithWeights[current_node][i].first], GraphWithWeights[current_node][i].first));

				open.push(make_pair(length + GBHeuristic(GraphWithWeights[current_node][i].first, end_vertex, numLandmarks), GraphWithWeights[current_node][i].first));
				//in_open.insert(GraphWithWeights[current_node][i].first);
				//counter++;
			//}
			}


		}

	}

}

void writeFile() {

	ofstream myfile;
	myfile.open("vertexSet.txt");
	int counters = 0;
	for (int i = 0; i < 110; i++) {
		if (counters < 10) {
			myfile << sources[counters] << endl;
			counters++;
		}
		else {
			myfile << destinations[counters-10] << endl;
			counters++;
		}
	}

	myfile.close();

}

void readFile() {

	ifstream file("vertexSet.txt");

	int count = 0;
	int x;

	while (count < 110 && file >> x) {
		if (count < 10) {
			sources[count] = x;
			count++;
		}
		else {
			destinations[count - 10] = x;
			count++;
		}
	}

}

int main() {
	string lenFilePath = "C:/Users/IDP-admin/source/repos/SpecialStudies/Homework2/Homework2/Homework2/Homework2/Homework2/NewYork.len";
	string coFilePath = "C:/Users/IDP-admin/source/repos/SpecialStudies/Homework2/Homework2/Homework2/Homework2/Homework2/NewYork.co";

	GenerateGraph(lenFilePath);
	GenerateCoordinateGraph(coFilePath);
	
	//dist = new double[GraphWithWeights.size()];
	//test();
	/*double * heuristic = new double[Coordinates.size()];*/

	srand(time(NULL));
	
	unordered_set <int> vertices;
	int r = 0;
	for (int i = 0; i < 110; i++) {
		r = rand() % Coordinates.size();

		if (vertices.find(r) == vertices.end())
			vertices.insert(r);
	}

	unordered_set<int> ::iterator itr;
	int count = 0;
	int count2 = 0;
	for (itr = vertices.begin(); itr != vertices.end(); itr++) {
		
		if (count < 10) {
			sources[count] = *itr;
			count++;
		}
		
		if (count == 10) {
			destinations[count2] = *itr;
			count2++;
		}

	}

	writeFile();
		
	int* testSize = new int[5];
	int numtests = 5;
	for (int i = 0; i < numtests; i++)
		testSize[i] = (i+2)*(i+2);
	
	int sourcesSize = 10;
	int destinationsSize = 100;

	in_closed = new bool[GraphWithWeights.size()];
	dist = new double[Coordinates.size()];

	in_closedr = new bool[GraphWithWeights.size()];
	distr = new double[Coordinates.size()];

	for (int i = 0; i < 13; i++) {
		duration[i] = 0;
		avgEdges[i] = 0;
	}

	/*auto started = chrono::high_resolution_clock::now();
	auto done = chrono::high_resolution_clock::now();

	int counting = 0;
	for (int i = 0; i < sourcesSize; i++) {
		for (int j = 0; j < destinationsSize; j++) {
			
			started = chrono::high_resolution_clock::now();
			Dijkstra(sources[i], destinations[j], 0);
			done = chrono::high_resolution_clock::now();
			duration[0] += chrono::duration_cast<chrono::milliseconds>(done - started).count();
			
			started = chrono::high_resolution_clock::now();
			BiDijkstra(sources[i], destinations[j], 1);
			done = chrono::high_resolution_clock::now();
			duration[1] += chrono::duration_cast<chrono::milliseconds>(done - started).count();

			started = chrono::high_resolution_clock::now();
			AStar(sources[i], destinations[j], 2);
			done = chrono::high_resolution_clock::now();
			duration[2] += chrono::duration_cast<chrono::milliseconds>(done - started).count();
		
			
		}
	}

	int resultCount = 3;
	int resultCount2 = 8;
	
	for (int k = 0; k < numtests; k++) {
		cout << "round " << k << endl;
		populateLandmarks(testSize[k]);
		for (int i = 0; i < sourcesSize; i++) {
			for (int j = 0; j < destinationsSize; j++) {
				
				started = chrono::high_resolution_clock::now();
				AStarRandomLandmarks(sources[i], destinations[j], testSize[k], k+resultCount);
				done = chrono::high_resolution_clock::now();
				duration[k+resultCount] += chrono::duration_cast<chrono::milliseconds>(done - started).count();
				
				started = chrono::high_resolution_clock::now();
				AStarGridLandmarks(sources[i], destinations[j], testSize[k], k + resultCount2);
				done = chrono::high_resolution_clock::now();
				duration[k + resultCount2] += chrono::duration_cast<chrono::milliseconds>(done - started).count();
		

			}
		}
	}*/
	
	
	for (int i = 0; i < 13; i++)
		cout << "Avg. Time (ms): " << duration[i] / 1000 << "; Avg. num of edges relaxed: " << avgEdges[i] / 1000 << endl;
	
}