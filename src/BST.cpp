#include "../include/BST.h"
#include <iostream>

using namespace std;

BST::BST() : root(nullptr) {}

BST::~BST() {
    deleteTree(root);
}

// Recursively delete all nodes
void BST::deleteTree(BSTNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Public interface to add a station
void BST::addStation(string name, string code, int id) {
    Station newStation(name, code, id);
    root = insertRecursive(root, newStation);
}

// Recursive insertion by station name
BSTNode* BST::insertRecursive(BSTNode* node, Station s) {
    if (!node) return new BSTNode(s);

    if (s.name < node->data.name)
        node->left = insertRecursive(node->left, s);
    else if (s.name > node->data.name)
        node->right = insertRecursive(node->right, s);

    return node;
}

// Search by name
BSTNode* BST::searchStation(string name) {
    return searchRecursive(root, name);
}

BSTNode* BST::searchRecursive(BSTNode* node, string name) {
    if (!node) return nullptr;
    if (node->data.name == name) return node;
    return (name < node->data.name) ? 
        searchRecursive(node->left, name) : searchRecursive(node->right, name);
}

// Print all stations in alphabetical order
void BST::printAllStations() {
    inorderRecursive(root);
}

void BST::inorderRecursive(BSTNode* node) {
    if (!node) return;
    inorderRecursive(node->left);
    cout << "Station: " << node->data.name << " [" << node->data.code << "]" << endl;
    inorderRecursive(node->right);
}

BSTNode* BST::getRoot() {
    return root;
}

// Search by ID (traverse both subtrees)
BSTNode* BST::searchStationById(int id) {
    return searchByIdRecursive(root, id);
}

BSTNode* BST::searchByIdRecursive(BSTNode* node, int id) {
    if (!node) return nullptr;
    if (node->data.id == id) return node;

    BSTNode* foundLeft = searchByIdRecursive(node->left, id);
    return foundLeft ? foundLeft : searchByIdRecursive(node->right, id);
}

// --- File saving ---

void BST::saveStationsToFile(ofstream& outFile) {
    saveStationsRecursive(root, outFile);
}

void BST::saveStationsRecursive(BSTNode* node, ofstream& outFile) {
    if (!node) return;

    // Format: Name,Code
    outFile << node->data.name << "," << node->data.code << endl;

    saveStationsRecursive(node->left, outFile);
    saveStationsRecursive(node->right, outFile);
}

void BST::saveTracksToFile(ofstream& outFile) {
    saveTracksRecursive(root, outFile);
}

void BST::saveTracksRecursive(BSTNode* node, ofstream& outFile) {
    if (!node) return;

    Track* currentTrack = node->tracks.getHead();
    while (currentTrack) {
        BSTNode* destNode = searchByIdRecursive(root, currentTrack->destinationStationId);
        if (destNode) {
            // Format: FromName,ToName,Distance,Time
            outFile << node->data.name << ","
                    << destNode->data.name << ","
                    << currentTrack->weightDistance << ","
                    << currentTrack->weightTime << endl;
        }
        currentTrack = currentTrack->next;
    }

    saveTracksRecursive(node->left, outFile);
    saveTracksRecursive(node->right, outFile);
}
