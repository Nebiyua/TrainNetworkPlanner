#include <iostream>
#include "../include/LinkedList.h"
#include "../include/BST.h"

using namespace std;

int main() {
    cout << "--- Testing BST (Phonebook) ---" << endl;
    
    BST cityDirectory;
    
    // 1. Add Stations out of order
    cityDirectory.addStation("Dire Dawa", "DD", 2);
    cityDirectory.addStation("Addis Ababa", "ADD", 1); // Should go Left of Dire Dawa
    cityDirectory.addStation("Mekelle", "MEK", 3);     // Should go Right of Dire Dawa
    cityDirectory.addStation("Bahir Dar", "BDR", 4);   // Should go Right of Addis
    
    // 2. Print (Should be Alphabetical: Addis, Bahir Dar, Dire Dawa, Mekelle)
    cout << "\nListing all stations (A-Z):" << endl;
    cityDirectory.printAllStations();

    // 3. Search Test
    cout << "\nSearching for 'Mekelle'..." << endl;
    BSTNode* result = cityDirectory.searchStation("Mekelle");
    
    if (result != nullptr) {
        cout << "Found: " << result->data.name << " (ID: " << result->data.id << ")" << endl;
    } else {
        cout << "Not Found!" << endl;
    }

    return 0;
}