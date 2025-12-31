#include "../include/Graph.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits> // For INT_MAX (Infinity)
#include <map>     // Easier to map ID -> Distance
#include <unordered_map>
#include <unordered_set>

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

void Graph::deleteStation(string name) {
    // Check if station exists
    BSTNode* node = stationRegistry.searchStation(name);
    if (!node) {
        cout << "Error: Station '" << name << "' not found!" << endl;
        return;
    }

    // Delete from BST
    stationRegistry.deleteStation(name);

    actionHistory.push("Deleted Station: " + name);

    cout << "Success: Station '" << name << "' deleted." << endl;
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

void Graph::loadData() {

    cout<<"loaded";
}
void Graph::saveData() {
    cout<<"saved";
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



// Helper DFS for bridges (Tarjan) on UNDIRECTED adjacency list
static void bridgeDfs(
    int u,
    int& timer,
    const std::vector<std::vector<int>>& adj,
    std::vector<int>& disc,
    std::vector<int>& low,
    std::vector<int>& parent,
    std::vector<std::pair<int,int>>& bridges
) {
    disc[u] = low[u] = ++timer;

    for (int v : adj[u]) {
        if (disc[v] == -1) {
            parent[v] = u;
            bridgeDfs(v, timer, adj, disc, low, parent, bridges);

            low[u] = std::min(low[u], low[v]);

            // Bridge condition in undirected graph
            if (low[v] > disc[u]) {
                bridges.push_back({u, v});
            }
        } else if (v != parent[u]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

void Graph::networkHealthOverview() {
    // 0) Collect all stations from BST
    std::vector<BSTNode*> nodes;
    stationRegistry.collectNodes(nodes);

    if (nodes.empty()) {
        cout << "Network Health Overview: No stations in the network.\n";
        return;
    }

    // 1) Build ID <-> index mapping (IDs may not be contiguous after deletes)
    int n = (int)nodes.size();
    std::unordered_map<int,int> idToIdx;
    std::vector<int> idxToId(n);
    std::vector<std::string> idxToName(n);

    for (int i = 0; i < n; i++) {
        int id = nodes[i]->data.id;
        idToIdx[id] = i;
        idxToId[i] = id;
        idxToName[i] = nodes[i]->data.name; // adjust if your field name differs
    }

    // 2) Compute in-degree and out-degree and build undirected adjacency (for bridges)
    std::vector<int> indeg(n, 0), outdeg(n, 0);

    // Use set per node to avoid duplicate neighbors (multiple tracks)
    std::vector<std::unordered_set<int>> undirectedNbrSet(n);

    for (int i = 0; i < n; i++) {
        BSTNode* uNode = nodes[i];
        int uId = uNode->data.id;

        Track* t = uNode->tracks.getHead();
        while (t != nullptr) {
            int vId = t->destinationStationId;

            auto it = idToIdx.find(vId);
            if (it != idToIdx.end()) {
                int u = i;
                int v = it->second;

                outdeg[u] += 1;
                indeg[v] += 1;

                // undirected view: add both directions
                undirectedNbrSet[u].insert(v);
                undirectedNbrSet[v].insert(u);
            }
            t = t->next;
        }
    }

    // Convert neighbor sets to adjacency list for bridge algorithm
    std::vector<std::vector<int>> adj(n);
    for (int i = 0; i < n; i++) {
        adj[i].assign(undirectedNbrSet[i].begin(), undirectedNbrSet[i].end());
    }

    // 3) Isolated stations: indeg=0 and outdeg=0
    std::vector<int> isolatedIdx;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0 && outdeg[i] == 0) isolatedIdx.push_back(i);
    }

    // 4) Busiest stations: max(total degree = in + out)
    int maxTotal = 0;
    for (int i = 0; i < n; i++) {
        maxTotal = std::max(maxTotal, indeg[i] + outdeg[i]);
    }

    std::vector<int> busiestIdx;
    for (int i = 0; i < n; i++) {
        if (indeg[i] + outdeg[i] == maxTotal) busiestIdx.push_back(i);
    }

    // 5) Bridges (critical links) on undirected adjacency
    std::vector<int> disc(n, -1), low(n, -1), parent(n, -1);
    std::vector<std::pair<int,int>> bridges;
    int timer = 0;

    for (int i = 0; i < n; i++) {
        if (disc[i] == -1) {
            bridgeDfs(i, timer, adj, disc, low, parent, bridges);
        }
    }

    // 6) Print overview
    cout << "\n=============================\n";
    cout << " Network Health Overview\n";
    cout << "=============================\n";

    cout << "Total stations: " << n << "\n";

    cout << "\nIsolated stations (no incoming/outgoing tracks): ";
    if (isolatedIdx.empty()) {
        cout << "None\n";
    } else {
        cout << "\n";
        for (int i : isolatedIdx) {
            cout << " - " << idxToName[i] << " (ID " << idxToId[i] << ")\n";
        }
    }

    cout << "\nBusiest station(s) by total degree (in+out = " << maxTotal << "):\n";
    for (int i : busiestIdx) {
        cout << " - " << idxToName[i]
             << " (ID " << idxToId[i]
             << "), in=" << indeg[i] << ", out=" << outdeg[i]
             << ", total=" << (indeg[i] + outdeg[i]) << "\n";
    }

    cout << "\nCritical links (bridges) in UNDIRECTED network view:\n";
    if (bridges.empty()) {
        cout << " - None (no single link disconnects the network)\n";
    } else {
        for (auto [u, v] : bridges) {
            // Print as station names
            cout << " - " << idxToName[u] << " <-> " << idxToName[v]
                 << " (IDs " << idxToId[u] << " <-> " << idxToId[v] << ")\n";
        }
    }

    cout << "=============================\n";
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