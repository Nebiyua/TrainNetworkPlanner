#ifndef BST_H
#define BST_H

#include <fstream>
#include "Station.h"
#include "LinkedList.h"
#include <vector>
#include <string>

// Node of the Binary Search Tree
struct BSTNode {
    Station data;
    LinkedList tracks;   // Tracks connected to this station
    BSTNode* left;
    BSTNode* right;

    BSTNode(Station s) : data(s), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for stations (ordered by name)
class BST {
private:
    BSTNode* root;

    // Internal helpers
    BSTNode* insertRecursive(BSTNode* node, Station s);
    BSTNode* searchRecursive(BSTNode* node, string name);
    BSTNode* searchByIdRecursive(BSTNode* node, int id);
    void inorderRecursive(BSTNode* node);
    void deleteTree(BSTNode* node);
    void getStatsRecursive(BSTNode* node, vector<string>& isolated, string& busiest, int& maxConnections);
    void collectNodesRecursive(BSTNode* node, vector<BSTNode*>& nodes);
    void saveStationsRecursive(BSTNode* node, ofstream& outFile);
    void saveTracksRecursive(BSTNode* node, ofstream& outFile);

public:
    BST();
    ~BST();

    void addStation(string name, string code, int id);
    BSTNode* searchStation(string name);
    BSTNode* searchStationById(int id);
    void printAllStations();
    void getNetworkStats(vector<string>& isolated, string& busiest, int& maxConnections);
    void collectNodes(vector<BSTNode*>& nodes);

    void saveStationsToFile(ofstream& outFile);
    void saveTracksToFile(ofstream& outFile);

    // Needed for later graph algorithms
    BSTNode* getRoot();
};

#endif
