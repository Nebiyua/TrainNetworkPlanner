#include "../include/Graph.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

Graph::Graph() : nextId(1) {} // Start IDs from 1

// Add a new station
void Graph::addStation(string name, string code) {
    stationRegistry.addStation(name, code, nextId);
    actionHistory.push("Added Station: " + name);
    cout << "Success: Station '" << name << "' added with ID " << nextId << endl;
    nextId++;
}

// Add a track between two stations
void Graph::addTrack(string fromStation, string toStation, int dist, int time) {
    BSTNode* sourceNode = stationRegistry.searchStation(fromStation);
    BSTNode* destNode = stationRegistry.searchStation(toStation);

    if (!sourceNode) { cout << "Error: Source station '" << fromStation << "' not found!\n"; return; }
    if (!destNode) { cout << "Error: Destination station '" << toStation << "' not found!\n"; return; }

    sourceNode->tracks.addTrack(destNode->data.id, dist, time);
    actionHistory.push("Added Track: " + fromStation + " -> " + toStation);
    cout << "Success: Track added from " << fromStation << " to " << toStation << endl;
}

// List all stations alphabetically
void Graph::listStations() {
    cout << "\n--- Network Stations (Alphabetical) ---\n";
    stationRegistry.printAllStations();
}

// Placeholder
void Graph::checkConnectivity() {
    cout << "Feature coming soon..." << endl;
}

// Placeholder
void Graph::getShortestPath(string start, string end) {
    cout << "Feature coming soon..." << endl;
}

// Save stations and tracks to CSV files
void Graph::saveData() {
    ofstream stationFile("data/stations.csv");
    if (stationFile.is_open()) {
        stationRegistry.saveStationsToFile(stationFile);
        stationFile.close();
        cout << "💾 Saved stations to data/stations.csv\n";
    } else {
        cout << "❌ Error: Could not open stations.csv for writing.\n";
    }

    ofstream trackFile("data/tracks.csv");
    if (trackFile.is_open()) {
        stationRegistry.saveTracksToFile(trackFile);
        trackFile.close();
        cout << "💾 Saved tracks to data/tracks.csv\n";
    } else {
        cout << "❌ Error: Could not open tracks.csv for writing.\n";
    }
}

// Load stations and tracks from CSV files
void Graph::loadData() {
    string line;

    // Load stations
    ifstream stationFile("data/stations.csv");
    if (stationFile.is_open()) {
        cout << "📂 Loading Stations...\n";
        while (getline(stationFile, line)) {
            stringstream ss(line);
            string name, code;
            if (getline(ss, name, ',') && getline(ss, code, ',')) {
                stationRegistry.addStation(name, code, nextId);
                nextId++;
            }
        }
        stationFile.close();
    } else {
        cout << "⚠️ No saved stations found (starting fresh).\n";
    }

    // Load tracks
    ifstream trackFile("data/tracks.csv");
    if (trackFile.is_open()) {
        cout << "📂 Loading Tracks...\n";
        while (getline(trackFile, line)) {
            stringstream ss(line);
            string from, to, distStr, timeStr;
            if (getline(ss, from, ',') && getline(ss, to, ',') &&
                getline(ss, distStr, ',') && getline(ss, timeStr, ',')) {
                
                try {
                    int dist = stoi(distStr);
                    int time = stoi(timeStr);
                    BSTNode* src = stationRegistry.searchStation(from);
                    BSTNode* dst = stationRegistry.searchStation(to);
                    if (src && dst) src->tracks.addTrack(dst->data.id, dist, time);
                } catch (...) {
                    continue; // Skip invalid lines
                }
            }
        }
        trackFile.close();
    }
}

// BFS to check if a path exists between two stations
bool Graph::isPathExisting(string start, string end) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);
    if (!startNode || !endNode) return false;

    int startId = startNode->data.id;
    int targetId = endNode->data.id;

    Queue q;
    q.enqueue(startId);
    vector<int> visited{startId};

    while (!q.isEmpty()) {
        int currentId = q.dequeue();
        if (currentId == targetId) return true;

        BSTNode* currentNode = stationRegistry.searchStationById(currentId);
        if (!currentNode) continue;

        for (Track* track = currentNode->tracks.getHead(); track; track = track->next) {
            int neighborId = track->destinationStationId;
            if (find(visited.begin(), visited.end(), neighborId) == visited.end()) {
                visited.push_back(neighborId);
                q.enqueue(neighborId);
            }
        }
    }
    return false;
}

// Dijkstra-like algorithm to find the fastest route
void Graph::getFastestRoute(string start, string end) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);
    if (!startNode || !endNode) {
        cout << "One of the stations does not exist.\n";
        return;
    }

    int startId = startNode->data.id;
    int endId = endNode->data.id;

    map<int, int> dist;          // Time from start
    map<int, int> previous;      // For path reconstruction
    map<int, bool> visited;

    dist[startId] = 0;

    for (int i = 0; i < 20; i++) { // Assume max 20 stations
        int u = -1;
        int minVal = INT_MAX;

        for (auto const& [id, d] : dist) {
            if (!visited[id] && d < minVal) {
                minVal = d;
                u = id;
            }
        }

        if (u == -1 || u == endId) break;
        visited[u] = true;

        BSTNode* uNode = stationRegistry.searchStationById(u);
        if (!uNode) continue;

        for (Track* track = uNode->tracks.getHead(); track; track = track->next) {
            int v = track->destinationStationId;
            int weight = track->weightTime;

            if (dist.find(v) == dist.end()) dist[v] = INT_MAX;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                previous[v] = u;
            }
        }
    }

    if (dist.find(endId) == dist.end() || dist[endId] == INT_MAX) {
        cout << "No route exists between " << start << " and " << end << endl;
    } else {
        cout << "Fastest Route Time: " << dist[endId] << " minutes" << endl;
        // Path reconstruction could be added here
    }
}
