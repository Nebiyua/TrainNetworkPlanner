#ifndef GRAPH_H
#define GRAPH_H

#include "BST.h"
#include "Stack.h"
#include "Queue.h"
#include <vector>

class Graph {
private:
    BST stationRegistry; // Our phonebook of stations
    Stack actionHistory; // Our undo history
    int nextId;          // Auto-increment ID for new stations

public:
    Graph();
    
    // Core Functions
    void addStation(string name, string code);
    void addTrack(string fromStation, string toStation, int dist, int time);
    void listStations();

    // Advanced
    void getShortestPath(string start, string end);
    void checkConnectivity();

    // File I/O
    void loadData();
    void saveData();
};

#endif