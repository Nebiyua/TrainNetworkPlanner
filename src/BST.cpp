#include "../include/BST.h"
#include <iostream>

using namespace std;

BST::BST() {
    root = nullptr;
}

void BST::deleteTree(BSTNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

BST::~BST() {
    deleteTree(root);
}

void BST::addStation(string name, string code, int id) {
    Station newStation(name, code, id);
    root = insertRecursive(root, newStation);
}

BSTNode* BST::insertRecursive(BSTNode* node, Station s) {
    if (node == nullptr) {
        return new BSTNode(s); 
    }

    if (s.name < node->data.name) {
        node->left = insertRecursive(node->left, s);
    } 
    else if (s.name > node->data.name) {
        node->right = insertRecursive(node->right, s);
    }

    return node; 
}

BSTNode* BST::searchStation(string name) {
    return searchRecursive(root, name);
}

BSTNode* BST::searchRecursive(BSTNode* node, string name) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (node->data.name == name) {
        return node;
    }

    if (name < node->data.name) {
        return searchRecursive(node->left, name);
    } else {
        return searchRecursive(node->right, name);
    }
}

void BST::printAllStations() {
    inorderRecursive(root);
}

void BST::inorderRecursive(BSTNode* node) {
    if (node != nullptr) {
        inorderRecursive(node->left);  // Visit Left (A-M)
        cout << "Station: " << node->data.name << " [" << node->data.code << "]" << endl; // Visit Self
        inorderRecursive(node->right); // Visit Right (N-Z)
    }
}

BSTNode* BST::getRoot() {
    return root;
}

BSTNode* BST::searchStationById(int id) {
    return searchByIdRecursive(root, id);
}

BSTNode* BST::searchByIdRecursive(BSTNode* node, int id) {
    if (node == nullptr) return nullptr;
    if (node->data.id == id) return node;
    
    BSTNode* foundLeft = searchByIdRecursive(node->left, id);
    if (foundLeft) return foundLeft;
    
    return searchByIdRecursive(node->right, id);
}

// In src/BST.cpp - Add at the very bottom

// --- SAVING STATIONS ---
void BST::saveStationsToFile(ofstream& outFile) {
    saveStationsRecursive(root, outFile);
}

void BST::saveStationsRecursive(BSTNode* node, ofstream& outFile) {
    if (node != nullptr) {
        // Format: Name,Code
        outFile << node->data.name << "," << node->data.code << endl;
        
        saveStationsRecursive(node->left, outFile);
        saveStationsRecursive(node->right, outFile);
    }
}

// --- SAVING TRACKS ---
void BST::saveTracksToFile(ofstream& outFile) {
    saveTracksRecursive(root, outFile);
}

void BST::saveTracksRecursive(BSTNode* node, ofstream& outFile) {
    if (node != nullptr) {
        // Look at every track leaving this station
        Track* currentTrack = node->tracks.getHead();
        while (currentTrack != nullptr) {
            // We need to find the destination NAME (we only have ID in the track)
            // This is a bit inefficient (O(n)), but fine for saving files.
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
}