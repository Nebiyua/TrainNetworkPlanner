#include "../include/Graph.h"
#include <iostream>

using namespace std;

Graph::Graph() {
    nextId = 1; // Start IDs from 1
}

void Graph::addStation(string name, string code) {
    stationRegistry.addStation(name, code, nextId);
    
    actionHistory.push("Added Station: " + name);
    
    nextId++;
    
    cout << "Success: Station '" << name << "' added with ID " << (nextId - 1) << endl;
}

void Graph::addTrack(string fromStation, string toStation, int dist, int time) {
    BSTNode* sourceNode = stationRegistry.searchStation(fromStation);
    
    BSTNode* destNode = stationRegistry.searchStation(toStation);

    if (sourceNode == nullptr) {
        cout << "Error: Source station '" << fromStation << "' not found!" << endl;
        return;
    }
    if (destNode == nullptr) {
        cout << "Error: Destination station '" << toStation << "' not found!" << endl;
        return;
    }

    sourceNode->tracks.addTrack(destNode->data.id, dist, time);

    actionHistory.push("Added Track: " + fromStation + " -> " + toStation);
    
    cout << "Success: Track added from " << fromStation << " to " << toStation << endl;
}

void Graph::listStations() {
    cout << "\n--- Network Stations (Alphabetical) ---" << endl;
    stationRegistry.printAllStations();
}

void Graph::checkConnectivity() {
    cout << "Feature coming soon..." << endl;
}

void Graph::getShortestPath(string start, string end) {
    cout << "Feature coming soon..." << endl;
}

void Graph::loadData() {}
void Graph::saveData() {}