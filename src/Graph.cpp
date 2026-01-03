#include "../include/Graph.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
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
    if (nextId == 1) {
        cout << "Database is empty.\n";
        return;
    }
    stationRegistry.printAllStations();
}

// Check if there is a direct (1-hop) connection
bool Graph::isDirectlyConnected(string start, string end) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);
    if (!startNode || !endNode) return false;

    // Iterate through linked list to see if destination is there
    Track* current = startNode->tracks.getHead();
    while (current != nullptr) {
        if (current->destinationStationId == endNode->data.id) {
            return true; 
        }
        current = current->next;
    }
    return false;
}

// Show network health (Isolated stations, Busiest station)
void bridgeDfs(int u, int& timer, const vector<vector<int>>& adj, 
               vector<int>& disc, vector<int>& low, vector<int>& parent, 
               vector<pair<int,int>>& bridges) {
    disc[u] = low[u] = ++timer;
    
    for (int v : adj[u]) {
        if (v == parent[u]) continue; // Don't go back to parent immediately
        
        if (disc[v] != -1) {
            // Back-edge
            low[u] = min(low[u], disc[v]);
        } else {
            // Tree-edge
            parent[v] = u;
            bridgeDfs(v, timer, adj, disc, low, parent, bridges);
            low[u] = min(low[u], low[v]);
            
            // Bridge condition
            if (low[v] > disc[u]) {
                bridges.push_back({u, v});
            }
        }
    }
}

void Graph::displayNetworkStats() {
    // 0) Collect all stations from BST
    vector<BSTNode*> nodes;
    stationRegistry.collectNodes(nodes);

    if (nodes.empty()) {
        cout << "Network Health Overview: No stations in the network.\n";
        return;
    }

    // 1) Build ID <-> index mapping
    // We map arbitrary Station IDs (1, 5, 99) to a clean array index (0, 1, 2...)
    int n = (int)nodes.size();
    unordered_map<int,int> idToIdx;
    vector<int> idxToId(n);
    vector<string> idxToName(n);

    for (int i = 0; i < n; i++) {
        int id = nodes[i]->data.id;
        idToIdx[id] = i;
        idxToId[i] = id;
        idxToName[i] = nodes[i]->data.name;
    }

    // 2) Compute Degrees & Build Undirected Adjacency for Bridges
    vector<int> indeg(n, 0), outdeg(n, 0);
    vector<unordered_set<int>> undirectedNbrSet(n);

    for (int i = 0; i < n; i++) {
        BSTNode* uNode = nodes[i];
        
        Track* t = uNode->tracks.getHead();
        while (t != nullptr) {
            int vId = t->destinationStationId;

            if (idToIdx.count(vId)) {
                int u = i;
                int v = idToIdx[vId];

                outdeg[u]++;
                indeg[v]++;

                // For bridges, we treat graph as undirected
                undirectedNbrSet[u].insert(v);
                undirectedNbrSet[v].insert(u);
            }
            t = t->next;
        }
    }

    // Convert sets to vector for DFS
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; i++) {
        adj[i].assign(undirectedNbrSet[i].begin(), undirectedNbrSet[i].end());
    }

    // 3) Find Bridges
    vector<int> disc(n, -1), low(n, -1), parent(n, -1);
    vector<pair<int,int>> bridges;
    int timer = 0;

    for (int i = 0; i < n; i++) {
        if (disc[i] == -1) {
            bridgeDfs(i, timer, adj, disc, low, parent, bridges);
        }
    }

    // 4) Find Busiest & Isolated
    vector<int> isolatedIdx;
    vector<int> busiestIdx;
    int maxTotal = -1;

    for (int i = 0; i < n; i++) {
        int total = indeg[i] + outdeg[i];
        if (total == 0) isolatedIdx.push_back(i);
        
        if (total > maxTotal) {
            maxTotal = total;
            busiestIdx.clear();
            busiestIdx.push_back(i);
        } else if (total == maxTotal) {
            busiestIdx.push_back(i);
        }
    }

    // 5) Print Results
    cout << "\n--- 📊 NETWORK HEALTH OVERVIEW (Advanced) ---" << endl;
    cout << "Total Stations: " << n << endl;

    cout << "\n🏆 Busiest Station(s) (In + Out Connections):" << endl;
    for (int i : busiestIdx) {
        cout << "   - " << idxToName[i] << " (Total: " << maxTotal << ")" << endl;
    }

    cout << "\n⚠️  Isolated Stations (Total Disconnected):" << endl;
    if (isolatedIdx.empty()) {
        cout << "   (None - Network is connected)" << endl;
    } else {
        for (int i : isolatedIdx) cout << "   - " << idxToName[i] << endl;
    }

    cout << "\n🌉 Critical Links (Bridges):" << endl;
    cout << "   (Removing these tracks would split the network)" << endl;
    if (bridges.empty()) {
        cout << "   (None - Network has redundant paths)" << endl;
    } else {
        for (auto [u, v] : bridges) {
            cout << "   - " << idxToName[u] << " <--> " << idxToName[v] << endl;
        }
    }
    cout << "---------------------------------------------" << endl;
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
    vector<int> visited;
    visited.push_back(startId);

    while (!q.isEmpty()) {
        int currentId = q.dequeue();
        if (currentId == targetId) return true;

        BSTNode* currentNode = stationRegistry.searchStationById(currentId);
        if (!currentNode) continue;

        for (Track* track = currentNode->tracks.getHead(); track; track = track->next) {
            int neighborId = track->destinationStationId;
            
            // Check if visited (simple vector find)
            bool found = false;
            for(int v : visited) if(v == neighborId) found = true;

            if (!found) {
                visited.push_back(neighborId);
                q.enqueue(neighborId);
            }
        }
    }
    return false;
}

// Dijkstra Algorithm: Supports optimization by Time or Distance
void Graph::getShortestPath(string start, string end, bool byTime) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);
    if (!startNode || !endNode) {
        cout << "One of the stations does not exist.\n";
        return;
    }

    int startId = startNode->data.id;
    int endId = endNode->data.id;

    map<int, int> dist;          // Shortest distance/time found
    map<int, int> parent;        // For path reconstruction
    map<int, bool> visited;

    dist[startId] = 0;

    // Run enough iterations to cover the graph
    for (int i = 0; i < 100; i++) { 
        int u = -1;
        int minVal = INT_MAX;

        // Find closest unvisited node
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

        // Relax neighbors
        for (Track* track = uNode->tracks.getHead(); track; track = track->next) {
            int v = track->destinationStationId;
            // Toggle metric based on user choice
            int weight = byTime ? track->weightTime : track->weightDistance;

            if (dist.find(v) == dist.end()) dist[v] = INT_MAX;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Track the path
            }
        }
    }

    if (dist.find(endId) == dist.end() || dist[endId] == INT_MAX) {
        cout << "No route exists between " << start << " and " << end << endl;
    } else {
        string unit = byTime ? "minutes" : "km";
        cout << "Result: " << dist[endId] << " " << unit << endl;

        // Reconstruct path (without using <stack> header)
        vector<string> path;
        int curr = endId;
        
        // Backtrack from end to start
        while (curr != startId) {
            BSTNode* node = stationRegistry.searchStationById(curr);
            path.push_back(node->data.name);
            curr = parent[curr];
        }
        // Add start node
        BSTNode* sNode = stationRegistry.searchStationById(startId);
        path.push_back(sNode->data.name);

        // Print in reverse (Start -> End)
        cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i];
            if (i > 0) cout << " -> ";
        }
        cout << endl;
    }
}