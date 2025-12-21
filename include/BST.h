#ifndef BST_H
#define BST_H

#include "Station.h"
#include "LinkedList.h"

struct BSTNode {
    Station data;
    LinkedList tracks; // COMPOSITION: Each station node has a list of tracks
    BSTNode* left;
    BSTNode* right;

    BSTNode(Station s) {
        data = s;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
private:
    BSTNode* root;

    // Helpers
    BSTNode* insertRecursive(BSTNode* node, Station s);
    BSTNode* searchRecursive(BSTNode* node, string name);
    BSTNode* searchByIdRecursive(BSTNode* node, int id);
    void inorderRecursive(BSTNode* node); // For printing A-Z
    void deleteTree(BSTNode* node);

public:
    BST();
    ~BST();

    void addStation(string name, string code, int id);
    BSTNode* searchStation(string name);
    BSTNode* searchStationById(int id);
    void printAllStations();
    
    // We need to expose the root for advanced Graph algorithms later
    BSTNode* getRoot(); 
};

#endif