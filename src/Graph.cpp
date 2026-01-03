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

Graph::Graph() : nextId(1) {} // Initialize station IDs starting from 1

// --- STATION & TRACK MANAGEMENT ---

// Add a new station to the registry
void Graph::addStation(string name, string code) {
    stationRegistry.addStation(name, code, nextId);          // Insert into BST
    actionHistory.push("Added Station: " + name);           // Log action
    cout << "Success: Station '" << name << "' added with ID " << nextId << endl;
    nextId++;
}

// Add a track between two stations
void Graph::addTrack(string fromStation, string toStation, int dist, int time) {
    BSTNode* sourceNode = stationRegistry.searchStation(fromStation);
    BSTNode* destNode = stationRegistry.searchStation(toStation);

    if (!sourceNode) { cout << "Error: Source station '" << fromStation << "' not found!\n"; return; }
    if (!destNode) { cout << "Error: Destination station '" << toStation << "' not found!\n"; return; }

    sourceNode->tracks.addTrack(destNode->data.id, dist, time);   // Append to adjacency list
    actionHistory.push("Added Track: " + fromStation + " -> " + toStation);
    cout << "Success: Track added from " << fromStation << " to " << toStation << endl;
}

// List all stations alphabetically (inorder BST traversal)
void Graph::listStations() {
    cout << "\n--- Network Stations (Alphabetical) ---\n";
    if (nextId == 1) {
        cout << "Database is empty.\n";
        return;
    }
    stationRegistry.printAllStations();
}

// Check if a direct (1-hop) connection exists
bool Graph::isDirectlyConnected(string start, string end) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);
    if (!startNode || !endNode) return false;

    // Traverse linked list of tracks from start station
    Track* current = startNode->tracks.getHead();
    while (current != nullptr) {
        if (current->destinationStationId == endNode->data.id) return true;
        current = current->next;
    }
    return false;
}

// --- NETWORK HEALTH & BRIDGES ---

// Helper: DFS for bridge-finding (Tarjan's algorithm)
void bridgeDfs(int u, int& timer, const vector<vector<int>>& adj, 
               vector<int>& disc, vector<int>& low, vector<int>& parent, 
               vector<pair<int,int>>& bridges) 
{
    disc[u] = low[u] = ++timer;

    for (int v : adj[u]) {
        if (v == parent[u]) continue; // Ignore immediate parent

        if (disc[v] != -1) {
            // Back-edge: update low-link value
            low[u] = min(low[u], disc[v]);
        } else {
            // Tree-edge: recurse
            parent[v] = u;
            bridgeDfs(v, timer, adj, disc, low, parent, bridges);
            low[u] = min(low[u], low[v]);

            // Identify bridge
            if (low[v] > disc[u]) bridges.push_back({u, v});
        }
    }
}

// Display advanced network statistics: busiest stations, isolated stations, and bridges
void Graph::displayNetworkStats() {
    vector<BSTNode*> nodes;
    stationRegistry.collectNodes(nodes); // Collect all BST nodes

    if (nodes.empty()) {
        cout << "Network Health Overview: No stations in the network.\n";
        return;
    }

    int n = (int)nodes.size();
    unordered_map<int,int> idToIdx;        // Map station IDs to array indices
    vector<int> idxToId(n);                // Reverse mapping
    vector<string> idxToName(n);           // Index -> station name

    for (int i = 0; i < n; i++) {
        int id = nodes[i]->data.id;
        idToIdx[id] = i;
        idxToId[i] = id;
        idxToName[i] = nodes[i]->data.name;
    }

    // Compute in/out-degrees & build undirected adjacency for bridge detection
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
                outdeg[u]++; indeg[v]++;

                // For bridge detection, treat graph as undirected
                undirectedNbrSet[u].insert(v);
                undirectedNbrSet[v].insert(u);
            }
            t = t->next;
        }
    }

    // Convert sets to vectors for DFS
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; i++) adj[i].assign(undirectedNbrSet[i].begin(), undirectedNbrSet[i].end());

    // Find bridges using DFS
    vector<int> disc(n, -1), low(n, -1), parent(n, -1);
    vector<pair<int,int>> bridges;
    int timer = 0;
    for (int i = 0; i < n; i++)
        if (disc[i] == -1) bridgeDfs(i, timer, adj, disc, low, parent, bridges);

    // Identify busiest and isolated stations
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

    // Print Network Health Summary
    cout << "\n--- 📊 NETWORK HEALTH OVERVIEW (Advanced) ---" << endl;
    cout << "Total Stations: " << n << endl;

    cout << "\n🏆 Busiest Station(s) (In + Out Connections):" << endl;
    for (int i : busiestIdx) cout << "   - " << idxToName[i] << " (Total: " << maxTotal << ")" << endl;

    cout << "\n⚠️  Isolated Stations (Total Disconnected):" << endl;
    if (isolatedIdx.empty()) cout << "   (None - Network is connected)" << endl;
    else for (int i : isolatedIdx) cout << "   - " << idxToName[i] << endl;

    cout << "\n🌉 Critical Links (Bridges):" << endl;
    cout << "   (Removing these tracks would split the network)" << endl;
    if (bridges.empty()) cout << "   (None - Network has redundant paths)" << endl;
    else for (auto [u, v] : bridges) cout << "   - " << idxToName[u] << " <--> " << idxToName[v] << endl;
    cout << "---------------------------------------------" << endl;
}

