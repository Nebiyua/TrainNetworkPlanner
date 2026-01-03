#ifndef GRAPH_H
#define GRAPH_H

#include "BST.h"
#include "Stack.h"
#include "Queue.h"
#include <vector>
#include <string>

using namespace std;

class Graph {
private:
    BST stationRegistry;   // Stores all stations
    Stack actionHistory;   // Tracks user actions
    int nextId;            // Auto-increment station ID

public:
    Graph();

    // Basic operations
    void addStation(string name, string code);
    void addTrack(string fromStation, string toStation, int dist, int time);
    void listStations();

    // Connectivity (BFS)
    bool isPathExisting(string start, string end);
    bool isDirectlyConnected(string start, string end); 

    // Shortest path
    void getShortestPath(string start, string end, bool byTime);
    void getFastestRoute(string start, string end);

    //Analytics
    void displayNetworkStats();

    // File I/O
    void loadData();
    void saveData();
};

#endif
