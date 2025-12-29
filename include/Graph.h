#ifndef GRAPH_H
#define GRAPH_H

#include "BST.h"
#include "Stack.h"
#include "Queue.h" // Ensure this is here
#include <vector>
#include <string>

using namespace std;

class Graph {
private:
    BST stationRegistry; 
    Stack actionHistory; 
    int nextId;          

public:
    Graph();

    // 1. Basic Operations
    void addStation(string name, string code);
    void deleteStation(string name);
    void addTrack(string fromStation, string toStation, int dist, int time);
    void listStations();

    // 2. Connectivity (BFS)
    // Old placeholder (can keep or remove)
    void checkConnectivity(); 
    // NEW FUNCTION:
    bool isPathExisting(string start, string end);

    // 3. Shortest Path (Dijkstra)
    // Old placeholder (can keep or remove)
    void getShortestPath(string start, string end);
    // NEW FUNCTION:
    void getFastestRoute(string start, string end);

    // 4. File I/O
    void loadData();
    void saveData();
};

#endif