#include "../include/Graph.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits> // For INT_MAX (Infinity)
#include <map>     // Easier to map ID -> Distance
#include <fstream>  // For file reading/writing
#include <sstream>  // For string splitting (parsing CSV)

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

void Graph::saveData() {
    // 1. Save Stations
    ofstream stationFile("data/stations.csv");
    if (stationFile.is_open()) {
        stationRegistry.saveStationsToFile(stationFile);
        stationFile.close();
        cout << "💾 Saved stations to data/stations.csv" << endl;
    } else {
        cout << "❌ Error: Could not open stations.csv for writing." << endl;
    }

    // 2. Save Tracks
    ofstream trackFile("data/tracks.csv");
    if (trackFile.is_open()) {
        stationRegistry.saveTracksToFile(trackFile);
        trackFile.close();
        cout << "💾 Saved tracks to data/tracks.csv" << endl;
    } else {
        cout << "❌ Error: Could not open tracks.csv for writing." << endl;
    }
}

void Graph::loadData() {
    // 1. Load Stations
    ifstream stationFile("data/stations.csv");
    string line;
    
    if (stationFile.is_open()) {
        cout << "📂 Loading Stations..." << endl;
        while (getline(stationFile, line)) {
            stringstream ss(line);
            string name, code;
            
            // Parse CSV: Name,Code
            if (getline(ss, name, ',') && getline(ss, code, ',')) {
                // Use internal add function so we don't spam "Success" messages
                // Or just use the public one, but it will be noisy.
                // Let's use the public one for now:
                stationRegistry.addStation(name, code, nextId);
                nextId++;
            }
        }
        stationFile.close();
    } else {
        cout << "⚠️ No saved stations found (starting fresh)." << endl;
    }

    // 2. Load Tracks
    ifstream trackFile("data/tracks.csv");
    if (trackFile.is_open()) {
        cout << "📂 Loading Tracks..." << endl;
        while (getline(trackFile, line)) {
            stringstream ss(line);
            string from, to, distStr, timeStr;
            
            // Parse CSV: From,To,Dist,Time
            if (getline(ss, from, ',') && 
                getline(ss, to, ',') && 
                getline(ss, distStr, ',') && 
                getline(ss, timeStr, ',')) {
                
                try {
                    int dist = stoi(distStr);
                    int time = stoi(timeStr);
                    
                    // We need to re-link them manually
                    BSTNode* src = stationRegistry.searchStation(from);
                    BSTNode* dst = stationRegistry.searchStation(to);
                    if (src && dst) {
                        src->tracks.addTrack(dst->data.id, dist, time);
                    }
                } catch (...) {
                    continue; // Skip bad lines
                }
            }
        }
        trackFile.close();
    }
}

bool Graph::isPathExisting(string start, string end) {
    // 1. Get IDs
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);

    if (!startNode || !endNode) return false;

    int startId = startNode->data.id;
    int targetId = endNode->data.id;

    // 2. Setup BFS
    Queue q;
    q.enqueue(startId);
    
    // To keep track of where we have been (to avoid infinite loops)
    vector<int> visited;
    visited.push_back(startId);

    // 3. The Loop
    while (!q.isEmpty()) {
        int currentId = q.dequeue();

        // Did we find it?
        if (currentId == targetId) return true;

        // Get the actual node to access its tracks
        BSTNode* currentNode = stationRegistry.searchStationById(currentId);
        
        if (currentNode) {
            // Iterate through the Linked List of tracks
            Track* track = currentNode->tracks.getHead();
            while (track != nullptr) {
                int neighborId = track->destinationStationId;

                // If neighbor NOT visited yet
                bool alreadyVisited = false;
                for(int v : visited) {
                    if(v == neighborId) alreadyVisited = true;
                }

                if (!alreadyVisited) {
                    visited.push_back(neighborId);
                    q.enqueue(neighborId);
                }
                
                track = track->next;
            }
        }
    }

    return false; // Queue empty, never found target
}

void Graph::getFastestRoute(string start, string end) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);

    if (!startNode || !endNode) {
        cout << "One of the stations does not exist." << endl;
        return;
    }

    int startId = startNode->data.id;
    int endId = endNode->data.id;

    // Distances from start
    map<int, int> dist;
    map<int, int> previous; // To reconstruct the path
    map<int, bool> visited;
    
    // Initialize
    // Ideally iterate all nodes, but for now we rely on the map default
    dist[startId] = 0;

    // The Main Loop (Run for count of stations)
    // Note: In a real app, we'd know exactly how many stations. 
    // Here we loop enough times to cover the graph.
    for (int i = 0; i < 20; i++) { // Assuming max 20 stations for demo
        
        // 1. Find the unvisited node with smallest distance
        int u = -1;
        int minVal = INT_MAX;

        // Iterate over our known distances map
        for (auto const& [id, d] : dist) {
            if (!visited[id] && d < minVal) {
                minVal = d;
                u = id;
            }
        }

        // If we didn't find a node (or unreachable), stop
        if (u == -1 || u == endId) break;

        visited[u] = true;

        // 2. Relax Neighbors
        BSTNode* uNode = stationRegistry.searchStationById(u);
        if (uNode) {
            Track* track = uNode->tracks.getHead();
            while (track != nullptr) {
                int v = track->destinationStationId;
                int weight = track->weightTime; // FASTEST means Time

                // If we found a shorter path to v
                if (dist.find(v) == dist.end()) dist[v] = INT_MAX; // Init if new
                
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    previous[v] = u;
                }
                track = track->next;
            }
        }
    }

    // 3. Print Result
    if (dist.find(endId) == dist.end() || dist[endId] == INT_MAX) {
        cout << "No route exists between " << start << " and " << end << endl;
    } else {
        cout << "Fastest Route Time: " << dist[endId] << " minutes" << endl;
        // Reconstruct path logic could go here...
    }
}