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


void BST::deleteStation(string name) {
    root = deleteByMerging(root, name);
}


BSTNode* BST::deleteByMerging(BSTNode* node, string name) {
    if (!node) return nullptr;

    if (name < node->data.name) {
        node->left = deleteByMerging(node->left, name);
    } else if (name > node->data.name) {
        node->right = deleteByMerging(node->right, name);
    } else { // Found node to delete
        BSTNode* tmp = node;
        if (!node->left) {
            node = node->right;
        } else if (!node->right) {
            node = node->left;
        } else {
            // Merge left and right subtrees
            BSTNode* leftSubtree = node->left;
            BSTNode* rightMost = leftSubtree;
            while (rightMost->right)
                rightMost = rightMost->right;
            rightMost->right = node->right;
            node = leftSubtree;
        }
        delete tmp;
    }
    return node;
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