// --- FILE I/O ---

// Save stations and tracks to CSV
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

// Load stations and tracks from CSV
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
                } catch (...) { continue; } // Skip invalid lines
            }
        }
        trackFile.close();
    }
}

// --- PATH & ROUTE FINDING ---

// BFS to check if any path exists (connectivity)
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

        // Traverse all neighbors
        for (Track* track = currentNode->tracks.getHead(); track; track = track->next) {
            int neighborId = track->destinationStationId;
            // Skip if already visited
            if (find(visited.begin(), visited.end(), neighborId) == visited.end()) {
                visited.push_back(neighborId);
                q.enqueue(neighborId);
            }
        }
    }
    return false;
}

// Dijkstra Algorithm: finds shortest path by time or distance
void Graph::getShortestPath(string start, string end, bool byTime) {
    BSTNode* startNode = stationRegistry.searchStation(start);
    BSTNode* endNode = stationRegistry.searchStation(end);
    if (!startNode || !endNode) { cout << "One of the stations does not exist.\n"; return; }

    int startId = startNode->data.id;
    int endId = endNode->data.id;

    map<int,int> dist;    // Shortest distance/time found so far
    map<int,int> parent;  // For path reconstruction
    map<int,bool> visited;

    dist[startId] = 0;

    // Run iterations to cover all reachable nodes
    for (int i = 0; i < 100; i++) {
        int u = -1;
        int minVal = INT_MAX;

        // Pick closest unvisited node
        for (auto const& [id,d] : dist)
            if (!visited[id] && d < minVal) { minVal = d; u = id; }

        if (u == -1 || u == endId) break;
        visited[u] = true;

        BSTNode* uNode = stationRegistry.searchStationById(u);
        if (!uNode) continue;

        // Relax all neighbors
        for (Track* track = uNode->tracks.getHead(); track; track = track->next) {
            int v = track->destinationStationId;
            int weight = byTime ? track->weightTime : track->weightDistance;
            if (dist.find(v) == dist.end()) dist[v] = INT_MAX;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Record path
            }
        }
    }

    // Check if path exists
    if (dist.find(endId) == dist.end() || dist[endId] == INT_MAX) {
        cout << "No route exists between " << start << " and " << end << endl;
        return;
    }

    string unit = byTime ? "minutes" : "km";
    cout << "Result: " << dist[endId] << " " << unit << endl;

    // Reconstruct path from end -> start
    vector<string> path;
    int curr = endId;
    while (curr != startId) {
        BSTNode* node = stationRegistry.searchStationById(curr);
        path.push_back(node->data.name);
        curr = parent[curr];
    }
    // Add start station
    path.push_back(startNode->data.name);

    // Print path Start -> End
    cout << "Path: ";
    for (int i = path.size()-1; i >= 0; i--) {
        cout << path[i];
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}
